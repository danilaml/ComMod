#include "LogisticLattice.h"

#include <QtGui/QPainter>
//#include <QtCore/QTimer>
#include <algorithm>
#include <random>

constexpr auto toRgb = [](double x){
	return qRgb(static_cast<int>(x < 0.33 ? 255 * (x * 3.) : 0),
				static_cast<int>((x > 0.33 && x < 0.66) ? 255 * ((x - 0.33) * 3.) : 0),
				static_cast<int>(x >= 0.667 ? 255 * ((x - 0.667) * 3.) : 0));
};

LogisticLattice::LogisticLattice(QWidget *parent)
	: QWidget(parent), mImage(mH, mW, QImage::Format_RGB32)
{
	mGN = 250;
	mCP = 42;
	mFillValue = -1.;
	fillTableWithRandom();
	drawIterations();
}

void LogisticLattice::setState(const SettingsState &state)
{
	mGN = state.GN;
	mCP = state.CP;
	if (state.random) {
		mFillValue = -1.;
	} else {
		mFillValue = state.fill;
	}
}

SettingsState LogisticLattice::getState()
{
	return {
		mGN,
		mCP,
		mFillValue,
		mFillValue < 0.
	};
}

void LogisticLattice::runSimulation(int iterations)
{
	while (iterations-- > 0) {
		drawIterations();
	}
	update();

	emit finished();
}

void LogisticLattice::resetSimulation()
{
	mImage.fill(Qt::black);
	if (mFillValue < 0.) {
		fillTableWithRandom();
	} else {
		fillTableWithValue(mFillValue);
	}
	update();
}

void LogisticLattice::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawImage(rect(), mImage);
}

void LogisticLattice::drawIterations()
{
	auto xij = [this](int i, int j){
		i = std::max(std::min<int>(i, mXs.size()), 0);
		j = std::max(std::min<int>(j, mXs[0].size()), 0);
		return mXs[i][j];
	};
	double rij = 4. * mGN / 1000.;
	// calculate Cij
	for (int i = 0; i < mH; ++i) {
		for (int j = 0; j < mW; ++j) {
			double xm = (xij(i - 1, j) + xij(i + 1, j) + xij(i, j - 1) + xij(i, j + 1)) / 4.;
			mCs[i][j] = mCP * (xm - mXs[i][j]) / 1000.;
		}
	}
	for (int i = 0; i < mH; ++i) {
		for (int j = 0; j < mW; ++j) {
			mXs[i][j] = rij * mXs[i][j] * (1. - mXs[i][j]) + mCs[i][j];
			mXs[i][j] = std::max(std::min(mXs[i][j], 1.0), 0.0);
			mImage.setPixel(i, j, toRgb(mXs[i][j]));
		}
	}
}

void LogisticLattice::fillTableWithRandom()
{
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0., 1.);
	for (int i = 0; i < mH; ++i) {
		for (int j = 0; j < mW; ++j) {
			mXs[i][j] = dis(gen);
			mImage.setPixel(i, j, toRgb(mXs[i][j]));
		}
	}
}

void LogisticLattice::fillTableWithValue(double value)
{
	for (int i = 0; i < mH; ++i) {
		for (int j = 0; j < mW; ++j) {
			mXs[i][j] = value;
			mImage.setPixel(i, j, toRgb(mXs[i][j]));
		}
	}
}
