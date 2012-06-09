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
#include <QModelIndex>
#include <QTextStream>
#include <QPen>
#include <QFileDialog>

using namespace std;

measureMaker::measureMaker(QWidget *parent) :
    QDialog(parent)
{
    savingFlags = false;
    internCpu1 = 0;
    internCpu2 = 0;
    internTotal1 = 0;
    internTotal2 = 0;
    initTableView();
    initPlotCurve();

    initTimerMaker();
}




void measureMaker::initTimerMaker()
{
    this->setGeometry(0, 0, 800, 600);
    QHBoxLayout *sliderLayout = new QHBoxLayout;
    QHBoxLayout *qwtPlotLayout = new QHBoxLayout;
    QHBoxLayout *controlButtonLayout1 = new QHBoxLayout;
    QHBoxLayout *controlButtonLayout2 = new QHBoxLayout;
    QVBoxLayout *controlButtonLayout = new QVBoxLayout;
    QVBoxLayout *tableViewLayout = new QVBoxLayout;
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
    savePushButton->setEnabled(false);
    timeGet = new QPushButton("Choose and Start");
    openPushButton = new QPushButton("Open");
    returnPushButton = new QPushButton("Return");
    refreshPushButton = new QPushButton("Refresh");
    stopTimeGet = new QPushButton("Stop");
    currentTask = new QLabel("current Tasks:");
    stopTimeGet->setEnabled(false);
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
    tableViewLayout->addWidget(taskTable);
    tableViewLayout->addWidget(currentTask);
    qwtPlotLayout->addLayout(tableViewLayout);
    mainLayout->addLayout(qwtPlotLayout);
    mainLayout->addLayout(controlButtonLayout);
    setLayout(mainLayout);
}



void measureMaker::restartDrawing()
{
    timeGet->setEnabled(false);
    stopTimeGet->setEnabled(true);
    savePushButton->setEnabled(false);
    timeSlider->setEnabled(false);
    timeSpinBox->setEnabled(false);
    drawingCpus.clear();
    drawingRams.clear();
    drawingTotals.clear();
    timesUsed.clear();
    timerGoing = 0;
    savingFlags = true;
    m_nTimerId = startTimer(timeSlider->value());
}

void measureMaker::stopDrawing()
{
    timeGet->setEnabled(true);
    stopTimeGet->setEnabled(false);
    if(savingFlags == true)
        savePushButton->setEnabled(true);
    savingFlags = false;
    timeSlider->setEnabled(true);
    timeSlider->setEnabled(true);
    if (m_nTimerId != 0 )
        killTimer(m_nTimerId);
}



void measureMaker::initPlotCurve()
{
    conditionPlot = new QwtPlot;
    conditionPlot->setTitle("Source Condition");
    //the column of qwt
    conditionPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    conditionPlot->setAxisTitle(QwtPlot::xBottom, "times-->(s)");
    conditionPlot->setAxisScale(QwtPlot::xBottom, 0.0, 100.0);
    conditionPlot->setAxisTitle(QwtPlot::yLeft, "uses-->(%)");
    conditionPlot->setAxisScale(QwtPlot::yLeft, 0.0, 100.0);

    cpuCurve = new QwtPlotCurve("CPU");
    ramCurve = new QwtPlotCurve("RAM");
    totalCurve = new QwtPlotCurve("TotalRamCurve");
    cpuCurve->attach(conditionPlot);
    ramCurve->attach(conditionPlot);
    totalCurve->attach(conditionPlot);
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
        proPriList.append(proPri);
        proMemList1.append(proMem);
        proNameList1.append(proName);
        proStateList.append(proState);
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
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,"  NAME  ");
    model->setHeaderData(1,Qt::Horizontal,"  PID  ");
    model->setHeaderData(2,Qt::Horizontal,"  RAM  ");
    headerView = taskTable->horizontalHeader();
    connect(headerView, SIGNAL(sectionClicked(int)),taskTable,SLOT(sortByColumn(int)));
    loadTasks();
    taskTable->setModel(model);
    taskTable->resizeColumnsToContents();
}




void measureMaker::timerEvent(QTimerEvent *event)
{

    QModelIndex cur = taskTable->currentIndex();
    if(!(cur.row()>=0))
    {
        return;
    }
    else
    {
        //Good!!!!!! It is too better!!
        QString value = model->item(cur.row(),1)->text();
        int cpus[10] = {0};
        QFile meminfo("/proc/meminfo");
        QString tempStr;
        QTextStream memStream(&meminfo);
        int ramTotal = 0;
        int ramFree = 0;
        int index = 2;
        if(!meminfo.open(QFile::ReadOnly|QFile::Text))
        {
            QMessageBox::warning(this, tr("warning"), tr(" meminfo open failure！"),QMessageBox::Yes);
        }
        else
        {
            QString s = memStream.readLine();
            if(s.startsWith("MemTotal:"))
            {
                index--;
                s.replace("MemTotal:","");
                s.replace(" ","");
                s.replace("kB","");
                ramTotal = s.toInt();
            }
            while(!memStream.atEnd()&&index!=0)
            {
                s = memStream.readLine();
                if(s.startsWith("MemFree:"))
                {
                    index--;
                    s.replace("MemFree:","");
                    s.replace(" ","");
                    s.replace("kB","");
                    ramFree = s.toInt();
                    continue;
                }
            }
        }


        QFile stat("/proc/stat");
        if ( !stat.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("stat open failure！"), QMessageBox::Yes);
            return;
        }

        tempStr = stat.readLine();
        internTotal1 = internTotal2;
        internTotal2 = 0;
        for (int i = 0; i < 7; i++)
        {
            internTotal2 += tempStr.section(" ", i+2, i+2).toInt();
        }
        stat.close();
        if ( !stat.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("stat open failure！"), QMessageBox::Yes);
            return;
        }
        int cpu_num = 0;
        while(true)
        {
            tempStr = stat.readLine();
            if(tempStr.left(3)!="cpu")
                break;
            cpu_num++;
        }
        stat.close();

        stat.setFileName("/proc/"+value+"/stat");
        if ( !stat.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
            return;
        }
        tempStr = stat.readLine();
        QString proMem = tempStr.section(" ", 22, 22);
        int stime = tempStr.section(" ", 14, 14).toInt();
        int utime = tempStr.section(" ", 13, 13).toInt();
        int cs = tempStr.section(" ", 15, 15).toInt() + tempStr.section(" ", 16, 16).toInt();

        internCpu1 = internCpu2;
        internCpu2 = stime+utime+cs;
        drawingCpus.append(cpu_num*abs(100*(internCpu2 - internCpu1)/(internTotal2-internTotal1)));
        cout << ramTotal << endl;
        drawingRams.append(proMem.toDouble()/10/ramTotal);
        drawingTotals.append((ramTotal-ramFree)*100/ramTotal);
        stat.close();
        timesUsed.append(timeSlider->value() * timerGoing/1000);
        timerGoing++;
        internproName = tempStr.mid(tempStr.indexOf("(") + 1, tempStr.indexOf(")")-tempStr.indexOf("(")-1);
        currentTask->setText("current Task: " + internproName + "; PID = " + value );
        //start drawing sth here
        cpuCurve->setSamples(timesUsed,drawingCpus);
        cpuCurve->setPen(QPen(Qt::red));
        ramCurve->setSamples(timesUsed,drawingRams);
        ramCurve->setPen(QPen(Qt::green));
        totalCurve->setSamples(timesUsed,drawingTotals);
        totalCurve->setPen(QPen(Qt::black));
        conditionPlot->replot();
    }
}
void measureMaker::openClicked()
{
    QString fileName = QFileDialog::getOpenFileName();
    QFile file(fileName);
    if ( !file.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("stat open failure！"), QMessageBox::Yes);
        return;
    }
    drawingCpus.clear();
    drawingRams.clear();
    drawingTotals.clear();
    timesUsed.clear();
    QString tempStr;
    int inFlags = 0;
    while(1)
    {
        tempStr = file.readLine();
        cout << tempStr.toStdString() << endl;
        if(tempStr=="over")
            break;
        if(inFlags == 0)
        {
            internTimes = tempStr.toDouble();
            inFlags = 1;
        }
        else if(inFlags == 1)
        {
            internproName = tempStr;
            inFlags = 2;
        }
        else if(inFlags == 2)
            inFlags = 3;
        else if(inFlags == 3&&!tempStr.startsWith("RAM"))
        {
            drawingCpus.append(tempStr.toDouble());
        }
        else if(inFlags == 3&&tempStr.startsWith("RAM"))
        {
            inFlags = 4;
        }

        else if(inFlags == 4&&!tempStr.startsWith("TOTAL"))
            drawingRams.append(tempStr.toDouble());
        else if(inFlags == 4&&tempStr.startsWith("TOTAL"))
            inFlags = 5;
        else if(inFlags == 5&&tempStr!="over")
            drawingTotals.append(tempStr.toDouble());
    }
    for(int i = 0; i < drawingRams.size(); i++)
        timesUsed.append(internTimes * i/1000);
    cout << internproName.toStdString() << endl;
    for(int i = 0; i < drawingRams.size(); i++)
    {
        cout << drawingCpus[i] << "    " << drawingRams[i] << "     " << drawingTotals[i] << "    " << timesUsed[i] << endl;
    }
    cpuCurve->setSamples(timesUsed,drawingCpus);
    cpuCurve->setPen(QPen(Qt::red));
    ramCurve->setSamples(timesUsed,drawingRams);
    ramCurve->setPen(QPen(Qt::green));
    totalCurve->setSamples(timesUsed,drawingTotals);
    totalCurve->setPen(QPen(Qt::black));
    conditionPlot->replot();
}

void measureMaker::returnToSuper()
{
    close();
}

void measureMaker::saveClicked()
{
    QString fileName = QFileDialog::getSaveFileName();
    QFile file(fileName);
    if ( !file.open(QIODevice::WriteOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("stat open failure！"), QMessageBox::Yes);
        return;
    }
    QTextStream t(&file);
    t << timeSlider->value();
    t << "\n";
    t << internproName ;
    t << "\n";
    t << "CPU";
    t << "\n";
    for(int i = 0; i < drawingCpus.size(); i++)
    {
        t << drawingCpus[i];
        t << "\n";
    }
    t << "RAM";
    t << "\n";
    for(int i =0; i < drawingRams.size(); i++)
    {
        t << drawingRams[i];
        t << "\n";
    }
    t << "TOTAL";
    t << "\n";
    for(int i =0; i < drawingTotals.size(); i++)
    {
        t << drawingTotals[i];
        t << "\n";
    }
    t << "over";
    file.close();
}

void measureMaker::RefreshClicked()
{
    loadTasks();
}

measureMaker::~measureMaker()
{
    if (m_nTimerId != 0 )
        killTimer(m_nTimerId);
}
