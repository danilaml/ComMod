#ifndef LOGISTICLATTICE_H
#define LOGISTICLATTICE_H

#include <QWidget>
#include <QtGui/QImage>
#include <array>

#include "SettingsState.h"

struct SettingsState;

class LogisticLattice : public QWidget
{
	Q_OBJECT
public:
	explicit LogisticLattice(QWidget *parent = nullptr);

	void setState(const SettingsState &state);
	SettingsState getState();

public slots:
	void runSimulation(int iterations);
	void resetSimulation();

protected:
    void paintEvent(QPaintEvent *) override;

private:
	void drawIterations();

	double mCP;
	double mGN;
	double mFillValue;

	QImage mImage;
	static constexpr int mH = 256;
	static constexpr int mW = 256;
	using table_t = std::array<std::array<double, mW>, mH>;
	table_t mXs{};
	table_t mCs{};

	void fillTableWithRandom();
	void fillTableWithValue(double value);
};

#endif // LOGISTICLATTICE_H
