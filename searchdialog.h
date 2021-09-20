#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <qtableview.h>

#include "mytablemodel.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();


    void SetTable(MyTableModel* myModel);

private slots:

    void on_pushButton_clicked();


    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::SearchDialog *ui;
    QTableView *view;
};

#endif // SEARCHDIALOG_H
