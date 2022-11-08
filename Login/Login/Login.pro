#-------------------------------------------------
#
# Project created by QtCreator 2020-06-23T10:08:43
#
#-------------------------------------------------

QT += core gui sql multimedia multimediawidgets
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Login
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
RC_ICONS = lockicon.ico
SOURCES += \
        main.cpp \
        login.cpp \
    dialog.cpp \
    calc.cpp \
    welcome.cpp \
    helpdialog.cpp \
    drag.cpp \
    inputiddevicedialog.cpp \
    workthread.cpp

HEADERS += \
        login.h \
    dialog.h \
    calc.h \
    welcome.h \
    helpdialog.h \
    function.h \
    drag.h \
    inputiddevicedialog.h \
    workthread.h

FORMS += \
        login.ui \
    dialog.ui \
    calc.ui \
    welcome.ui \
    helpdialog.ui \
    inputiddevicedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

