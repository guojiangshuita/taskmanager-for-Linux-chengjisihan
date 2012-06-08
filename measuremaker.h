#ifndef MEASUREMAKER_H
#define MEASUREMAKER_H

#include <QDialog>

class QwtPlotCurve;
class QLabel;
class QwtPlot;
class QPushButton;
class QProgressBar;
class QTimer;
class QGridLayout;
class QSlider;
class QSpinBox;
class QVBoxLayout;
class QHBoxLayout;
class QTableView;
class QStandardItemModel;
class QHeaderView;

class measureMaker : public QDialog
{
    Q_OBJECT
public:
    explicit measureMaker(QWidget *parent = 0);
    QString outputFile;
    char flags;
signals:

protected:
    void timerEvent(QTimerEvent *event);
    int m_nTimerId;
    
public slots:
    void restartDrawing();
    void stopDrawing();
    void timerStart();
    void returnToSuper();
    void saveClicked();
    void openClicked();
    void RefreshClicked();
private:
    void loadTasks();
    void initTimerMaker();
    void initPlotCurve();
    void initTableView();
    QwtPlotCurve *cpuCurve;
    QwtPlotCurve *ramCurve;
    QwtPlot *conditionPlot;
    QSlider *timeSlider;
    QString inputFile;
    QLabel *labelTime;
    QSpinBox *timeSpinBox;
    QPushButton *timeGet;
    QPushButton *stopTimeGet;
    QPushButton *savePushButton;
    QPushButton *openPushButton;
    QPushButton *returnPushButton;
    QPushButton *refreshPushButton;
    QTableView *taskTable;
    QStandardItemModel *model;
    QHeaderView *headerView;
    QTimer *timer;
};

#endif // MEASUREMAKER_H
