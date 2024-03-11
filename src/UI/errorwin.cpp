#include "errorwin.h"
#include "ui_errorwin.h"

ErrorWin::ErrorWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ErrorWin)
{
    ui->setupUi(this);
}

ErrorWin::~ErrorWin()
{
    delete ui;
}

void ErrorWin::setErrorMsg(std::string errorMsg) {
    ui->label->setText(QString::fromStdString(errorMsg));
}