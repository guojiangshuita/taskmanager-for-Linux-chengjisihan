#include <QDialog>
#include <iostream>
#include <QLabel>
#include <QPushButton>
//#include <QScrollBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QProgressBar>
#include <QGridLayout>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QMenuBar>
#include <QStatusBar>
#include <QSettings>
#include <QFileDialog>
#include <QHeaderView>
#include <qtextstream.h>
#include <QFile>
#include <QDir>
#include <QVector>
#include <sstream>
#include "mainwindow.h"
#include "finddialog.h"
#include "measuremaker.h"
#include "superkiller.h"
#include "spreadsheet.h"
#include "newtaskdialog.h"







using namespace std;

mainwindow::mainwindow()
{
    spreadsheets = new spreadsheet;
    this->setGeometry(0, 0, 700, 600);
    this->openedtaskDialog = 0;
    this->finddialoging = 0;
    saveConditionFlags='N';
//    initInfo();
    a0=0;
    a1=0;
    b0=0;
    b1=0;
    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
    createWidgets();
    readSettings();
    setCentralWidget(centralDialog);
}


void mainwindow::createActions()
{
    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon("/home/claude/Icon/4308/3267"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a report"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon("/home/claude/Icon/4308/3261"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save a report"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    finddialogAction = new QAction(tr("&Find"), this);
    finddialogAction->setIcon(QIcon("/home/claude/Icon/4308/3263"));
    finddialogAction->setStatusTip(tr("Find the task"));
    finddialogAction->setShortcut(tr("CTRL+F"));
    connect(finddialogAction, SIGNAL(triggered()), this, SLOT(finddialogs()));
    measureMakerAction = new QAction(tr("&Measure Maker"), this);
    measureMakerAction->setShortcut(tr("CTRL+M"));
    measureMakerAction->setIcon(QIcon("/home/claude/Icon/4308/3257"));
    measureMakerAction->setStatusTip(tr("Activite the measuremaker"));
    connect(measureMakerAction,SIGNAL(triggered()),this,SLOT(measureMaker_activate()));
    superKillerAction = new QAction(tr("Super &Killer"), this);
    superKillerAction->setShortcut(tr("CTRL+K"));
    superKillerAction->setStatusTip(tr("Activate the superkiller"));
    superKillerAction->setIcon(QIcon("/home/claude/Icon/4308/3264"));
    connect(superKillerAction, SIGNAL(triggered()), this, SLOT(superKiller_activate()));
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon("/home/claude/Icon/4308/3256"));
    newAction->setShortcut(tr("CTRL+N"));
    newAction->setStatusTip(tr("Create a new task"));
    connect(newAction, SIGNAL(triggered()),this, SLOT(newTask()));
    for(int i = 0; i < MaxRecentFiles; i++)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setIcon(QIcon("/home/claude/Icon/4308/3258"));
    aboutAction->setShortcut(tr("CTRL+A"));
    aboutAction->setStatusTip(tr("See the information"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    aboutTaskMgrAction = new QAction(tr("About &TaskMgr"), this);
    aboutTaskMgrAction->setShortcut(tr("CTRL+G"));
    aboutTaskMgrAction->setIcon(QIcon("/home/claude/Icon/4308/3259"));
    aboutTaskMgrAction->setStatusTip(tr("See the usage"));
    connect(aboutTaskMgrAction, SIGNAL(triggered()), this, SLOT(about_TaskMgr()));
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setIcon(QIcon(":/home/claude/下载/726/exit.ico"));
    exitAction->setStatusTip(tr("Exit"));
    exitAction->setShortcut(tr("CTRL+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    optionAction = new QAction(tr("Sett&ings"), this);
    optionAction->setIcon(QIcon("/home/claude/Icon/4308/3260"));
    optionAction->setStatusTip(tr("Set the option"));
    optionAction->setShortcut(tr("CTRL+I"));
    connect(optionAction, SIGNAL(triggered()), this, SLOT(optionDisplay()));
}


void mainwindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(finddialogAction);
    separatorAction = fileMenu->addSeparator();
    for(int i = 0; i < MaxRecentFiles; i++)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(measureMakerAction);
    toolsMenu->addAction(superKillerAction);
    optionsMenu = menuBar()->addMenu(tr("&Option"));
    optionsMenu->addAction(optionAction);
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutTaskMgrAction);
}

void mainwindow::createWidgets()
{

    centralDialog = new QDialog;
    mainLayout = new QGridLayout;
    //this->mainLayout->setGeometry(QRect(50, 0, 700, 600));
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    cpuCondition = new QLabel(tr("CPU"));
    cpuCondition->setAlignment(Qt::AlignLeft);
    ramCondition = new QLabel(tr("RAM"));
    ramCondition->setAlignment(Qt::AlignLeft);
    cpuPersentage = new QProgressBar;
    cpuPersentage->setMaximum(100);
    cpuPersentage->setMinimum(0);
    cpuPersentage->setTextVisible(true);
    cpuPersentage->setTextDirection(QProgressBar::BottomToTop);
    cpuPersentage->text();
    cpuPersentage->setOrientation(Qt::Vertical);
    ramPersentage = new QProgressBar;
    ramPersentage->setMaximum(100);
    ramPersentage->setMinimum(0);
    ramPersentage->setTextVisible(true);
    ramPersentage->setTextDirection(QProgressBar::BottomToTop);
    ramPersentage->setOrientation(Qt::Vertical);
    //set value here
    kill = new QPushButton(tr("Kill a task"));
    kill->setEnabled(false);
    refreshTask = new QPushButton(tr("Refresh"));
    refreshTask->setEnabled(false);
    mainLayout->addWidget(cpuPersentage,0, 0, 10, 1);
    mainLayout->addWidget(ramPersentage, 0, 1, 10, 1);
    mainLayout->addWidget(cpuCondition,11, 0, 1, 1);
    mainLayout->addWidget(ramCondition,21, 1, 1, 1);
    mainLayout->addWidget(kill, 22, 0, 1, 2);
    mainLayout->addWidget(refreshTask, 23, 0, 1, 2);
    taskTable = new QTableView(this);
    taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //set the none editing mode
    taskTable->setSelectionBehavior( QAbstractItemView::SelectRows);
    taskTable->setSelectionMode( QAbstractItemView::SingleSelection);
    //choose only one line at one time
    setTableView();
    mainLayout->addWidget(taskTable, 0, 2, 24, 5);
    centralDialog->setLayout(mainLayout);

}

void mainwindow::setTableView()
{
    model = new QStandardItemModel(taskTable);
    model->clear();
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,"   PID   ");
    model->setHeaderData(1,Qt::Horizontal,"   RAM   ");
    model->setHeaderData(2,Qt::Horizontal,"   CPU   ");
    model->setHeaderData(3,Qt::Horizontal,"   NAME   ");
    headerView = taskTable->horizontalHeader();
    connect(headerView, SIGNAL(sectionClicked(int)),taskTable,SLOT(sortByColumn(int)));
    loadTasks();
    taskTable->setModel(model);
}

void mainwindow::loadTasks()
{
    //about 4 count need to be get
    int index = 4;
    QFile meminfo("/proc/meminfo");
    QFile stat("/proc/stat");
    QTextStream memStream(&meminfo);
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
            else if(s.startsWith("SwapTotal:"))
            {
                index--;
                s.replace("SwapTotal:","");
                s.replace(" ","");
                s.replace("kB","");
                swapTotal = s.toInt();
                continue;
            }
            else if(s.startsWith("SwapFree:"))
            {
                index--;
                s.replace("SwapFree:","");
                s.replace(" ","");
                s.replace("kB","");
                swapFree = s.toInt();
                continue;
            }
        }
    }
    //close the meminfo
    ramUsed = ramTotal-ramFree;
    swapUsed = swapTotal-swapFree;
    meminfo.close();

    //about the condition of cpu

    int tt = 2;
    int cpuInfo[2][7];
    int cpuTotal[2][2];
    int cpus[10];
    QString tempStr;
    while (tt)
    {
        stat.setFileName("/proc/stat");
        if ( !stat.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr(" stat open failure！"),QMessageBox::Yes);
            return ;
        }
        tempStr = stat.readLine();
        for (int i = 0; i < 7; i++)
        {
            cpuInfo[2-tt][i] = tempStr.section(" ", i+1, i+1).toInt();
            //count the total
            cpuTotal[1][2-tt] += cpuInfo[2-tt][i];
            if (i == 3)
            {
                //about the idel
                cpuTotal[0][2-tt] += cpuInfo[2-tt][i];
            }
        }
        tt--;
        stat.close();
    }
//    total_0=USER[0]+NICE[0]+SYSTEM[0]+IDLE[0]+IOWAIT[0]+IRQ[0]+SOFTIRQ[0]
//    total_1=USER[1]+NICE[1]+SYSTEM[1]+IDLE[1]+IOWAIT[1]+IRQ[1]+SOFTIRQ[1]
//    cpu usage=(IDLE[0]-IDLE[1]) / (total_0-total_1) * 100

    int a = cpuTotal[0][1] - cpuTotal[0][0];
    int b = cpuTotal[1][1] - cpuTotal[1][0];
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;


    if ( !stat.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("stat open failure！"), QMessageBox::Yes);
        return;
    }

    tempStr = stat.readLine();
    cout << tempStr.toStdString();
    int cpu_num = 0;
    while(true)
    {
        tempStr = stat.readLine();
        if(tempStr.left(3)!="cpu")
            break;
        for (int i = 0; i < 7; i++)
        {
            cpus[cpu_num] += tempStr.section(" ", i+1, i+1).toInt();
            //count the total

        }
        cpu_num++;
    }
    stat.close();

    //clear the list of the tasks

    //model->clear();
    QDir tasks("/proc");
    QStringList tasksList = tasks.entryList();
    QString taskString = tasksList.join("\n");
    QString id_of_pro;
    bool ok;
    int find_start = 3;
    int nProPid;
    int number_of_sleep = 0, number_of_run = 0, number_of_zombie = 0;
    int totalProNum = 0;
    QString proName;
    QString proState;
    QString proPri;
    QString proMem;
    QVector <QString> proNameList;
    QVector <QString> pidList;
    QVector <QString> proPriList;
    QVector <QString> proMemList;
    QVector <int> proCPUList;
    while(1)
    {

        a = taskString.indexOf("\n", find_start);
        b = taskString.indexOf("\n", a+1);
        find_start = b;
        //a, b-a-1, b
        id_of_pro = taskString.mid(a+1, b-a-1);
        pidList.append(id_of_pro);
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
        proMemList.append(proMem);
        proNameList.append(proName);
        proCPUList.append(100*(stime+utime+cs)/cpus[tempStr.section(" ", 40, 40).toInt()]);

        switch ( proState.at(0).toLatin1() )
        {
            case 'S':   number_of_sleep++; break; //Sleep
            case 'R':   number_of_run++; break; //Running
            case 'Z':   number_of_zombie++; break; //Zombie
            default :   break;
        }
    }


    model->setRowCount(totalProNum);
    for(int i = 0; i < totalProNum-1; i++)
    {
        //input sth can get the task name and
        //the condition of RAM and CPU
        QStandardItem *itemPid = new QStandardItem(pidList[i]);
        model->setItem(i, 0, itemPid);
        QStandardItem *itemRam = new QStandardItem(proMemList[i]);
        model->setItem(i, 1, itemRam);
        QString ss = QString::number(proCPUList[i],10);
        QStandardItem *itemCPU = new QStandardItem(ss);
        model->setItem(i, 2, itemCPU);
        QStandardItem *itemName = new QStandardItem(proNameList[i]);
        model->setItem(i, 3, itemName);
    }


}


void mainwindow::createStatusBar()
{
    locationLabel = new QLabel;
    //locationLabel->setAlignment(Qt::AlignCenter);
    locationLabel->setIndent(3);
    statusBar()->addWidget(locationLabel);
}

void mainwindow::createToolBar()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->addAction(finddialogAction);
    toolToolBar = addToolBar(tr("&Tools"));
    toolToolBar->addAction(measureMakerAction);
    toolToolBar->addAction(superKillerAction);
    optionToolBar = addToolBar(tr("&Option"));
    optionToolBar->addAction(optionAction);
    helpToolBar = addToolBar(tr("&Help"));
    helpToolBar->addAction(aboutAction);
    helpToolBar->addAction(aboutTaskMgrAction);
}

void mainwindow::createContextMenu()
{

}

void mainwindow::newTask()
{
    if(!openedtaskDialog)
    {
        openedtaskDialog = new newTaskDialog(this);
    }
    openedtaskDialog->show();
    openedtaskDialog->raise();
    openedtaskDialog->activateWindow();
}

void mainwindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open record"), ".", (tr("Record files (*.rd)")));
    if(!fileName.isEmpty())
        loadFile(fileName);
}

bool mainwindow::loadFile(const QString &fileName)
{
    if(!readFile(fileName))
    {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    saveConditionFlags='L';
    return true;
}



//set files name etc.

void mainwindow::save()
{

}

bool mainwindow::readFile(const QString &fileName)
{

}

void mainwindow::finddialogs()
{
    if(!finddialoging)
    {
        finddialoging=new finddialog(this);
        connect(finddialoging, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)), this, SLOT(findNext(const QString &,Qt::CaseSensitivity)));
        connect(finddialoging, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)), this, SLOT(findPrevious(const QString &,Qt::CaseSensitivity)));
    }
    finddialoging->show();
    finddialoging->raise();
    finddialoging->activateWindow();
}

void mainwindow::setCurrentFile(const QString &fileName)
{
    curFile=fileName;

}

void mainwindow::sorts()
{

}



void mainwindow::measureMaker_activate()
{
    mMSet=new measureMaker(this);
    if(saveConditionFlags=='L')
    {
        mMSet->setInputFiles(curFile);
        mMSet->show();
    }

}

void mainwindow::superKiller_activate()
{

}

void mainwindow::openRecentFile()
{

}

void mainwindow::about_TaskMgr()
{

}

void mainwindow::optionDisplay()
{

}



void mainwindow::about()
{
    QMessageBox::about(this, tr("About TaskMGR"),
                             tr("<h2>TaskMGR 1.0</h2>"
                                "<p>Copyright &copy; 2011 Software Inc."
                                "<p>TaskMGR is a small application that"
                                "<p>can easily arrange the using of RAM condition"));
}

void mainwindow::readSettings()
{

}

void mainwindow::findNext(const QString &text, Qt::CaseSensitivity)
{

}

void mainwindow::findPrevious(const QString &text, Qt::CaseSensitivity)
{

}

void mainwindow::writeSettings()
{
    QSettings settings("Software Inc.", "TaskMgr");
    //settings.setValue();
}
