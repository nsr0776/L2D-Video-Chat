#include "connecting.h"
#include "ui_connecting.h"

Connecting::Connecting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connecting)
{
    ui->setupUi(this);
}

Connecting::~Connecting()
{
    delete ui;
}
