#include <QtGui>
#include "mainwindow.h"
#include "spreadsheet.h"
#include "newtaskdialog.h"
#include <QDialog>
#include <iostream>


using namespace std;

mainwindow::mainwindow()
{
    spreadsheets = new spreadsheet;
    this->setGeometry(0, 0, 800, 600);
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
    connect(optionAction, SLOT(trigger()), this, SLOT(optionDisplay()));
}

void mainwindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
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
    kill = new QPushButton(tr("Kill1"));
    kill->setEnabled(false);
    kills = new QPushButton(tr("Kill2"));
    kills->setEnabled(false);
    mainLayout->addWidget(cpuPersentage,0, 0, 20, 1);
    mainLayout->addWidget(ramPersentage, 0, 1, 20, 1);
    mainLayout->addWidget(cpuCondition,21, 0, 1, 1);
    mainLayout->addWidget(ramCondition,21, 1, 1, 1);
    mainLayout->addWidget(kill, 22, 0, 1, 2);
    mainLayout->addWidget(kills, 23, 0, 1, 2);
    taskList = new QListView;
    taskList->setFlow(QListView::TopToBottom);
    taskList->setModelColumn(4);
    centralDialog->setLayout(mainLayout);
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
    toolToolBar = addToolBar(tr("&Tools"));
    toolToolBar->addAction(measureMakerAction);
    toolToolBar->addAction(superKillerAction);
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
    this->setEnabled(false);
    openedtaskDialog->show();
    openedtaskDialog->setEnabled(true);
    openedtaskDialog->raise();
    openedtaskDialog->activateWindow();
    this->setEnabled(true);
    if(openedtaskDialog->closeEvent)
        this->setEnabled(true);
}

void mainwindow::open()
{

}

void mainwindow::save()
{

}

void mainwindow::sorts()
{

}

void mainwindow::about()
{

}

void mainwindow::measureMaker_activate()
{

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

void mainwindow::readSettings()
{

}

