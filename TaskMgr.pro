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
    ../../下载/726/gear.ico

FORMS +=
