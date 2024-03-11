#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <iostream>
#include "strmif.h"
#include <initguid.h>
#include <vector>
#include <string>


namespace Ui {
class SettingWin;
}

class SettingWin : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWin(QWidget *parent = nullptr);
    ~SettingWin();
signals:
    void settingApplySuccess();

public slots:
    void canApply();
private slots:
    void on_pushButton_Apply_clicked();
    void on_pushButton_ProfilePhoto_clicked();
    void on_pushButton_Model_clicked();
	void on_pushButton_OK_clicked();
	void on_pushButton_Cancel_clicked();
    void on_pushButton_AudioPortTest_clicked();
    void on_pushButton_TestListenPort_clicked();
    void on_pushButton_TestCallPort_clicked();
private:
    Ui::SettingWin *ui;
};
bool setError(QWidget* obj);

bool clearError(QWidget* obj, bool AC = true);

int listDevices(std::vector<std::string>& list);

bool checkPortAvalibility(int port);


#endif // SETTING_H


