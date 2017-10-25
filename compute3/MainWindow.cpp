#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LogisticLattice.h"
#include "SettingsDialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->nextButton, &QPushButton::pressed, this, [this]{
		ui->widget->runSimulation(ui->spinBox->value());
	});
	QAction *settingAction = new QAction(tr("Settings"), this);
	ui->menuBar->addAction(settingAction);
	connect(ui->resetButton, &QPushButton::pressed, ui->widget, &LogisticLattice::resetSimulation);
	connect(settingAction, &QAction::triggered, this, &MainWindow::openSettings);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openSettings()
{
	SettingsDialog settings(this);
	settings.setSettings(ui->widget->getState());
	if (settings.exec() == QDialog::Accepted) {
		ui->widget->setState(settings.getSettings());
		ui->widget->resetSimulation();
	}
}
