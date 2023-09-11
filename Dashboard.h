#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QStringView>

class Dashboard : public QMainWindow {
    Q_OBJECT

private:
    QWidget *mainWidget;
    QGridLayout *layout;

    void initWindow();
    void readConfig(const QString &configFileName);
    void readConfigStart(QXmlStreamReader &xmlReader,
                         const QString &fileName);
    void addLabel(QXmlStreamReader &xmlReader, const QString &fileName);
    void fileOpenError(const QString &fileName, const QFile &file);
    void xmlParseError(const QString &fileName,
                       const QXmlStreamReader &xmlReader);
    void xmlDocumentStartError(const QString &fileName);
    void xmlDocumentTypeError(const QString &fileName,
                              const QXmlStreamReader &xmlReader);
    void unknownElementError(const QStringView &name,
                             const QString &parentName,
                             const QString &fileName,
                             const QXmlStreamReader &xmlReader);
    void missingGridPos(const QString &widgetType,
                        const QString &fileName,
                        const QXmlStreamReader &xmlReader);

public:
    Dashboard(const QString &configFileName, QWidget *parent = nullptr);
    ~Dashboard();
};

#endif // DASHBOARD_H
