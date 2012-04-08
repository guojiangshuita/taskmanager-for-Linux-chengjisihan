#ifndef NEWTASKDIALOG_H
#define NEWTASKDIALOG_H
#include <QDialog>

class QPushButton;
class QLabel;
class QLineEdit;
class QString;

class newTaskDialog : public QDialog
{
    Q_OBJECT
public:
    newTaskDialog(QWidget *parent = 0);
    QString *taskName;
    bool judgeClose;
    bool closeEvent;
private slots:
    //contain the slots here
    void newTasksClicked();
    void cancelClicked();
    void enewTasksButton(const QString &text);
private:
    QPushButton *newTasksButton;
    QPushButton *cancelButton;
    QLabel *taskLabel;
    QLineEdit *newTaskEdit;
    bool findTask();
    void alertDialog();
};

#endif // NEWTASKDIALOG_H
