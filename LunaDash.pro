QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 qwt

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Adder.cpp \
    BoolVariable.cpp \
    BoolWidget.cpp \
    BoxWidget.cpp \
    ClientId.cpp \
    Dashboard.cpp \
    DoubleVariable.cpp \
    DurationDescriber.cpp \
    GaugeWidget.cpp \
    GridPos.cpp \
    JSONDecoder.cpp \
    JSONField.cpp \
    LabelIndicatorWidget.cpp \
    LabelWidget.cpp \
    MQTTClient.cpp \
    NumberWidget.cpp \
    Range.cpp \
    StringDecoder.cpp \
    StringVariable.cpp \
    TabBarWidget.cpp \
    TabWidget.cpp \
    TextWidget.cpp \
    TLSInfo.cpp \
    Topic.cpp \
    Transformer.cpp \
    UserId.cpp \
    ValuedWidget.cpp \
    Variable.cpp \
    Variables.cpp \
    Widget.cpp \
    WidgetGrid.cpp \
    WidgetStyle.cpp \
    WidgetStyles.cpp \
    XMLFileReader.cpp \
    XMLSourcedEntity.cpp \
    main.cpp

HEADERS += \
    Adder.h \
    BoolVariable.h \
    BoolWidget.h \
    BoxWidget.h \
    ClientId.h \
    Dashboard.h \
    DoubleVariable.h \
    DurationDescriber.h \
    GaugeWidget.h \
    GridPos.h \
    JSONDecoder.h \
    JSONField.h \
    LabelIndicatorWidget.h \
    LabelWidget.h \
    MQTTClient.h \
    NumberWidget.h \
    Range.h \
    StringDecoder.h \
    StringVariable.h \
    TLSInfo.h \
    TabBarWidget.h \
    TabWidget.h \
    TextWidget.h \
    Topic.h \
    Transformer.h \
    UserId.h \
    ValuedWidget.h \
    Variable.h \
    Variables.h \
    Widget.h \
    WidgetGrid.h \
    WidgetStyle.h \
    WidgetStyles.h \
    XMLFileReader.h \
    XMLSourcedEntity.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -lpaho-mqtt3as
