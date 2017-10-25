#ifndef ANTGAMEFIELD_H
#define ANTGAMEFIELD_H

#include <QWidget>
#include <QTimer>
#include <QtGui/QImage>

enum class Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

class AntGameField : public QWidget
{
    Q_OBJECT
public:
    explicit AntGameField(QWidget *parent = nullptr);

    void resizeEvent(QResizeEvent * event);

protected:
    void paintEvent(QPaintEvent *) override;

public slots:
    void updateField(int steps, int scale, Direction dir, int time);
    void nextStep();

private:
    int mScale;
    int mSteps;
    int mCurStep;
    int mStepTime;
    QPoint mCurPos;
    Direction mDir;
    QImage mImage;
    QTimer mTimer;
};

#endif // ANTGAMEFIELD_H
