#include "newtaskdialog.h"
#include <QtGui>
using namespace std;

newTaskDialog::newTaskDialog(QWidget *parent) : QDialog(parent)
{
    taskLabel = new QLabel(tr("&Task:"));
    newTaskEdit = new QLineEdit;
    taskLabel->setBuddy(newTaskEdit);
    judgeClose = false;
    closeEvent = false;
    newTasksButton = new QPushButton(tr("&OK"));
    cancelButton = new QPushButton(tr("&Cancel"));
    newTasksButton->setDefault(true);
    newTasksButton->setEnabled(false);

    connect(newTaskEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enewTasksButton(const QString &)));
    connect(newTasksButton, SIGNAL(clicked()), this, SLOT(newTasksClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(taskLabel);
    topLayout->addWidget(newTaskEdit);
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(newTasksButton);
    bottomLayout->addWidget(cancelButton);
    bottomLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);
    setWindowTitle("Create a new task");
    setFixedHeight(sizeHint().height());
}

void newTaskDialog::newTasksClicked()
{
    judgeClose = true;
    if(findTask())
    {
        taskName = new QString(newTaskEdit->text());
        close();
    }
    else
    {
        alertDialog();
    }

}

void newTaskDialog::alertDialog()
{

}
void newTaskDialog::cancelClicked()
{
    closeEvent = true;
    close();

}

bool newTaskDialog::findTask()
{

}

void newTaskDialog::enewTasksButton(const QString &text)
{
    newTasksButton->setEnabled(!text.isEmpty());
}
