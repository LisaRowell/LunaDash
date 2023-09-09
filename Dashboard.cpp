#include "Dashboard.h"

#include <QLayout>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QStringView>
#include <QPushButton>

Dashboard::Dashboard(const QString &configFileName, QWidget *parent)
    : QMainWindow(parent) {
    initWindow();
    readConfig(configFileName);

    addTestWidgets();
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
        xmlReader.skipCurrentElement();
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
        xmlDocumentTypeError(fileName, xmlReader.name());
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
    errorStream << "Failed to parse config file '" << fileName << "':" << Qt::endl;
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
                                     const QStringView &foundType) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Incorrect document type '" << foundType << "' in '";
    errorStream << fileName << "'. Expected Dashboard.";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Document Type Error", errorStr);
    exit(EXIT_FAILURE);
}

void Dashboard::addTestWidgets() {
    QPushButton *button1 = new QPushButton("One");
    QPushButton *button2 = new QPushButton("Two");
    QPushButton *button3 = new QPushButton("Three");
    QPushButton *button4 = new QPushButton("Four");
    QPushButton *button5 = new QPushButton("Five");

    layout->addWidget(button1, 0, 0);
    layout->addWidget(button2, 0, 1);
    layout->addWidget(button3, 1, 0, 1, 2);
    layout->addWidget(button4, 2, 0);
    layout->addWidget(button5, 2, 1);
}
