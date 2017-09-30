#ifndef MANDELBROTRENDERTHREAD_H
#define MANDELBROTRENDERTHREAD_H

#include "RenderThread.h"

class MandelbrotRenderThread : public RenderThread
{
	Q_OBJECT
public:
	explicit MandelbrotRenderThread(QObject *parent = nullptr);

protected:
	void run() override;
};

#endif // MANDELBROTRENDERTHREAD_H
