#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LogisticLattice.h"
#include "SettingsDialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	mTimer.setInterval(100);
	connect(&mTimer, &QTimer::timeout, this, [this]{
		ui->widget->runSimulation(ui->spinBox->value());
	});

	connect(ui->nextButton, &QPushButton::pressed, this, [this]{
		if (mTimer.isActive()) {
			mTimer.stop();
			ui->nextButton->setEnabled(true);
			ui->resetButton->setEnabled(true);
			ui->checkBox->setEnabled(true);
			ui->nextButton->setText(tr("Next"));
		} else {
			ui->resetButton->setDisabled(true);
			ui->checkBox->setDisabled(true);
			if (!ui->checkBox->isChecked()) {
				ui->nextButton->setText(tr("Stop"));
				mTimer.start();
			} else {
				ui->nextButton->setDisabled(true);
			}
			ui->widget->runSimulation(ui->spinBox->value());
		}
	});

	connect(ui->widget, &LogisticLattice::finished, this, [this]{
		if (!mTimer.isActive()) {
			ui->nextButton->setEnabled(true);
			ui->resetButton->setEnabled(true);
			ui->checkBox->setEnabled(true);
		}
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
