#ifndef finddialog_H
#define finddialog_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QString;

class finddialog : public QDialog
{
    Q_OBJECT
public:
    finddialog(QWidget *parent = 0);
signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);
private slots:
    void findClicked();
    void enableFindButton(const QString &text);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardCheckBox;
    QPushButton *findButton;
    QPushButton *cancelButton;
};

#endif // finddialog_H
