#ifndef POLYROOTSTHREAD_H
#define POLYROOTSTHREAD_H

#include "RenderThread.h"

class PolyRootsThread : public RenderThread
{
    Q_OBJECT

public:
    explicit PolyRootsThread(QObject *parent);

protected:
    void run() override;
};

#endif // POLYROOTSTHREAD_H
