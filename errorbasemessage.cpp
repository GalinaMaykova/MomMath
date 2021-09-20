#include "errorbasemessage.h"
#include "ui_errorbasemessage.h"

ErrorBaseMessage::ErrorBaseMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorBaseMessage)
{
    ui->setupUi(this);
}

ErrorBaseMessage::~ErrorBaseMessage()
{
    delete ui;
}

void ErrorBaseMessage::SetMessage(QString msg)
{
    ui->label->setText(msg);

}

void ErrorBaseMessage::on_pushButton_clicked()
{
    QWidget::close();
}
