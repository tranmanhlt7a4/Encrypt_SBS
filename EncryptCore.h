#ifndef ENCRYPTCORE_H
#define ENCRYPTCORE_H

#include <QString>
#include <QRandomGenerator>
#include <QStringList>
#include <QObject>

class EncryptCore {
public:
    EncryptCore(const QString& mode = QObject::tr("Encode"), const bool& autoDetectEnable = false);

    QString currentMode() const;
    void setCurrentMode(const QString& newMode);

    void setAutoDetectEnable(const bool& val);

    QString start(const QString& content);

private:
    QString encode(const QString& content) const;
    QString decode(const QString& content) const;

    QString m_currentMode;

    bool m_autoDetectEnable;
};

#endif // ENCRYPTCORE_H
