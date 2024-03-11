#include "acceptcall.h"
#include "ui_acceptcall.h"

AcceptCall::AcceptCall(QDialog*parent) :
    QDialog(parent),
    ui(new Ui::AcceptCall)
{
    ui->setupUi(this);
    this->setFixedSize(baseSize);
    ui->textBrowser_Detail->insertPlainText("TODO");
    timer = new QTimer();
    QObject::connect(ui->pushButton_Accept, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(ui->pushButton_Refuse, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(timer, &QTimer::timeout, this, &QDialog::reject);
    timer->start(1000 * 15);//15s timeoutÄÄ¸ö

}

AcceptCall::~AcceptCall()
{
    delete timer;
    delete ui;
}

void AcceptCall::on_toolButton_showDetail_clicked() {
    if(this->size() == baseSize)
        this->setFixedSize(detailedSize);
    else
        this->setFixedSize(baseSize);

}

void AcceptCall::setBasicInfo(std::string name, QPixmap profile) {
    ui->label_Name->setText(QString::fromStdString(name));
    ui->label_Profile->setPixmap(profile);
}