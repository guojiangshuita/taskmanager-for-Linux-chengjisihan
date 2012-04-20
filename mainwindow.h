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
class QMessageBox;
class newTaskDialog;
class finddialog;
class QStandardItemModel;
class QTableView;
class QHeaderView;
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
    int countTheTask();
    void setTableView();
    void loadTasks();

    QLabel *locationLabel;
    QLabel *cpuCondition;
    QLabel *ramCondition;
    QGridLayout *mainLayout;
    QTableView *taskTable;
    QHeaderView *headerView;
    QStandardItemModel *model;
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
    QString curFile;
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
};

#endif // MAINWINDOW_H
