QT       += network core gui sql qmqtt multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

TARGET = HANGER_CLIENT
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
include(./qextserialport/qextserialport.pri)

SOURCES += \
    DataReading.cpp \
    LogManager.cpp \
    main.cpp \
    mainwindow.cpp \
    hled.cpp \   
    db.cpp \
    server.cpp \
    uart.cpp \
    TrayIcon.cpp

HEADERS += \
    LogManager.h \
    mainwindow.h \
    hled.h

FORMS += \
    mainwindow.ui
# INCLUDEPATH += C:\Qt\5.14.2\mingw73_64\qmqtt\src\mqtt
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#//win32:RC_ICONS += your_icon.ico

RESOURCES += \
    files.qrc


