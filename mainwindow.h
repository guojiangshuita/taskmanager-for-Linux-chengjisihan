/**************
  **************

  Author: 成吉丝晗
  **************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QLabel;
class QListView;
class QPushButton;
//class QScrollBar;
class QMenu;
class QToolBar;
class QAction;
class spreadsheet;
class QProgressBar;
class QGridLayout;
class QDialog;
class QwtPlot;
class QwtPlotCurve;
class QMessageBox;
class newTaskDialog;
class finddialog;
class QStandardItemModel;
class QTableView;
class QHeaderView;
class QString;
class measureMaker;
class QFile;

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
    void finddialogs();
    void findNext(const QString &text, Qt::CaseSensitivity);
    void findPrevious(const QString &text, Qt::CaseSensitivity);

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void createToolBar();
    void createContextMenu();
    void createWidgets();
    void readSettings();
    void writeSettings();
    bool loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    bool readFile(const QString &fileName);
    bool writefile(const QString &fileName);
    void setTableView();
    void loadTasks();
    void initInfo();

    QLabel *locationLabel;
    QLabel *cpuCondition;
    QLabel *ramCondition;
    QLabel *swapCondition;
    QLabel *cpuUsedCondition;
    QLabel *ramUsedCondition;
    QLabel *cpuFreeCondition;
    QLabel *ramFreeCondition;
    QLabel *cpuTotalCondition;
    QLabel *ramTotalConditon;
    QLabel *swapUsedCondition;
    QLabel *swapFreeCondition;
    QLabel *swapTotalCondition;
    int cpuUsed, cpuTotal, cpuFree, ramUsed, ramTotal, ramFree, swapUsed, swapFree, swapTotal;
    QGridLayout *mainLayout;
    QTableView *taskTable;
    QHeaderView *headerView;
    QStandardItemModel *model;
    QPushButton *kill;
    QPushButton *refreshTask;
    QProgressBar *cpuPersentage;
    QProgressBar *ramPersentage;
    spreadsheet *spreadsheets;
    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;
    QStringList recentFiles;
    QString curFile;
    int a0, a1, b0, b1;
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
    QAction *finddialogAction;
    QToolBar *fileToolBar;
    QToolBar *toolToolBar;
    QToolBar *helpToolBar;
    QToolBar *optionToolBar;
    QDialog *centralDialog;
    QwtPlot *resourcePlot;
    QwtPlotCurve *cpuCurve;
    QwtPlotCurve *ramCurve;
    QwtPlotCurve *taskCurve;
    newTaskDialog *openedtaskDialog;
    finddialog *finddialoging;
    QString *fileName;
    measureMaker *mMSet;
    char saveConditionFlags;
};

#endif // MAINWINDOW_H
