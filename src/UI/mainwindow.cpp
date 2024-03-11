#include <LApp/LAppDelegate.hpp>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Network_QT.h>
#include <qpixmap.h>
#include <settingwin.h>
#include <qvalidator.h>
#include <errorwin.h>
#include <detection.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
	, mirrorWindow(new VideoWindow)
{
    ui->setupUi(this);
    settingWin = new SettingWin();
    aboutWin = nullptr;

    QObject::connect(ui->actionExit,&QAction::triggered,this,&MainWindow::endProgram);
    QObject::connect(ui->actionAbout,&QAction::triggered,this,[this](){this->aboutWin = this->aboutWin == nullptr ?  new About():this->aboutWin;this->aboutWin->show();});
    QObject::connect(ui->actionSetting,&QAction::triggered,this,[this](){
        this->settingWin = this->settingWin == nullptr ?  new SettingWin():
            this->settingWin;this->settingWin->show(); QObject::connect(settingWin, &SettingWin::settingApplySuccess, this, &MainWindow::updateInfo, Qt::QueuedConnection);
        });
    ui->lineEdit_Port->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    updateInfo();
    
    listener = new QThread(this);
    listener->start();
    Network_QT::getInstance()->moveToThread(listener);
    audioThread = new QThread(this);
    audioThread->start();
    audioThread->setPriority(QThread::HighPriority);

    Audio::getInstance()->moveToThread(audioThread);
    //Network_QT::getInstance()->networkInit();
    emit Network_QT::getInstance()->restartSignal();

}

MainWindow::~MainWindow()
{
	delete mirrorWindow;
	delete mirrorThread;
    delete ui;
}

void MainWindow::endProgram(){
    //do someting;
    delete aboutWin;
    this->close();
}

void MainWindow::on_pushButton_Call_clicked() {
    try {
        if (ui->lineEdit_IP->text().isEmpty()) {
            setError(ui->lineEdit_IP);
            throw std::exception("BAD_ADDRESS");
        }
        if (ui->lineEdit_Port->text().isEmpty()) {
            setError(ui->lineEdit_Port);
            throw std::exception("BAD_ADDRESS");
        }
        
        clearError(ui->lineEdit_Port);    
        clearError(ui->lineEdit_IP);    
        std::string host = ui->lineEdit_IP->text().toStdString();    
        host.append(":").append(ui->lineEdit_Port->text().toStdString());    
        Network_QT::getInstance()->call(host);    
        
    }
    catch (std::exception e) {
        if ((std::string)e.what() == (std::string)"NO_RESPONSE") {
            ErrorWin* temp = new ErrorWin();
            temp->setErrorMsg("Target Host No Respond!");
            temp->show();
            return;
        }
        else if ((std::string)e.what() == (std::string)"BAD_ADDRESS") {
            ErrorWin* temp = new ErrorWin();
            temp->setErrorMsg("Wrong Address");
            temp->show();
            return;
        }
        else {
            return;
        }
    }
}

void MainWindow::on_mirrorButton_clicked() {
	if (mirrorThread != nullptr && mirrorThread->isRunning()) {
		// Do nothing
	}
	else {
		FacialLandmarkDetector::getInstance()->startDetector();

		delete mirrorThread;
		mirrorThread = new QThread(this);
		auto d = LAppDelegate::GetInstance();
		d->setPeerNumber(-1);
		d->moveToThread(mirrorThread);
		mirrorThread->start();
		d->emitStartSignal();
	}
}

void MainWindow::updateInfo() {
    qDebug() << "Info Updated";
    QPixmap tempPixmap(QString::fromStdString(Setting::getSetting()->getProfilePath()));
    tempPixmap.scaled(ui->label_Profile->size(), Qt::KeepAspectRatio);
    ui->label_Profile->setScaledContents(true);
    ui->label_Profile->setPixmap(tempPixmap);
    ui->label_Name->setText(QString::fromStdString(Setting::getSetting()->getName()));

}