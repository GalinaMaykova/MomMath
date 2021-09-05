#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchdialog.h"
#include "databaseexception.h"
#include "errorbasemessage.h"

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

//нажатие кнопки "поиск"
void MainWindow::on_pushButton_clicked() //кнпка поиска
{
    SearchDialog searchD;
    searchD.initTable();
    QString DBPath =  QCoreApplication::applicationDirPath();
    QString DBName = DBPath + "/MomDB.db";
    if(!QFile(DBName).exists())
    {
        ErrorBaseMessage errBase;
        errBase.setModal(true); // разрешаем окну открываться
        errBase.exec(); //выполнить
        return;
    }
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
