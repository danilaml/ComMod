#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QAction>
#include "DrawingWidget.h"
#include "MandelbrotRenderThread.h"
#include "JuliaRenderThread.h"
#include "PolyRootsThread.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), // rework: ?
	ui(new Ui::MainWindow),
	dw(new DrawingWidget(new MandelbrotRenderThread(this), this))
{
	ui->setupUi(this);
	setCentralWidget(dw);

	connect(ui->actionJulia_Set, &QAction::triggered, this, [this](){
		dw->setRenderThread(new JuliaRenderThread(dw));
	});
	connect(ui->actionMandelbrot_Set, &QAction::triggered, this, [this](){
		dw->setRenderThread(new MandelbrotRenderThread(dw));
	});
	connect(ui->actionPolynomial_Roots, &QAction::triggered, this, [this](){
		dw->setRenderThread(new PolyRootsThread(dw));
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}
