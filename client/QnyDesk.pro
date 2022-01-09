#-------------------------------------------------
#
# Project created by QtCreator 2022-01-06T14:07:20
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QnyDesk 

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


SOURCES += \
        main.cpp \
        MainWindow.cpp \
        common/DeviceInfo.cpp \
        common/NetworkHandler.cpp \
        protocol/Exchange.pb.cc 

HEADERS += \
        MainWindow.h \
        button/SettingButton.h \
        common/NetworkHandler.h \
        common/DeviceInfo.h \
        common/Logger.h \
        qny_passive/PassiveHandler.h \
        protocol/Exchange.pb.h \
        button/PasswordButton.h

FORMS += \
        ui/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc \
    css.qrc

win32: LIBS += -LD:/CTools/protobuf/lib/ -llibprotobuf.dll
INCLUDEPATH += D:/CTools/protobuf/include
DEPENDPATH += D:/CTools/protobuf
