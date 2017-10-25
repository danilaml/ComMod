#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "SettingsState.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

	void setSettings(const SettingsState &s);
	SettingsState getSettings() const;

private:
	Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
