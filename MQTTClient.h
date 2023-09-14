#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "XMLSourcedEntity.h"

#include "Variables.h"
#include "ClientStatusVariable.h"

#include <MQTTAsync.h>

#include <QXmlStreamReader>
#include <QVector>
#include <QString>
#include <QObject>

class MQTTClient : public XMLSourcedEntity {
    Q_OBJECT

private:
    QVector<ClientStatusVariable *>statusVariables;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

    QString serverName;
    MQTTAsync handle;

    void startConnection();
    void connectedCallbackInvoked();
    void connectionLostCallbackInvoked();
    void addStatusVariable(QXmlStreamReader &xmlReader,
                           const QString &fileName, Variables &variables);
    void mqttError(const QString description, int error) const;

    static void connectedCallback(void *context, char *cause);
    static void connectionLostCallback(void *context, char *cause);
    static int messageArrivedCallback(void *context, char *topicName,
                                      int topicLen,
                                      MQTTAsync_message *message);
    static void deliveryCompleteCallback(void *context, MQTTAsync_token token);

public slots:
    void connected();
    void disconnected();

signals:
    void connectedSignal();
    void connectionLostSignal();

public:
    MQTTClient(QXmlStreamReader &xmlReader, const QString &fileName,
               Variables &variables);
};

#endif // MQTT_CLIENT_H
