QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BoolVariable.cpp \
    BoolWidget.cpp \
    ClientId.cpp \
    Dashboard.cpp \
    DoubleVariable.cpp \
    GridPos.cpp \
    JSONDecoder.cpp \
    JSONField.cpp \
    LabelIndicatorWidget.cpp \
    LabelWidget.cpp \
    MQTTClient.cpp \
    NumberWidget.cpp \
    StringVariable.cpp \
    TextWidget.cpp \
    TLSInfo.cpp \
    Topic.cpp \
    UserId.cpp \
    ValuedWidget.cpp \
    Variable.cpp \
    Variables.cpp \
    Widget.cpp \
    WidgetStyle.cpp \
    XMLFileReader.cpp \
    XMLSourcedEntity.cpp \
    main.cpp

HEADERS += \
    BoolVariable.h \
    BoolWidget.h \
    ClientId.h \
    Dashboard.h \
    DoubleVariable.h \
    GridPos.h \
    JSONDecoder.h \
    JSONField.h \
    LabelIndicatorWidget.h \
    LabelWidget.h \
    MQTTClient.h \
    NumberWidget.h \
    StringVariable.h \
    TLSInfo.h \
    TextWidget.h \
    Topic.h \
    UserId.h \
    ValuedWidget.h \
    Variable.h \
    Variables.h \
    Widget.h \
    WidgetStyle.h \
    XMLFileReader.h \
    XMLSourcedEntity.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -lpaho-mqtt3as
