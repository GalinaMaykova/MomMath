#include "searchdialog.h"
#include "ui_searchdialog.h"
#include "errorbasemessage.h"
#include "databaseexception.h"
#include "twolistselection.h"
#include "taskviewer.h"

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


void SearchDialog::initTable()
{
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->setHorizontalHeaderItem(
                                    0, new QTableWidgetItem("ID задачи"));
    ui->tableWidget->setHorizontalHeaderItem(
                                    1, new QTableWidgetItem("Подходящие задачи:"));

    }
// Вывод в таблицу задач, которые соответствуют списку критериев
//Если вектор пустой, выводятся все задачи
// PropertiesID - критерии, по которым искать.

void SearchDialog::AddToTable(QVector<int> PropertiesID)
{
    ui->tableWidget->setRowCount(0);
    QString myQuery;
    if (PropertiesID.empty())
    {
        myQuery = "select TaskId, headline from task";
    }
    else
    {
        QString temp;
        for(int i=0; i<PropertiesID.size() - 1; ++i)
        {
            temp += QString::number(PropertiesID[i]);
            temp += ", ";
        }
        temp +=QString::number(PropertiesID[PropertiesID.size() - 1]);

        myQuery = "select TaskId from reference "
                "WHERE PropertyID IN ("+ temp + ") "
                "GROUP BY TaskID "
                "HAVING count(distinct PropertyID) = " + QString::number(PropertiesID.size());
    }
    QString DBPath =  QCoreApplication::applicationDirPath();
        QString DBName = DBPath + "/MomDB.db";
        if(QFile(DBName).exists())
        {
            QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(DBName);
            if (sdb.open())
            {
                QSqlQuery query;
                if (!query.exec(myQuery)){
                    ui->label_3->setText("Запрос не удался");
                    return;
                }
                if (!query.first()) {
                    ui->label_3->setText("Нет задач по критериям");
                    return;
                }

                if (!PropertiesID.empty()) //сложный запрос
                {
                     do {
                        int taskID = query.value(0).toInt();
                         QSqlQuery query1;
                        if (!query1.exec("select TaskId, headline from task where TaskId = " + QString::number(taskID))){
                            ui->label_2->setText("Запрос не удался");
                            return;
                        }
                        query1.first();
                        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0,
                                                  new QTableWidgetItem(query1.value(0).toString()));
                        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1,
                                                  new QTableWidgetItem(query1.value(1).toString()));
                    }while(query.next());

                } else // простой запрос
                {
                    do {
                        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0,
                                                  new QTableWidgetItem(query.value(0).toString()));
                        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1,
                                                  new QTableWidgetItem(query.value(1).toString()));
                    } while(query.next());
                }
                ui->tableWidget->resizeColumnsToContents();
                sdb.close();
            }
        }
        else // если базы данных не существует
        {
            ErrorBaseMessage errBase;
            errBase.setModal(true); // разрешаем окну открываться
            errBase.exec(); //выполнить
            throw DataBaseException();
        }
}


//просмотр задачи при двойном нажатии
void SearchDialog::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    QString idcheck;
    QTableWidgetItem *item = ui->tableWidget->item(index.row(), 0);
    if (NULL != item) {
       idcheck = item->text();
    }
    TaskViewer tv;
    tv.View(idcheck);
    tv.setModal(true);
    tv.exec();
}


//Загрузка свойств из таблицы properties базы данных
// return - пара <id критерия, текст критерия>
QList<QPair<int , QString>> SearchDialog::LoadProperties() // должна возвращать пару id строка
{
        QList<QPair<int,QString>> answer;
        QString DBPath =  QCoreApplication::applicationDirPath();
        QString DBName = DBPath + "/MomDB.db";
        if(QFile(DBName).exists())
        {
            QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(DBName);
            if (sdb.open())
            {
                QSqlQuery query;

                if (!query.exec("select PropertyID, propertyNAME from properties")){
                    ui->label->setText("Запрос критериев не удался");
                    return answer;
                }
                if (!query.first()) {
                  //  QMessageBox::warning(this, tr("Непонятно"),
                    //    tr("Нет задач по критериям"));
                    ui->label->setText("Нет критериев в базе");
                    return answer;
                }
                do {
                    int propID = query.value(0).toInt();
                    QString propertyName =  query.value(1).toString();
                answer.append(qMakePair(propID, propertyName));
                } while(query.next());
                sdb.close();
            }
        }
        return answer;
}


//нажатие кнопки "изменить критерии"
void SearchDialog::on_pushButton_clicked()
{
     TwoListSelection w;
     QList<QPair<int , QString>> listProp;
     try
     {
         listProp =  LoadProperties();
     }
     catch (DataBaseException ex)
     {
         this->close();
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
     try
     {
         this->AddToTable(PropIDvec);
     }
     catch (DataBaseException ex)
     {
         this->close();
         return;
     }
}

