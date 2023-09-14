#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "Variables.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QStringView>

class Dashboard : public QMainWindow {
    Q_OBJECT

private:
    Variables variables;
    QWidget *mainWidget;
    QGridLayout *layout;

    void initWindow();
    void readConfig(const QString &configFileName, Variables &variables);
    void readConfigStart(QXmlStreamReader &xmlReader,
                         const QString &fileName);
    void addMQTTClient(QXmlStreamReader &xmlReader, const QString &fileName);
    void addLabel(QXmlStreamReader &xmlReader, const QString &fileName);
    void addText(QXmlStreamReader &xmlReader, const QString &fileName);
    void fileOpenError(const QString &fileName, const QFile &file) const;
    void xmlParseError(const QString &fileName,
                       const QXmlStreamReader &xmlReader) const;
    void xmlDocumentStartError(const QString &fileName) const;
    void xmlDocumentTypeError(const QString &fileName,
                              const QXmlStreamReader &xmlReader) const;
    void unknownElementError(const QStringView &name,
                             const QString &parentName,
                             const QString &fileName,
                             const QXmlStreamReader &xmlReader) const;
    void missingGridPosError(const QString &widgetType,
                             const QString &fileName,
                             const QXmlStreamReader &xmlReader) const;

public:
    Dashboard(const QString &configFileName, QWidget *parent = nullptr);
    ~Dashboard();
};

#endif // DASHBOARD_H
