#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchdialog.h"
#include "databaseexception.h"
#include "errorbasemessage.h"
#include "mytablemodel.h"
#include "mysqlsearcher.h"
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
void MainWindow::on_searchButton_clicked() //кнпка поиска
{
    SearchDialog searchD;
    QString DBPath =  QCoreApplication::applicationDirPath();
    QString DBName = DBPath + "/MomDB.db";

    if(QFile(DBName).exists())
    {
        mySQLsearcher::GetInstance()->SetDBPath(DBName);
    }
    else
    {
        ErrorBaseMessage errBase;
        errBase.SetMessage("АААА, нет базы!!!");
        errBase.setModal(true); // разрешаем окну открываться
        errBase.exec(); //выполнить
        return;
    }
    QVector<myRow> table;

    try
    {
        table = mySQLsearcher::GetInstance()->GetTableAllTask();
    }
    catch (DataBaseException& ex)
    {
        ErrorBaseMessage errBase;
        errBase.SetMessage(ex.getMsg());
        errBase.setModal(true); // разрешаем окну открываться
        errBase.exec(); //выполнитьв
        return;
    }

    MyTableModel* myModel = new MyTableModel();
    myModel->refreshData(table);
    searchD.SetTable(myModel);
    searchD.setModal(true); // разрешаем окну открываться
    searchD.exec(); //выполнить
}


