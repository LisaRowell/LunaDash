#include "Label.h"

#include "Dashboard.h"

#include <QLayout>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QStringView>

Dashboard::Dashboard(const QString &configFileName, QWidget *parent)
    : QMainWindow(parent) {
    initWindow();
    readConfig(configFileName);
}

Dashboard::~Dashboard() { 
}

void Dashboard::initWindow() {
    setWindowTitle("Luna");

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    layout = new QGridLayout(mainWidget);
}

// Pull in the XML the configuration file, doing a streaming read, creating
// objects as they're found.
void Dashboard::readConfig(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        fileOpenError(fileName, file);
    }

    QXmlStreamReader xmlReader(&file);
    readConfigStart(xmlReader, fileName);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("MQTTBroker") == 0) {
            xmlReader.skipCurrentElement();
        } else if (elementName.compare("Label") == 0) {
            addLabel(xmlReader, fileName);
        } else {
            unknownElementError(elementName, "Dashboard", fileName,
                                xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
    if (xmlReader.hasError()) {
        xmlParseError(fileName, xmlReader);
    }

    file.close();
}

void Dashboard::readConfigStart(QXmlStreamReader &xmlReader,
                                const QString &fileName) {
    if (!xmlReader.readNext()) {
        xmlParseError(fileName, xmlReader);
    }
    if (!xmlReader.isStartDocument()) {
        xmlDocumentStartError(fileName);
    }
    if (!xmlReader.readNextStartElement()) {
        xmlParseError(fileName, xmlReader);
    }
    if (xmlReader.name().compare("Dashboard") != 0) {
        xmlDocumentTypeError(fileName, xmlReader);
    }
}

void Dashboard::addLabel(QXmlStreamReader &xmlReader, const QString &fileName) {
    Label *label = new Label(xmlReader, fileName);
    if (label->hasGridPos()) {
        layout->addWidget(label, label->row(), label->col());
    } else {
        missingGridPos("Label", fileName, xmlReader);
        delete label;
    }
}

// We had some error opening the config file. Show an error message and
// abandon ship.
void Dashboard::fileOpenError(const QString &fileName, const QFile &file) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Failed to open config file '" << fileName << "':" << Qt::endl;
    errorStream << file.errorString();

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config File Open Error", errorStr);
    exit(EXIT_FAILURE);
}

// The Qt XML stream parse found something that it didn't like. Show an error
// message with it's error description and exit.
void Dashboard::xmlParseError(const QString &fileName,
                              const QXmlStreamReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Failed to parse config file '" << fileName << "' ("
                << xmlReader.lineNumber() << "," << xmlReader.columnNumber()
                <<"):" << Qt::endl;
    errorStream << xmlReader.errorString();

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Parse Error", errorStr);
    exit(EXIT_FAILURE);
}

void Dashboard::xmlDocumentStartError(const QString &fileName) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Failed to parse config file '" << fileName << "':" << Qt::endl;
    errorStream << "Missing document start";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Parse Error", errorStr);
    exit(EXIT_FAILURE);
}

// The root XML element wasn't the type we expected.
void Dashboard::xmlDocumentTypeError(const QString &fileName,
                                     const QXmlStreamReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Incorrect document type '" << xmlReader.name() << "' in '"
                << fileName << "' (" << xmlReader.lineNumber() << ", "
                << xmlReader.columnNumber() << ")." << Qt::endl;
    errorStream << "Expected Dashboard.";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Document Type Error", errorStr);
    exit(EXIT_FAILURE);
}

void Dashboard::unknownElementError(const QStringView &name,
                                    const QString &parentName,
                                    const QString &fileName,
                                    const QXmlStreamReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << parentName <<" contains unsupported element '" << name
                << "' in file '" << fileName << "' ("
                << xmlReader.lineNumber() << ", " << xmlReader.columnNumber()
                << "):" << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Element Error", errorStr);
}

void Dashboard::missingGridPos(const QString &widgetType,
                               const QString &fileName,
                               const QXmlStreamReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Dashboard " << widgetType << " widget missing GridPos in '"
                << fileName << "' (" << xmlReader.lineNumber() << ", "
                << xmlReader.columnNumber() << ")." << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Missing GridPos Error", errorStr);
}
