#include "taskviewer.h"
#include "ui_taskviewer.h"
#include <QtSql>
#include <QScrollArea>
#include <QLabel>
#include "mysqlsearcher.h"
#include "errorbasemessage.h"
#include "databaseexception.h"

TaskViewer::TaskViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskViewer)
{
    ui->setupUi(this);
}

TaskViewer::~TaskViewer()
{
    delete ui;
}

void TaskViewer::View(QString taskId)
{
    QByteArray imgFromDB;

    try
    {
        mySQLsearcher::GetInstance()->GetTaskPNG(taskId,imgFromDB);
    }
    catch (DataBaseException& ex)
    {
        ErrorBaseMessage errBase;
        errBase.SetMessage(ex.getMsg());
        errBase.setModal(true); // разрешаем окну открываться
        errBase.exec(); //выполнитьв
        return;
    }

    QPixmap img;
    img.loadFromData(imgFromDB,"PNG");
    int w = img.width();
    if (w > 1138)
    {
        img = img.scaledToWidth(1138 ,Qt::SmoothTransformation);
    }
    QLabel *imageLabel = new QLabel;
    imageLabel->setPixmap(img);
    ui->scrollArea->setBackgroundRole(QPalette::Base);
    ui->scrollArea->setWidget(imageLabel);
}
