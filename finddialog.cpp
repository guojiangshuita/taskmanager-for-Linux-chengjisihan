#include <QtGui>
#include "finddialog.h"


finddialog::finddialog(QWidget *parent) : QDialog(parent)
{
    label = new QLabel(tr("Find &task:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    caseCheckBox = new QCheckBox(tr("Match &case"));
    backwardCheckBox = new QCheckBox(tr("Search &backward"));

    findButton = new QPushButton(tr("&Find"));
    findButton->setDefault(true);
    findButton->setEnabled(false);
    cancelButton = new QPushButton(tr("Cancel"));

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton(const QString &)));
    connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(caseCheckBox);
    leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(cancelButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
    setWindowTitle("Find");
    setFixedHeight(sizeHint().height());
}

void finddialog::findClicked()
{
    QString text = lineEdit->text();
    Qt::CaseSensitivity cs =
            caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    if(backwardCheckBox->isChecked())
        emit findPrevious(text, cs);
    else
        emit findNext(text, cs);

}

void finddialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}
