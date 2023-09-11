#ifndef XML_SOURCED_ENTITY_H
#define XML_SOURCED_ENTITY_H

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>

class XMLSourcedEntity {
private:
    const QVector<QString> &allowedAttrs;
    const QVector<QString> &requiredAttrs;

    void unsupportedAttrError(const QXmlStreamAttribute &attribute,
                              const QString &fileName,
                              const QXmlStreamReader &xmlReader);
    void missingRequiredAttrError(const QString &attributeName,
                                  const QString &fileName,
                                  const QXmlStreamReader &xmlReader);

protected:
    void checkAttrs(const QXmlStreamAttributes &attributes,
                    const QString &fileName,
                    const QXmlStreamReader &xmlReader);
    void ignoreChildElements(QXmlStreamReader &xmlReader,
                             const QString &parentName,
                             const QString &fileName);
    void unsupportedChildElement(const QString &parentName,
                                 const QString &fileName,
                                 const QXmlStreamReader &xmlReader);

public:
    XMLSourcedEntity(const QVector<QString> &allowedAttrs,
                     const QVector<QString> &requiredAttrs);
};

#endif // XML_SOURCED_ENTITY_H
