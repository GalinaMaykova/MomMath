#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchdialog.h"
#include "databaseexception.h"

#include <QtSql>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TestDb()
{
    QString DBPath =  QCoreApplication::applicationDirPath();
        QString DBName = DBPath + "/MomDB.db";
        if(QFile(DBName).exists())
        {
            QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(DBName);
            if (sdb.open())
            {
                QSqlQuery query("SELECT headline FROM task");
                int i=0;
                while (query.next())
                {
                   QString name = query.value(0).toString();
                   if(i==0)
                   {
                        ui->label->setText(name);

                    }
                    i++;
                }
                sdb.close();
            }
        }
        else
        {
            ui->label->setText("нет базы данных");
        }
}


void MainWindow::on_pushButton_clicked() //кнпка поиска
{
    SearchDialog searchD;
    searchD.initTable();
    try
    {
        QVector<int> vec;
        searchD.AddToTable(vec);
    }
    catch (DataBaseException ex)
    {
        return;
    }
    searchD.setModal(true); // разрешаем окну открываться
    searchD.exec(); //выполнить

}
