#include "PolyRootsThread.h"
#include <QtGui/QRgb>
#include <QtGui/QImage>
#include <complex>
#include <cmath>

PolyRootsThread::PolyRootsThread(QObject *parent) : RenderThread(parent)
{
}

void PolyRootsThread::run()
{
    constexpr int maxIterations = 25;
    constexpr QRgb colorOne = qRgb(255, 0, 0);
    constexpr QRgb colorTwo = qRgb(0, 255, 0);
    constexpr QRgb colorThree = qRgb(0, 0, 255);
    const auto xn = [](double x, double y){
        return 2 * x / 3 + (x * x - y * y) / (x * x + y * y) / (x * x + y * y) / 3;
    };
    const auto yn = [](double x, double y){
        return 2 * y / 3 * (1 - x / (x * x + y * y) / (x * x + y * y));
    };
    constexpr std::complex<double> z1 = 1.;
    constexpr std::complex<double> z2(-0.5, - std::sqrt(3.) / 2.);
    constexpr std::complex<double> z3(-0.5, std::sqrt(3.) / 2.);
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
                double ynext = ay;
                int numIterations = 0;
                do {
                    ynext = yn(ax, ynext);
                    ax = xn(ax, ynext);
                } while (++numIterations < maxIterations);

                const std::complex<double> zn(ax, ynext);

                const double d1 = std::abs(zn - z1);
                const double d2 = std::abs(zn - z2);
                const double d3 = std::abs(zn - z3);
                const double md = std::min(d1, std::min(d2, d3));

                if (md == d1) {
                    *scanLine++ = colorOne;
                } else if (md == d2){
                    *scanLine++ = colorTwo;
                } else {
                    *scanLine++ = colorThree;
                }
            }
        }
        if (!restart)
            emit renderedImage(image, scaleFactor);
        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}
