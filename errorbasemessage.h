#ifndef ERRORBASEMESSAGE_H
#define ERRORBASEMESSAGE_H

#include <QDialog>

namespace Ui {
class ErrorBaseMessage;
}

class ErrorBaseMessage : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorBaseMessage(QWidget *parent = nullptr);
    ~ErrorBaseMessage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ErrorBaseMessage *ui;
};

#endif // ERRORBASEMESSAGE_H
