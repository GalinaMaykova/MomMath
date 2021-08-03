#ifndef TASKVIEWER_H
#define TASKVIEWER_H

#include <QDialog>

namespace Ui {
class TaskViewer;
}

class TaskViewer : public QDialog
{
    Q_OBJECT

public:
    explicit TaskViewer(QWidget *parent = nullptr);
    ~TaskViewer();
     void View(QString taskId);

private:
    Ui::TaskViewer *ui;
};

#endif // TASKVIEWER_H
