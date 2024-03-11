#ifndef CONNECTING_H
#define CONNECTING_H

#include <QWidget>

namespace Ui {
class Connecting;
}

class Connecting : public QWidget
{
    Q_OBJECT

public:
    explicit Connecting(QWidget *parent = nullptr);
    ~Connecting();

private:
    Ui::Connecting *ui;
};

#endif // CONNECTING_H
