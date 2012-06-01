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
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,"   PID   ");
    model->setHeaderData(1,Qt::Horizontal,"   RAM   ");
    model->setHeaderData(2,Qt::Horizontal,"   CPU   ");
    model->setHeaderData(3,Qt::Horizontal,"   PATH   ");
    headerView = taskTable->horizontalHeader();
    connect(headerView, SIGNAL(sectionClicked(int)),taskTable,SLOT(sortByColumn(int)));
    loadTasks();
    taskTable->setModel(model);
}

void mainwindow::loadTasks()
{
<<<<<<< HEAD
    int a0 = 0, a1 = 0, b0 = 0, b1 = 0;
    //about 4 count need to be get
    int index = 4;
    QFile meminfo("/proc/meminfo");
    QFile stat("/proc/stat");
    QTextStream memStream(&meminfo);
    if(!meminfo.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this, tr("warning"), tr(" meminfo文件打不开！"),QMessageBox::Yes);
=======
    //about 4 count need to be get
    int index = 4;
    QFile meminfo("/proc/meminfo");
    QFile maps("/proc/self/maps");
    QFile statm("/proc/self/statm");
    QTextStream memStream(&meminfo);
    QTextStream mapsStream(&maps);
    QTextStream statmStream(&statm);
    if(!meminfo.open(QFile::ReadOnly|QFile::Text))
    {
        std::cout<< "Open failure" << endl;
        exit(-1);
>>>>>>> d9853fec76e4176d3c83b2f34e4ea6d40d6a9c8a
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
<<<<<<< HEAD
=======
            std::cout<<ramTotal<< endl;
>>>>>>> d9853fec76e4176d3c83b2f34e4ea6d40d6a9c8a
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
<<<<<<< HEAD
=======
                std::cout<<ramFree<<endl;
>>>>>>> d9853fec76e4176d3c83b2f34e4ea6d40d6a9c8a
                continue;
            }
            else if(s.startsWith("SwapTotal:"))
            {
                index--;
                s.replace("SwapTotal:","");
                s.replace(" ","");
                s.replace("kB","");
                swapTotal = s.toInt();
<<<<<<< HEAD
=======
                std::cout<<swapTotal<<endl;
>>>>>>> d9853fec76e4176d3c83b2f34e4ea6d40d6a9c8a
                continue;
            }
            else if(s.startsWith("SwapFree:"))
            {
                index--;
                s.replace("SwapFree:","");
                s.replace(" ","");
                s.replace("kB","");
                swapFree = s.toInt();
<<<<<<< HEAD
=======
                std::cout<<swapFree<<endl;
>>>>>>> d9853fec76e4176d3c83b2f34e4ea6d40d6a9c8a
                continue;
            }
        }
    }
<<<<<<< HEAD
    //close the meminfo
    ramUsed = ramTotal-ramFree;
    swapUsed = swapTotal-swapFree;
    meminfo.close();

    //about the condition of cpu

    int tt = 2;
    int cpuInfo[2][7];
    int cpuTotal[2][2];
    QString tempStr;
    while (tt)
    {
        stat.setFileName("/proc/stat");
        if ( !stat.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr(" stat文件打不开！"),QMessageBox::Yes);
            return ;
        }
        tempStr = stat.readLine();
        for (int i = 0; i < 7; i++)
        {
            cpuInfo[2-tt][i] = tempStr.section(" ", i+1, i+1).toInt();
            cpuTotal[1][2-tt] += cpuInfo[2-tt][i];
            if (i == 3)
            {
                cpuTotal[0][2-tt] += cpuInfo[2-tt][i];
            }
        }
        tt--;
        stat.close();
    }

    int a = cpuTotal[0][1] - cpuTotal[0][0];
    int b = cpuTotal[1][1] - cpuTotal[1][0];
    if (a < 0)
    {
        a = -a;
    }
    if (b < 0)
    {
        b = -b;
    }
    cout << a << "      " << b << endl;

    if ( !stat.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("stat文件打不开！"), QMessageBox::Yes);
        return;
    }

    tempStr = stat.readLine();
    a0 = a1;
    b0 = b1;
    a1 = b1 = 0;
    int gg;
    for (int i = 0; i < 7; i++)
    {
        b1 += tempStr.section(" ", i+2, i+2).toInt();
        gg = b1;
        if (i == 3)
        {
            a1 += tempStr.section(" ", i+2, i+2).toInt();
        }
    }
    int m, n;
    m = a1 - a0;
    n = b1 - b0;
    if (m < 0)
    {
        m = -m;
    }
    if (n < 0)
    {
        n = -n;
    }
    cout << (n-m)*100/n << endl;
    stat.close(); //关闭stat文件


=======
    ramUsed = ramTotal-ramFree;
    swapUsed = swapTotal-swapFree;

    if(!maps.open((QFile::ReadOnly|QFile::Text)))
    {

    }
>>>>>>> d9853fec76e4176d3c83b2f34e4ea6d40d6a9c8a
    model->setRowCount(countTheTask());
    for(int i = 0; i < countTheTask(); i++)
    {
        //input sth can get the task name and
        //the condition of RAM and CPU
    }
}

int mainwindow::countTheTask()
{
    return 20;
    //this is the test mode and you will add something
    //useful here
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
