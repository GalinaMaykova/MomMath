#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

    void initTable();
    void AddToTable(QVector<int> list);
    QList<QPair<int , QString>> LoadProperties();

private slots:
    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();


private:
    Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
