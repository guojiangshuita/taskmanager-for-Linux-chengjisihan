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
#include <QListView>

measureMaker::measureMaker(QWidget *parent) :
    QDialog(parent)
{
    initTimerMaker();
    savingCondition='N';
}

void measureMaker::setInputFiles(QString input)
{
    inputFile=input;
}

void measureMaker::initTimerMaker()
{
    canBeSaved=false;
    setWindowTitle(tr("MeasureMaker"));
    timeSlider = new QSlider;
    timeSlider->setRange(500,1500);
    timeSpinBox = new QSpinBox;
    timeSpinBox->setRange(500,1500);
    connect(timeSlider, SIGNAL(valueChanged(int)), timeSpinBox, SLOT(setValue(int)));
    connect(timeSpinBox, SIGNAL(valueChanged(int)), timeSlider, SLOT(setValue(int)));
    labelTime = new QLabel("Intermission(ms):");
    timeGet = new QPushButton("Start Drawing");
    stopTimeGet = new QPushButton("Stop");
    connect(timeGet, SIGNAL(clicked()), this, SLOT(restartDrawing()));
    connect(stopTimeGet, SIGNAL(clicked()), this, SLOT(stopDrawing()));
    QVBoxLayout *sliderLayout = new QVBoxLayout;
    QVBoxLayout *controlButtonLayout = new QVBoxLayout;
    sliderLayout->addWidget(timeSpinBox);
    sliderLayout->addWidget(timeSlider);
    controlButtonLayout->addStretch();
    controlButtonLayout->addWidget(timeGet);
    controlButtonLayout->addWidget(stopTimeGet);
    timerLayout = new QVBoxLayout;
    timerLayout->addLayout(sliderLayout);
    timerLayout->addLayout(controlButtonLayout);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(timerStart()));
    mainLayout = new QVBoxLayout;
//    mainLayout->addLayout();
}

void measureMaker::restartDrawing()
{
    canBeSaved=true;
    timer->start(timeSlider->value());
}

void measureMaker::stopDrawing()
{
    timer->stop();
}

void measureMaker::timerStart()
{
    conditionPlot->replot();
}

void measureMaker::setSavingCondition(char c)
{
    savingCondition=c;
}

void measureMaker::initPlotCurve()
{
    conditionPlot = new QwtPlot;
    conditionPlot->setTitle("Source Condition");
    cpuCurve = new QwtPlotCurve("CPU");
    ramCurve = new QwtPlotCurve("RAM");
}

void measureMaker::initListView()
{
    taskList = new QListView(this);

}
