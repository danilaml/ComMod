#include "MandelbrotRenderThread.h"
#include <QtGui/QRgb>
#include <QtGui/QImage>
#include <complex>

MandelbrotRenderThread::MandelbrotRenderThread(QObject *parent) : RenderThread(parent)
{
}

void MandelbrotRenderThread::run()
{
    constexpr int maxIterations = 100; // 150-170?
    constexpr double rectSize = 2.0; // bigger?
    constexpr QRgb inColor = qRgb(255, 255, 255);
    constexpr QRgb outColor = qRgb(0, 0, 0);
    while (true) {
        mutex.lock();
        QSize resultSize = this->resultSize;
        double scaleFactor = this->scaleFactor;
        double centerX = this->centerX;
        double centerY = this->centerY;
        mutex.unlock();

        int halfWidth = resultSize.width() / 2;
        int halfHeight = resultSize.height() / 2;
        QImage image(resultSize, QImage::Format_RGB32);

        for (int y = -halfHeight; y < halfHeight; ++y) {
            if (restart)
                break;
            if (abort)
                return;

            QRgb *scanLine = reinterpret_cast<QRgb *>(image.scanLine(y + halfHeight));
            double ay = centerY + (y * scaleFactor);

            for (int x = -halfWidth; x < halfWidth; ++x) {
                double ax = centerX + (x * scaleFactor);
                std::complex<double> z = 0;
                std::complex<double> c(ax, ay);
                int numIterations = 0;
                do {
                    z = z * z + c;
                    if (std::abs(z) > rectSize) {
                        break;
                    }
                } while (++numIterations < maxIterations);

                *scanLine++ = (numIterations < maxIterations) ? inColor : outColor;
            }
        }
        if (!restart) // Maybe emit it per scanline
            emit renderedImage(image, scaleFactor);
        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}
