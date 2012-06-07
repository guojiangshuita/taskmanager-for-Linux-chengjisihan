#include <QApplication>
#include "mainwindow.h"
#include <QSplashScreen>
#include <QDesktopWidget>
#include "finddialog.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QDesktopWidget *desktop = app.desktop();
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("./images/splash2.png"));
    splash->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("Setting up the main window ..."), topRight, Qt::white);
    mainwindow main;
    main.move(desktop->width()/2-main.width()/2,desktop->height()/2-main.height()/2);
    splash->showMessage(QObject::tr("Loading modules..."), topRight, Qt::white);
    //put the func here
    main.show();
    sleep(1);
    splash->finish(&main);
    delete splash;
    return app.exec();
}
