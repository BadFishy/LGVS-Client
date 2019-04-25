#ifndef REGUI_H
#define REGUI_H

#include <QWidget>

#include <QCloseEvent>

namespace Ui {
class Regui;
}

class Regui : public QWidget
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent* event);

public:
    explicit Regui(QWidget *parent = 0);
    ~Regui();

private slots:
    int on_pushButton_clicked();

private:
    Ui::Regui *ui;
};

#endif // REGUI_H
