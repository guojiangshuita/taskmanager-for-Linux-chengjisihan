/**************
  **************

  Author: 成吉丝晗
  **************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "newtaskdialog.h"

class QLabel;
class QListView;
class QPushButton;
class QScrollBar;
class QMenu;
class QToolBar;
class QAction;
class spreadsheet;
class QProgressBar;
class QGridLayout;
class QDialog;
class QwtPlot;
class QwtPlotCurve;

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow();
//protected:
//    void closeEvent(QCloseEvent *event);
private slots:
    //put some funcs here
    void open();
    void save();
    void newTask();
    void sorts();
    void about();
    void measureMaker_activate();
    void superKiller_activate();
    void openRecentFile();
    void about_TaskMgr();
    void optionDisplay();
private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void createToolBar();
    void createContextMenu();
    void createWidgets();
    void readSettings();
    QLabel *locationLabel;
    QLabel *cpuCondition;
    QLabel *ramCondition;
    QGridLayout *mainLayout;
    QListView *taskList;
    QPushButton *kill;
    QPushButton *kills;
    QProgressBar *cpuPersentage;
    QProgressBar *ramPersentage;
    spreadsheet *spreadsheets;
    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;
    QStringList recentFiles;
    QString cuiFile;
    enum {MaxRecentFiles = 5};
    QAction *separatorAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *measureMakerAction;
    QAction *superKillerAction;
    QAction *recentFileActions[MaxRecentFiles];
    QAction *newAction;
    QAction *aboutAction;
    QAction *aboutTaskMgrAction;
    QAction *exitAction;
    QAction *optionAction;
    QToolBar *fileToolBar;
    QToolBar *toolToolBar;
    QToolBar *helpToolBar;
    QDialog *centralDialog;
    QwtPlot *resourcePlot;
    QwtPlotCurve *cpuCurve;
    QwtPlotCurve *ramCurve;
    QwtPlotCurve *taskCurve;

    newTaskDialog *openedtaskDialog;
};

#endif // MAINWINDOW_H
