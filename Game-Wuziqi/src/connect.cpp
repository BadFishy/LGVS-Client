#include "connect.h"

ConnectDialog::ConnectDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText("192.168.1.171");

    QSignalMapper *mmm = new QSignalMapper(this);

    for(int i = 0; i < 12; i++)
    {
        button[i] = new QPushButton(this);
        ui->gridLayout->addWidget(button[i], i/6,i%6);

        connect(button[i], SIGNAL(clicked()), mmm, SLOT(map()));
        if(i == 10)
        {
            mmm->setMapping(button[i],"AC");
            button[i]->setText("AC");
        }
        if(i == 11)
        {
            mmm->setMapping(button[i],".");
            button[i]->setText(".");
        }
        if(i < 10)
        {
            mmm->setMapping(button[i],QString::number(i));
            button[i]->setText(QString::number(i));
        }
    }
    connect(mmm, SIGNAL(mapped(QString)), this, SLOT(addNumber(QString)));

}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

QString ConnectDialog::getHostIp()
{
    return hostIp;
}

void ConnectDialog::addNumber(QString str)
{
    if(str == "AC")
        ui->lineEdit->setText("");
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text()+str);
        hostIp = ui->lineEdit->text();
    }
}


void ConnectDialog::on_okButton_clicked()
{
    hostIp = ui->lineEdit->text();
    emit getButton(1);
    this->close();
}

void ConnectDialog::on_cancelButton_clicked()
{
    this->close();
}
