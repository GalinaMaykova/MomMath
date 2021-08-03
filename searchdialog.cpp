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

void SearchDialog::AddToTable(QVector<int> IDsProp) // здесь нужно не лист, а id ишники принимать
{
    ui->tableWidget->setRowCount(0);
    QString myQuery;
    if (IDsProp.empty())
    {
        myQuery = "select TaskId, headline from task";
    }
    else
    {
        //TODO
        //написать сложный запрос с учетом id выбранных критериев
        // 2 и 3 - номера id
        QString temp;
        for(int i=0; i<IDsProp.size() - 1; ++i)
        {
            temp += QString::number(IDsProp[i]);
            temp += ", ";
        }
        temp +=QString::number(IDsProp[IDsProp.size() - 1]);

        myQuery = "select TaskId from reference "
                "WHERE PropertyID IN ("+ temp + ") "
                "GROUP BY TaskID "
                "HAVING count(distinct PropertyID) = " + QString::number(IDsProp.size());
//        myQuery = "select TaskId, headline from task"
//                "INNER JOIN reference ON reference.TaskID = task.TaskID"
//                "INNER JOIN properties ON properties.PropertyID = reference.PropertyID"
//                "WHERE properties.propertyNAME = для 7го AND properties.propertyNAME = сложное";

       // select a.name as Город, count(b.id) as Количество "
        //                            "from cities "
        //                            "a inner join people b on a.id = b.city_id "
        //                            "group by a.name "
        //                            "order by count(b.id) desc"





    }

    QString DBPath =  QCoreApplication::applicationDirPath();
        QString DBName = DBPath + "/MomDB.db";
        if(QFile(DBName).exists())
        {
            QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(DBName);
            if (sdb.open())
            {

//                QSqlQuery query("SELECT headline FROM task");
//                int i=0;
//                while (query.next())
//                {
//                   QString name = query.value(0).toString();
//                   if(i==0)
//                   {
//                        ui->label->setText(name);

//                    }
//                    i++;
//                }

                QSqlQuery query;
//                if (!query.exec("select a.name as Город, count(b.id) as Количество "
//                            "from cities "
//                            "a inner join people b on a.id = b.city_id "
//                            "group by a.name "
//                            "order by count(b.id) desc")) {


//                if (!query.exec("select text as Текст, headline as Заголовок "
//                                "from task ")) {
//                    ui->label_2->setText("Запрос не удался");
//                    return;
//                }

              //  if (!query.exec("select TaskId, headline "
                //                "from task ")) {

                if (!query.exec(myQuery)){
                    ui->label_2->setText("Запрос не удался");
                    return;
                }






                if (!query.first()) {
                  //  QMessageBox::warning(this, tr("Непонятно"),
                    //    tr("Нет задач по критериям"));
                    ui->label_2->setText("Нет задач по критериям");
                    return;
                }


                if (!IDsProp.empty()) // если сложный запрос
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

                } else // дальше идет если простой запрос
                {




                //заберем информацию о полях запроса
//                QSqlRecord rec = query.record();
//                //подарим таблице заголовки по указанным альясам
//                for(int i = 0; i < rec.count(); ++i) {
//                    ui->tableWidget->insertColumn(i);
//                    ui->tableWidget->setHorizontalHeaderItem(
//                                i, new QTableWidgetItem(rec.fieldName(i))); //в коде на сайте ошибка
//                }


                    //пока есть данные в запросе - берем
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
        else
        {
            ErrorBaseMessage errBase;
            errBase.setModal(true); // разрешаем окну открываться
            errBase.exec(); //выполнить
            throw DataBaseException();
        }
}

void SearchDialog::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    //TODO
    //добавить просмотр каждой задачи

   // int currRow = index.row();
//    QDataStream


    QString idcheck;
    QTableWidgetItem *item = ui->tableWidget->item(index.row(), 0);
    if (NULL != item) {
       idcheck = item->text();
    }


    TaskViewer tv;
    tv.View(idcheck);
    tv.setModal(true); // разрешаем окну открываться
    tv.exec();




}




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






void SearchDialog::on_pushButton_clicked()
{
    // QApplication a(argc, argv);
     TwoListSelection w;
   //  TwoListSelection *w = new TwoListSelection;
  //   QStringList input;
//     for (int i = 0; i < 10; i++) {
//       input << QString("item-%1").arg(i);
//     }
     //TODO
     //реализовать добавление критериев из базы данных


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


  //   return a.exec();




}

