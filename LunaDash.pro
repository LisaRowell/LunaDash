QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    MQTT Transformers Variables Widgets

SOURCES += \
    Dashboard.cpp \
    MQTT/ClientId.cpp \
    MQTT/JSONDecoder.cpp \
    MQTT/JSONField.cpp \
    MQTT/MQTTClient.cpp \
    MQTT/TLSInfo.cpp \
    MQTT/Topic.cpp \
    MQTT/UserId.cpp \
    Transformers/Adder.cpp \
    Transformers/Concatenator.cpp \
    Transformers/DurationDescriber.cpp \
    Transformers/StringDecoder.cpp \
    Transformers/StringInput.cpp \
    Transformers/SuffixRemover.cpp \
    Transformers/Transformer.cpp \
    Variables.cpp \
    Variables/BoolVariable.cpp \
    Variables/DoubleVariable.cpp \
    Variables/StringVariable.cpp \
    Variables/Variable.cpp \
    WidgetGrid.cpp \
    Widgets/BoolWidget.cpp \
    Widgets/BoxWidget.cpp \
    Widgets/DialGaugeWidget.cpp \
    Widgets/GridPos.cpp \
    Widgets/LabelIndicatorWidget.cpp \
    Widgets/LabelWidget.cpp \
    Widgets/NumberWidget.cpp \
    Widgets/NumericGaugeWidget.cpp \
    Widgets/Range.cpp \
    Widgets/ScaleSteps.cpp \
    Widgets/ScaleThreshold.cpp \
    Widgets/ScaleThresholds.cpp \
    Widgets/SpacerWidget.cpp \
    Widgets/TabBarWidget.cpp \
    Widgets/TabWidget.cpp \
    Widgets/TextWidget.cpp \
    Widgets/ValuedWidget.cpp \
    Widgets/Widget.cpp \
    WidgetStyle.cpp \
    WidgetStyles.cpp \
    XMLFileReader.cpp \
    XMLSourcedEntity.cpp \
    main.cpp

HEADERS += \
    Dashboard.h \
    MQTT/ClientId.h \
    MQTT/JSONDecoder.h \
    MQTT/JSONField.h \
    MQTT/MQTTClient.h \
    MQTT/TLSInfo.h \
    MQTT/Topic.h \
    MQTT/UserId.h \
    Transformers/Adder.h \
    Transformers/Concatenator.h \
    Transformers/DurationDescriber.h \
    Transformers/StringDecoder.h \
    Transformers/StringInput.h \
    Transformers/SuffixRemover.h \
    Transformers/Transformer.h \
    Variables.h \
    Variables/BoolVariable.h \
    Variables/DoubleVariable.h \
    Variables/StringVariable.h \
    Variables/Variable.h \
    WidgetGrid.h \
    Widgets/BoolWidget.h \
    Widgets/BoxWidget.h \
    Widgets/DialGaugeWidget.h \
    Widgets/GridPos.h \
    Widgets/LabelIndicatorWidget.h \
    Widgets/LabelWidget.h \
    Widgets/NumberWidget.h \
    Widgets/NumericGaugeWidget.h \
    Widgets/Range.h \
    Widgets/ScaleSteps.h \
    Widgets/ScaleThreshold.h \
    Widgets/ScaleThresholds.h \
    Widgets/SpacerWidget.h \
    Widgets/TabBarWidget.h \
    Widgets/TabWidget.h \
    Widgets/TextWidget.h \
    Widgets/ValuedWidget.h \
    Widgets/Widget.h \
    WidgetStyle.h \
    WidgetStyles.h \
    XMLFileReader.h \
    XMLSourcedEntity.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -lpaho-mqtt3as

DISTFILES += \
    COPYING \
    README.md \
    LunaDash-icon-256x256.png \
    LunaDash.desktop \
    LunaDash-ldash.xml \
    Examples/Dashboard.ldash \
    Examples/SpacerExample.ldash \
    Examples/Batteries.png \
    Examples/Power.png \
    SpacerExample.png
