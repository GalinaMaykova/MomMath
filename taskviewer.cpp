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


//  int id = check.toInt();

QSqlQuery query1;
if (!query1.exec("select text from task where TaskId = " + taskId)){
//   ui->->setText("!!!Запрос не удался");
    ui->label_2->setText("!!!Запрос не удался");
   return;
}
query1.first();
QByteArray fromDB = query1.value(0).toByteArray();


QPixmap img;
img.loadFromData(fromDB,"PNG");

int w = img.width();
int h = img.height();


if (w > 1138)
{
    img = img.scaledToWidth(1138 ,Qt::SmoothTransformation);
}

QLabel *imageLabel = new QLabel;
 imageLabel->setPixmap(img);
//// imageLabel->setBackgroundRole(QPalette::Base);
 //imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
 //imageLabel->resize(img.width(), img.height());

 ui->scrollArea->setBackgroundRole(QPalette::Base);

 ui->scrollArea->setWidget(imageLabel);




//mw.setWidget(label);
//mw.resize(200, 200)


}
