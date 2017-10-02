#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSize>

class RenderThread : public QThread
{
    Q_OBJECT

public:
    explicit RenderThread(QObject *parent = nullptr);
    ~RenderThread();

    void render(double centerX, double centerY, double scaleFactor, QSize resultSize);

signals:
    void renderedImage(const QImage &image, double scaleFactor);

protected:

    QMutex mutex;
    QWaitCondition condition;
    double centerX;
    double centerY;
    double scaleFactor;
    QSize resultSize;
    bool restart;
    bool abort;
};

#endif // RENDERTHREAD_H
