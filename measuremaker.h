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
class QListView;

class measureMaker : public QDialog
{
    Q_OBJECT
public:
    explicit measureMaker(QWidget *parent = 0);
    void setInputFiles(QString input);
    void setSavingCondition(char c);
    QString outputFile;
    char flags;
signals:

protected:

    
public slots:
    void restartDrawing();
    void stopDrawing();
    void timerStart();
private:
    void initTimerMaker();
    void initPlotCurve();
    void initListView();
    QwtPlotCurve *cpuCurve;
    QwtPlotCurve *ramCurve;
    QwtPlot *conditionPlot;
    QSlider *timeSlider;
    QString inputFile;
    QLabel *labelTime;
    QSpinBox *timeSpinBox;
    QVBoxLayout *timerLayout;
    QVBoxLayout *mainLayout;
    QPushButton *timeGet;
    QPushButton *stopTimeGet;
    QListView *taskList;
    QPushButton *refreshButton;
    QTimer *timer;
    bool canBeSaved;
    char savingCondition;
};

#endif // MEASUREMAKER_H
