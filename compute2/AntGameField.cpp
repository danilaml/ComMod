#include "AntGameField.h"

#include <QPainter>
#include <QResizeEvent>

#include <QDebug>

constexpr int stepTime = 50; // todo - make this a parameter?

AntGameField::AntGameField(QWidget *parent) :
    QWidget(parent),
    mScale(8),
    mSteps(0),
    mCurStep(0),
    mCurPos(),
    mDir(Direction::UP),
    mImage()
{
    mImage.fill(Qt::white);
    connect(&mTimer, &QTimer::timeout, this, &AntGameField::nextStep);
}

void AntGameField::resizeEvent(QResizeEvent * event)
{
    event->accept();

    const QSize curSize = size();
    const int min = std::min(curSize.width(), curSize.height());
    resize(min, min);
}

void AntGameField::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(rect(), mImage);
}

void AntGameField::updateField(int steps, int scale, Direction dir)
{
    mScale = scale;
    mImage = QImage(size() / mScale, QImage::Format_RGB32); // maybe use mono
    mImage.fill(Qt::white);
    mCurPos = rect().center() / mScale;
    mDir = dir;
    mSteps = steps;
    mCurStep = 0;

    mTimer.start(stepTime);
}

void AntGameField::nextStep()
{
    constexpr QPoint dxdy[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    constexpr Direction left[] = {
        Direction::LEFT,
        Direction::RIGHT,
        Direction::DOWN,
        Direction::UP
    };
    constexpr Direction right[] = {
        Direction::RIGHT,
        Direction::LEFT,
        Direction::UP,
        Direction::DOWN
    };
    constexpr QRgb whiteC = qRgb(255, 255, 255);
    constexpr QRgb blackC = qRgb(0, 0, 0);
    if (mCurStep == mSteps) {
        mTimer.stop();
    } else {
        mCurPos += dxdy[static_cast<int>(mDir)];
        //qDebug() << "Currently in " << mCurPos << '\n';
        //qDebug() << "The dir is: " << static_cast<int>(mDir) << '\n';
        if (mImage.rect().contains(mCurPos)) {
            if (mImage.pixel(mCurPos) == blackC) {
                mImage.setPixel(mCurPos, whiteC);
                mDir = left[static_cast<int>(mDir)];
            } else {
                mImage.setPixel(mCurPos, blackC);
                mDir = right[static_cast<int>(mDir)];
            }
            update();
            mCurStep++;
        } else {
            mCurStep = mSteps;
            mTimer.stop(); // not needed?
        }
    }
}
