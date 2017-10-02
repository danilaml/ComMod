#ifndef JULIARENDERTHREAD_H
#define JULIARENDERTHREAD_H

#include "RenderThread.h"

class JuliaRenderThread : public RenderThread
{
    Q_OBJECT

public:
    explicit JuliaRenderThread(QObject *parent = nullptr);

protected:
    void run() override;
};

#endif // JULIARENDERTHREAD_H
