HEADERS += \
    mainwindow.h \
    spreadsheet.h \
    newtaskdialog.h \
    finddialog.h \
    superkiller.h \
    measuremaker.h

SOURCES += \
    mainwindow.cpp \
    spreadsheet.cpp \
    newtaskdialog.cpp \
    main.cpp \
    finddialog.cpp \
    superkiller.cpp \
    measuremaker.cpp
INCLUDEPATH += /usr/local/qwt-6.0.1/include

LIBS += -L"/usr/local/qwt-6.0.1/lib/" -lqwt

OTHER_FILES += \
    ../../下载/726/filesaveas.ico \
    ../../下载/726/filesave.ico \
    ../../下载/726/fileopen.ico \
    ../../下载/726/run.ico \
    ../../下载/726/organizer.ico \
    ../../下载/726/exit.ico \
    ../../下载/726/db_update.ico \
    ../../下载/726/filenew.ico \
    ../../下载/726/help.ico \
    ../../下载/726/info.ico \
    ../../下载/726/gear.ico \
    images/3256.png \
    images/3257.png \
    images/3258.png \
    images/3259.png \
    images/3260.png \
    images/3261.png \
    images/3262.png \
    images/3263.png \
    images/3264.png \
    images/3265.png \
    images/3266.png \
    images/3267.png

FORMS +=
