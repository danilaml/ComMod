#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QPixmap>
#include <QWidget>
#include "renderthread.h"

class DrawingWidget : public QWidget
{
    Q_OBJECT

public:
    DrawingWidget(RenderThread *t, QWidget *parent = 0);

    void setRenderThread(RenderThread *t);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) override;
#endif
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void updatePixmap(const QImage &image, double scaleFactor);
    void zoom(double zoomFactor);

private:
    void scroll(int deltaX, int deltaY);
    void resetDefaults();

    RenderThread *thread;
    QPixmap pixmap;
    QPoint pixmapOffset;
    QPoint lastDragPos;
    double centerX;
    double centerY;
    double pixmapScale;
    double curScale;
};

#endif // DRAWINGWIDGET_H
