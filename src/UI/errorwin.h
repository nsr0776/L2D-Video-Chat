#ifndef ERRORWIN_H
#define ERRORWIN_H

#include <QWidget>

namespace Ui {
class ErrorWin;
}

class ErrorWin : public QWidget
{
    Q_OBJECT

public:
    explicit ErrorWin(QWidget *parent = nullptr);
    ~ErrorWin();

    void setErrorMsg(std::string);

private:
    Ui::ErrorWin *ui;
};

#endif // ERRORWIN_H
