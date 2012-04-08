#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    mainwindow *main = new mainwindow;
    main->show();
    return app.exec();
}
