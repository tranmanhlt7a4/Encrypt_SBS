#include "EncryptCore.h"

EncryptCore::EncryptCore(const QString &mode, const bool &autoDetectEnable)
    : m_currentMode(mode),
      m_autoDetectEnable(autoDetectEnable)
{

}

QString EncryptCore::currentMode() const
{
    return m_currentMode;
}

void EncryptCore::setCurrentMode(const QString &newMode)
{
    m_currentMode = newMode;
}

void EncryptCore::setAutoDetectEnable(const bool &val)
{
    m_autoDetectEnable = val;
}

QString EncryptCore::start(const QString &content)
{
    if (content.isEmpty()) {
        return "";
    }

    if (m_currentMode == QObject::tr("Encode")) {
        return encode(content);
    }
    else {
        return decode(content);
    }
}

QString EncryptCore::encode(const QString &content) const
{
    QString output("");
    int a1(0), a2(0), a3(0), a4(0);

    for (int i = 0; i < content.size(); i++) {
        a1 = QRandomGenerator::global()->bounded(1000);
        a2 = QRandomGenerator::global()->bounded(1000);
        a3 = QRandomGenerator::global()->bounded(1000);
        a4 = content[i].unicode() + a1 + a2 - a3;

        output += QString::number(a1) + " " + QString::number(a2) + " " + QString::number(a3) + " " + QString::number(a4) + " ";
    }

    return output;
}

QString EncryptCore::decode(const QString &content) const
{
    QString mainContent(content.simplified());
    mainContent = mainContent.replace("\n", " ");

    QString output("");
    QStringList ls(mainContent.split(" "));

    if (ls.isEmpty()) {
        return "";
    }

    if (ls.size() % 4 != 0) {
        if (!m_autoDetectEnable) {
            return QObject::tr("Not enough input data!");
        }
        else {
            bool ok = true;
            for (int i = 0; i < ls.size(); i++) {
                ls[i].toInt(&ok);

                if(!ok) {
                    return QObject::tr("Error: The input is invalid!");
                }
            }
        }
    }

    for (int i = 0; i < ls.size() - 3; i += 4) {
        bool ok1 = true, ok2 = true, ok3 = true, ok4 = true;

        int c = (ls[i + 3].toInt(&ok1) + ls[i + 2].toInt(&ok2)) - (ls[i + 1].toInt(&ok3) + ls[i].toInt(&ok4));

        if (!(ok1 &&  ok2 && ok3 && ok4)) {
            return QObject::tr("Error: The input is invalid!");
        }

        if (c < 0 || c > 0xffff) {
            return "";
        }

        output += QChar(c);
    }

    return output;
}
