#include "searchdialog.h"
#include "ui_searchdialog.h"
#include "errorbasemessage.h"
#include "databaseexception.h"
#include "twolistselection.h"
#include "taskviewer.h"
#include "mytablemodel.h"
#include "mysqlsearcher.h"

#include <QtSql>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::SetTable(MyTableModel* myModel){
    ui->tableView->setModel(myModel);
    ui->tableView->setColumnWidth(1, 300);
}

//нажатие кнопки "изменить критерии"
void SearchDialog::on_pushButton_clicked()
{
     TwoListSelection w;
     QList<QPair<int , QString>> listProp;
     try
     {
         listProp = mySQLsearcher::GetInstance()->LoadProperties();
     }
     catch (DataBaseException& ex)
     {
         ErrorBaseMessage errBase;
         errBase.SetMessage(ex.getMsg());
         errBase.setModal(true); // разрешаем окну открываться
         errBase.exec(); //выполнитьв
         return;
     }
     QStringList allPropertName;
     for(const QPair<int , QString> &myPair : listProp)
     {
         allPropertName.append(myPair.second);
     }
     w.addAvailableItems(allPropertName);
     w.setModal(true);
     w.exec();
     QStringList sl = w.seletedItems();
     ui->textBrowser->clear();
     QVector<int> PropIDvec;
     //вывод выбранного в окошечко
     for (const QString &str : sl) {
         ui->textBrowser->append(str);
         //поиск выбранных id-шников
         for(const QPair<int , QString> &myPair : listProp)
         {
             if(myPair.second == str)
             {
                 PropIDvec.push_back(myPair.first);
             }
         }
     }
     QVector<myRow> table;
     try
     {
         table = mySQLsearcher::GetInstance()->GetTableTaskWithPtoperties(PropIDvec);
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
     SetTable(myModel);
}

//двойной клик на ячейку таблицы
void SearchDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString idcheck;
    ui->label_2->setText(QString::number(index.row()));
    QModelIndex indexID = ui->tableView->model()->index(index.row(), 0, QModelIndex());
    idcheck = ui->tableView->model()->data(indexID).toString();
    TaskViewer tv;
    tv.View(idcheck);
    tv.setModal(true);
    tv.exec();
}
