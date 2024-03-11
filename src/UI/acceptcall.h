#ifndef ACCEPTCALL_H
#define ACCEPTCALL_H

#include <QDialog>
#include <QTimer>
namespace Ui {
class AcceptCall;
}

class AcceptCall : public QDialog
{
    Q_OBJECT

public:
    explicit AcceptCall(QDialog*parent = nullptr);
    ~AcceptCall();
    void setBasicInfo(std::string, QPixmap profile);
public slots:
    void on_toolButton_showDetail_clicked();

private:
    Ui::AcceptCall *ui;
    QSize baseSize = QSize(317, 257);
    QSize detailedSize = QSize(317, 469);
    QTimer* timer;
};

#endif // ACCEPTCALL_H
