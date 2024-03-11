#include "settingwin.h"
#include "ui_SettingWin.h"
#include "setting.h"
#include "errorwin.h"
#include <QFileDialog>
#include <QtNetwork/qtcpsocket.h>
#include <QtMultimedia/QAudioDeviceInfo>
#include <Network_QT.h>

bool allClear;
SettingWin::SettingWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingWin)
{
    ui->setupUi(this);
    //Apply funtion
    QObject::connect(ui->lineEdit_Name,&QLineEdit::textChanged,this,&SettingWin::canApply);
	//TODO: Name check
    QObject::connect(ui->lineEdit_ListenPort,&QLineEdit::textChanged,this,&SettingWin::canApply);
    QObject::connect(ui->lineEdit_CallPort,&QLineEdit::textChanged,this,&SettingWin::canApply);
	QObject::connect(ui->lineEdit_AudioPort, &QLineEdit::textChanged, this, &SettingWin::canApply);
	ui->lineEdit_ListenPort->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
	ui->lineEdit_CallPort->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
	ui->lineEdit_AudioPort->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));

	ui->lineEdit_ListenPort->setText(QString::number( Setting::getSetting()->getListenPort()));
	ui->lineEdit_CallPort->setText(QString::number(Setting::getSetting()->getCallPort()));
	ui->lineEdit_AudioPort->setText(QString::number(Setting::getSetting()->getAudioPort()));
	ui->lineEdit_Name->setText(QString::fromStdString(Setting::getSetting()->getName()));

    QObject::connect(ui->comboBox_ProfilePhoto,&QComboBox::currentTextChanged,this,&SettingWin::canApply);
	ui->comboBox_ProfilePhoto->addItem("..\\..\\res\\defaultProfile.jpg");
	Setting::getSetting()->setProfile("..\\..\\res\\defaultProfile.jpg");
    QObject::connect(ui->comboBox_Model,&QComboBox::currentTextChanged,this,&SettingWin::canApply);
    QObject::connect(ui->comboBox_Camera,&QComboBox::currentTextChanged,this,&SettingWin::canApply);
	QObject::connect(ui->comboBox_InputDevice, &QComboBox::currentTextChanged, this, &SettingWin::canApply);
	QObject::connect(ui->comboBox_OutputDevice, &QComboBox::currentTextChanged, this, &SettingWin::canApply);

    QObject::connect(ui->spinBox_MaxCallQueue, QOverload<int>::of(&QSpinBox::valueChanged),this,&SettingWin::canApply);

    QObject::connect(ui->checkBox_Debug_Console,&QCheckBox::stateChanged,this,&SettingWin::canApply);
    QObject::connect(ui->checkBox_Debug_ShowCamera,&QCheckBox::stateChanged,this,&SettingWin::canApply);
    QObject::connect(ui->checkBox_Debug_ShowFEI,&QCheckBox::stateChanged,this,&SettingWin::canApply);


	allClear = true;
    //QObject::connect(ui->pushButton_Apply,&QPushButton::clicked,this,&Setting::applyFun);
	std::vector<std::string> CamList;
	int num = listDevices(CamList);
	for (int i = 0; i < num; i++) {
		//char* itemName[64] = { 0 };		sprintf(itemName[0], "ID: %d %s" ,i, CamList[i].c_str());//+ i + "\t" + CamList[i];);
		QString name;
		name.sprintf("ID:%d %s", i, CamList[i].c_str());

		ui->comboBox_Camera->addItem(name);
		
	}

	for (int i = 0; i < QAudioDeviceInfo::availableDevices(QAudio::AudioInput).size()/2;i++) {
		ui->comboBox_InputDevice->addItem(QAudioDeviceInfo::availableDevices(QAudio::AudioInput)[i].deviceName());
	}
	for (int i = 0; i < QAudioDeviceInfo::availableDevices(QAudio::AudioOutput).size() / 2; i++) {
		ui->comboBox_OutputDevice->addItem(QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)[i].deviceName());
	}

}

SettingWin::~SettingWin()
{
    delete ui;
}

void SettingWin::canApply(){
    ui->pushButton_Apply->setEnabled(true);
}

void SettingWin::on_pushButton_Apply_clicked(){
    //Do something
	Setting* setting = Setting::getSetting();
	allClear = true;
	QString errorColor = "background-color: rgba(255, 0, 0, 100)";
	//background-color: rgba(255, 0, 0, 100);
	allClear = setting->setName(ui->lineEdit_Name->text().toStdString()) ? clearError(ui->lineEdit_Name,allClear) : setError(ui->lineEdit_Name);
	allClear = setting->setListenPort(ui->lineEdit_ListenPort->text().toInt()) ? clearError(ui->lineEdit_ListenPort, allClear) : setError(ui->lineEdit_ListenPort);
	allClear = setting->setCallPort(ui->lineEdit_CallPort->text().toInt()) ? clearError(ui->lineEdit_CallPort, allClear) : setError(ui->lineEdit_CallPort);
	allClear = setting->setAudioPort(ui->lineEdit_AudioPort->text().toInt()) ? clearError(ui->lineEdit_AudioPort, allClear) : setError(ui->lineEdit_AudioPort);
	allClear = setting->setProfile(ui->comboBox_ProfilePhoto->currentText().toStdString()) ? clearError(ui->comboBox_ProfilePhoto, allClear) : setError(ui->comboBox_ProfilePhoto);
	allClear = setting->setModelID(ui->comboBox_Model->currentText().toStdString()) ? clearError(ui->comboBox_Model, allClear) : setError(ui->comboBox_Model);
	allClear = setting->setCameraID(ui->comboBox_Camera->currentIndex() == 0 ? 0 : ui->comboBox_Camera->currentIndex() - 1) ? clearError(ui->comboBox_Camera, allClear) : setError(ui->comboBox_Camera);
	setting->setMaximumListenQueue(ui->spinBox_MaxCallQueue->value());
	setting->debug = ui->checkBox_Debug_Console->isChecked();
	setting->showCamera = ui->checkBox_Debug_ShowCamera->isChecked();
	setting->ShowFR = ui->checkBox_Debug_ShowFEI->isChecked();
	setting->setOutputDevice(QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)[ui->comboBox_OutputDevice->currentIndex()]);
	setting->setInputDevice(QAudioDeviceInfo::availableDevices(QAudio::AudioInput)[ui->comboBox_InputDevice->currentIndex()]);
	if (allClear) {
		ui->pushButton_Apply->setEnabled(false);
		emit settingApplySuccess();
		emit Network_QT::getInstance()->restartSignal();
	}
	else {
		ErrorWin* temp = new ErrorWin();
		temp->setWindowFlags(Qt::Dialog);
		temp->show();
		//Error window
	}
}

void SettingWin::on_pushButton_ProfilePhoto_clicked(){
    //Do something`
	//定义文件对话框类
	QFileDialog* fileDialog = new QFileDialog(this);
	//定义文件对话框标题
	fileDialog->setWindowTitle(QStringLiteral("Select Profile"));
	//设置默认文件路径
	fileDialog->setDirectory(".");
	//设置文件过滤器
	fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	//设置视图模式
	fileDialog->setViewMode(QFileDialog::Detail);
	//打印所有选择的文件的路径
	QStringList fileNames;
	if (fileDialog->exec()) {
		ui->comboBox_ProfilePhoto->addItem(fileDialog->selectedFiles()[0]);
		ui->comboBox_ProfilePhoto->setCurrentText(fileDialog->selectedFiles()[0]);

	}
	//————————————————
	//	版权声明：本文为CSDN博主「wb175208」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
	//	原文链接：https ://blog.csdn.net/wb175208/article/details/86661722
    ui->pushButton_Apply->setEnabled(true);
}

void SettingWin::on_pushButton_Model_clicked(){
    //Do something
	QFileDialog* fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("Select Model"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("JSON(*.json)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		ui->comboBox_ProfilePhoto->setCurrentText(fileDialog->selectedFiles()[0]);
		ui->comboBox_ProfilePhoto->addItem(fileDialog->selectedFiles()[0]);
	}
    ui->pushButton_Apply->setEnabled(true);
}

bool setError(QWidget* obj) {//Hight light the error box
	obj->setStyleSheet("background-color: rgba(255, 0, 0, 100);");
	return false;
}

bool clearError(QWidget* obj, bool AC) {
	obj->setStyleSheet("");
	return AC;

}
void SettingWin::on_pushButton_OK_clicked() {
	on_pushButton_Apply_clicked();
	if (allClear)
		this->close();
}

void SettingWin::on_pushButton_Cancel_clicked() {
	this->close();
}




#pragma comment(lib, "setupapi.lib")
#define VI_MAX_CAMERAS 20
DEFINE_GUID(CLSID_SystemDeviceEnum, 0x62be5d10, 0x60eb, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86);
DEFINE_GUID(CLSID_VideoInputDeviceCategory, 0x860bb310, 0x5d01, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86);
DEFINE_GUID(IID_ICreateDevEnum, 0x29840822, 0x5b84, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86);

//列出硬件设备
//https://www.cnblogs.com/herd/p/9277402.html
int listDevices(std::vector<std::string>& list)
{
	ICreateDevEnum* pDevEnum = NULL;
	IEnumMoniker* pEnum = NULL;
	int deviceCounter = 0;
	CoInitialize(NULL);

	HRESULT hr = CoCreateInstance(
		CLSID_SystemDeviceEnum,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum,
		reinterpret_cast<void**>(&pDevEnum)
	);

	if (SUCCEEDED(hr))
	{
		hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
		if (hr == S_OK) {

			IMoniker* pMoniker = NULL;
			while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
			{
				IPropertyBag* pPropBag;
				hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
					(void**)(&pPropBag));

				if (FAILED(hr)) {
					pMoniker->Release();
					continue; // Skip this one, maybe the next one will work.
				}

				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"Description", &varName, 0);
				if (FAILED(hr))
				{
					hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				}

				if (SUCCEEDED(hr))
				{
					hr = pPropBag->Read(L"FriendlyName", &varName, 0);
					int count = 0;
					char tmp[255] = { 0 };
					while (varName.bstrVal[count] != 0x00 && count < 255)
					{
						tmp[count] = (char)varName.bstrVal[count];
						count++;
					}
					list.push_back(tmp);
				}

				pPropBag->Release();
				pPropBag = NULL;
				pMoniker->Release();
				pMoniker = NULL;

				deviceCounter++;
			}

			pDevEnum->Release();
			pDevEnum = NULL;
			pEnum->Release();
			pEnum = NULL;
		}
	}
	return deviceCounter;
}

void SettingWin::on_pushButton_AudioPortTest_clicked() {
	if (!checkPortAvalibility(ui->pushButton_AudioPortTest->text().toInt())) {
		setError(ui->lineEdit_AudioPort);
	}
	else {
		clearError(ui->lineEdit_AudioPort,true);
	}
}
void SettingWin::on_pushButton_TestListenPort_clicked() {
	if (!checkPortAvalibility(ui->pushButton_TestListenPort->text().toInt())) {
		setError(ui->lineEdit_ListenPort);
	}
	else {
		clearError(ui->lineEdit_ListenPort, true);
	}
}
void SettingWin::on_pushButton_TestCallPort_clicked() {
	if (!checkPortAvalibility(ui->pushButton_TestCallPort->text().toInt())) {
		setError(ui->lineEdit_CallPort);
	}
	else {
		clearError(ui->lineEdit_CallPort, true);
	}
}

bool checkPortAvalibility(int port) {
	QTcpSocket* temp = new QTcpSocket();
	bool state;
	state = temp->bind(port);
	delete temp;
	return state;
}