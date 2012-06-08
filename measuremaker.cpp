#include "measuremaker.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_series_data.h>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>
#include <QTableView>
#include <QDir>
#include <QVector>
#include <QFile>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <qwt_legend.h>
#include <iostream>

using namespace std;

measureMaker::measureMaker(QWidget *parent) :
    QDialog(parent)
{

    initTableView();
    initPlotCurve();

    initTimerMaker();
}




void measureMaker::initTimerMaker()
{
    this->setGeometry(0, 0, 700, 400);
    QHBoxLayout *sliderLayout = new QHBoxLayout;
    QHBoxLayout *qwtPlotLayout = new QHBoxLayout;
    QHBoxLayout *controlButtonLayout1 = new QHBoxLayout;
    QHBoxLayout *controlButtonLayout2 = new QHBoxLayout;
    QVBoxLayout *controlButtonLayout = new QVBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setWindowTitle(tr("MeasureMaker"));
    timeSlider = new QSlider;
    timeSlider->setRange(500,1500);
    timeSlider->setOrientation(Qt::Horizontal);
    timeSpinBox = new QSpinBox;
    timeSpinBox->setRange(500,1500);
    connect(timeSlider, SIGNAL(valueChanged(int)), timeSpinBox, SLOT(setValue(int)));
    connect(timeSpinBox, SIGNAL(valueChanged(int)), timeSlider, SLOT(setValue(int)));
    labelTime = new QLabel("Intermission(ms):");
    savePushButton = new QPushButton("Save");
    timeGet = new QPushButton("Choose and Start");
    openPushButton = new QPushButton("Open");
    returnPushButton = new QPushButton("Return");
    refreshPushButton = new QPushButton("Refresh");
    stopTimeGet = new QPushButton("Stop");
    connect(timeGet, SIGNAL(clicked()), this, SLOT(restartDrawing()));
    connect(stopTimeGet, SIGNAL(clicked()), this, SLOT(stopDrawing()));
    connect(savePushButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    connect(openPushButton, SIGNAL(clicked()), this, SLOT(openClicked()));
    connect(returnPushButton, SIGNAL(clicked()), this, SLOT(returnToSuper()));
    connect(refreshPushButton, SIGNAL(clicked()), this, SLOT(RefreshClicked()));
    sliderLayout->addWidget(timeSlider);
    sliderLayout->addWidget(timeSpinBox);
    controlButtonLayout1->addWidget(savePushButton);
    controlButtonLayout1->addWidget(returnPushButton);
    controlButtonLayout1->addWidget(timeGet);
    controlButtonLayout2->addWidget(openPushButton);
    controlButtonLayout2->addWidget(refreshPushButton);
    controlButtonLayout2->addWidget(stopTimeGet);
    controlButtonLayout->addLayout(sliderLayout);
    controlButtonLayout->addLayout(controlButtonLayout1);
    controlButtonLayout->addLayout(controlButtonLayout2);
    qwtPlotLayout->addWidget(conditionPlot);
    qwtPlotLayout->addWidget(taskTable);
    mainLayout->addLayout(qwtPlotLayout);
    mainLayout->addLayout(controlButtonLayout);
    setLayout(mainLayout);
}



void measureMaker::restartDrawing()
{

}

void measureMaker::stopDrawing()
{
}

void measureMaker::timerStart()
{

}


void measureMaker::initPlotCurve()
{
    conditionPlot = new QwtPlot;
    conditionPlot->setTitle("Source Condition");
    //the column of qwt
    conditionPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    conditionPlot->setAxisTitle(QwtPlot::xBottom, "x -->");
    conditionPlot->setAxisScale(QwtPlot::xBottom, 0.0, 100.0);
    conditionPlot->setAxisTitle(QwtPlot::yLeft, "y -->");
    conditionPlot->setAxisScale(QwtPlot::yLeft, 0.0, 100.0);

    cpuCurve = new QwtPlotCurve("CPU");
    ramCurve = new QwtPlotCurve("RAM");
}

void measureMaker::loadTasks()
{
    int cpus[10] = {0};
    QString tempStr;
    QFile stat("/proc/stat");
    if ( !stat.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("stat open failure！"), QMessageBox::Yes);
        return;
    }

    tempStr = stat.readLine();
    int cpu_num = 0;
    while(true)
    {
        tempStr = stat.readLine();
        if(tempStr.left(3)!="cpu")
            break;
        for (int i = 0; i < 7; i++)
        {
            cpus[cpu_num] += tempStr.section(" ", i+2, i+2).toInt();
            //count the total
        }
        cpu_num++;
    }
    stat.close();
    QDir tasks("/proc");
    QStringList tasksList = tasks.entryList();
    QString taskString = tasksList.join("\n");
    QString id_of_pro;
    bool ok;
    int find_start = 3;
    int nProPid;
    int totalProNum = 0;
    QString proName;
    QString proState;
    QString proPri;
    QString proMem;
    QVector <QString> proNameList1;
    QVector <QString> pidList1;
    QVector <QString> proPriList;
    QVector <QString> proMemList1;
    QVector <QString> proStateList;
    QVector <int> proCPUList1;
    int a, b;
    while(1)
    {

        a = taskString.indexOf("\n", find_start);
        b = taskString.indexOf("\n", a+1);
        find_start = b;
        //a, b-a-1, b
        id_of_pro = taskString.mid(a+1, b-a-1);
        pidList1.append(id_of_pro);
        totalProNum++;
        //make sure whether is the pid
        nProPid = id_of_pro.toInt(&ok, 10);
        //pid stops, break!
        if(!ok)
            break;
        stat.setFileName("/proc/" + id_of_pro + "/stat");
        if ( !stat.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
            return;
        }
        tempStr = stat.readLine();
        if (tempStr.length() == 0)
            break;
        a = tempStr.indexOf("(");
        b = tempStr.indexOf(")");
        proName = tempStr.mid(a+1, b-a-1);
        //delete the blanks
        proName.trimmed();
        proState = tempStr.section(" ", 2, 2);
        proPri = tempStr.section(" ", 17, 17);
        proMem = tempStr.section(" ", 22, 22);
        int stime = tempStr.section(" ", 14, 14).toInt();
        int utime = tempStr.section(" ", 13, 13).toInt();
        int cs = tempStr.section(" ", 15, 15).toInt() + tempStr.section(" ", 16, 16).toInt();
        proPriList.append(proPri);
        proMemList1.append(proMem);
        proNameList1.append(proName);
        proStateList.append(proState);
        proCPUList1.append(100*(stime+utime+cs)/cpus[tempStr.section(" ", 40, 40).toInt()]);
        stat.close();
    }
    model->setRowCount(totalProNum);
    for(int i = 0; i < totalProNum-1; i++)
    {
        //input sth can get the task name and
        //the condition of RAM and CPU
        QStandardItem *itemName = new QStandardItem(proNameList1[i]);
        model->setItem(i, 0, itemName);
        QStandardItem *itemPid = new QStandardItem(pidList1[i]);
        model->setItem(i, 1, itemPid);
        QStandardItem *itemRam = new QStandardItem(proMemList1[i]);
        model->setItem(i, 2, itemRam);
        QString ss = QString::number(proCPUList1[i],10);
        QStandardItem *itemCPU = new QStandardItem(ss);
        model->setItem(i, 3, itemCPU);
    }
}

void measureMaker::initTableView()
{

    taskTable = new QTableView(this);
    taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //set the none editing mode
    taskTable->setSelectionBehavior( QAbstractItemView::SelectRows);
    taskTable->setSelectionMode( QAbstractItemView::SingleSelection);
    //choose only one line at one time
    model = new QStandardItemModel(taskTable);
    model->clear();
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,"  NAME  ");
    model->setHeaderData(1,Qt::Horizontal,"  PID  ");
    model->setHeaderData(2,Qt::Horizontal,"  RAM  ");
    model->setHeaderData(3,Qt::Horizontal,"  CPU  ");
    headerView = taskTable->horizontalHeader();
    connect(headerView, SIGNAL(sectionClicked(int)),taskTable,SLOT(sortByColumn(int)));
    loadTasks();
    taskTable->setModel(model);

}




void measureMaker::timerEvent(QTimerEvent *event)
{

}
void measureMaker::openClicked()
{

}

void measureMaker::returnToSuper()
{
    close();
}

void measureMaker::saveClicked()
{

}

void measureMaker::RefreshClicked()
{

}
