#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <map>

const std::map<QString, Direction> toEnum = {
    {"Up", Direction::UP},
    {"Down", Direction::DOWN},
    {"Left", Direction::LEFT},
    {"Right", Direction::RIGHT}
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [this](){
       ui->widget->updateField(ui->spinBox->value(),
                               ui->spinBox_2->value(),
                               toEnum.at(ui->comboBox->currentText()));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
