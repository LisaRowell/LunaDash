QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BoolVariable.cpp \
    ClientStatusVariable.cpp \
    Dashboard.cpp \
    GridPos.cpp \
    Label.cpp \
    MQTTClient.cpp \
    StringVariable.cpp \
    Text.cpp \
    Topic.cpp \
    ValuedWidget.cpp \
    Variable.cpp \
    Variables.cpp \
    Widget.cpp \
    XMLSourcedEntity.cpp \
    main.cpp

HEADERS += \
    BoolVariable.h \
    ClientStatusVariable.h \
    Dashboard.h \
    GridPos.h \
    Label.h \
    MQTTClient.h \
    StringVariable.h \
    Text.h \
    Topic.h \
    ValuedWidget.h \
    Variable.h \
    Variables.h \
    Widget.h \
    XMLSourcedEntity.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -lpaho-mqtt3a
