#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);
	connect(ui->checkBox, &QCheckBox::stateChanged, this, [this](int state){
		ui->fillSpinBox->setEnabled(state == Qt::CheckState::Unchecked);
	});
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::setSettings(const SettingsState &s)
{
	ui->gnSpinBox->setValue(s.GN);
	ui->cpSpinBox->setValue(s.CP);
	ui->checkBox->setChecked(s.random);
	if (!s.random) {
		ui->fillSpinBox->setValue(s.fill);
	}
}

SettingsState SettingsDialog::getSettings() const
{
	return {
		ui->gnSpinBox->value(),
		ui->cpSpinBox->value(),
		ui->fillSpinBox->value(),
		ui->checkBox->isChecked()
	};
}
