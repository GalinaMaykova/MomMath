#include "taskviewer.h"
#include "ui_taskviewer.h"
#include <QtSql>
#include <QScrollArea>
#include <QLabel>

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

void TaskViewer::View(QString taskId){
QString DBPath =  QCoreApplication::applicationDirPath();
    QString DBName = DBPath + "/MomDB.db";

        QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
        sdb.setDatabaseName(DBName);
        sdb.open();

QSqlQuery query1;
if (!query1.exec("select text from task where TaskId = " + taskId)){

    ui->label_2->setText("Невозможно загрузить задачу");
   return;
}
query1.first();
QByteArray fromDB = query1.value(0).toByteArray();


QPixmap img;
img.loadFromData(fromDB,"PNG");

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
