#include "JuliaRenderThread.h"
#include <QtGui/QRgb>
#include <QtGui/QImage>
#include <complex>

JuliaRenderThread::JuliaRenderThread(QObject *parent) : RenderThread(parent)
{
}

void JuliaRenderThread::run()
{
    constexpr int maxIterations = 20;
    constexpr QRgb whiteC = qRgb(255, 255, 255);
    constexpr QRgb blackC = qRgb(0, 0, 0);
    constexpr double eps = 1e-3; // 1e-6 for z^2
    constexpr double side = 0.5;
    constexpr double inf = 2.0;
    const auto f = [](const std::complex<double> &z){
        return z * z + z / 2.;
    };
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
                const double nside = side * scaleFactor;
                std::complex<double> z1(ax - nside, ay - nside);
                std::complex<double> z2(ax + nside, ay - nside);
                std::complex<double> z3(ax - nside, ay + nside);
                std::complex<double> z4(ax + nside, ay + nside);
                //std::cout << "initial: " << z1 << ", " << z2 << ", " << z3 << ", " << z4 << '\n';
                int numIterations = 0;
                do {
                    z1 = f(z1);
                    z2 = f(z2);
                    z3 = f(z3);
                    z4 = f(z4);
                    if ((std::abs(z1) < eps && std::abs(z2) < eps && std::abs(z3) < eps && std::abs(z4) < eps) ||
                        (std::abs(z1) > inf && std::abs(z2) > inf && std::abs(z3) > inf && std::abs(z4) > inf)) {
                        break;
                    }
                } while (++numIterations < maxIterations);

                if (numIterations < maxIterations) {
                    *scanLine++ = whiteC; // Not in Julia Set
                } else {
                    *scanLine++ = blackC;
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
