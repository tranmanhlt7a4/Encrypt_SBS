#include "Setting.h"

Setting::Setting()
    : m_langCode("en_EN"),
      m_layout("Horizontal"),
      m_autoDetectMode(true),
      m_alwaysOnTop(false),
      m_enableStats(false),
      m_wordWrap(true)
{

}

void Setting::setOption(const QStringList &listOption)
{
    for (int i = 0; i < listOption.size(); i++) {
        QStringList ls = listOption[i].split("=");

        if (ls[0] == "Language") {
            m_langCode = ls[1];
        }
        else if (ls[0] == "AlwaysOnTop") {
            m_alwaysOnTop = ls[1] == "true";
        }
        else if (ls[0] == "Layout") {
            m_layout = ls[1];
        }
        else if (ls[0] == "AutoDetectMode") {
            m_autoDetectMode = ls[1] == "true";
        }
        else if (ls[0] == "EnableStats") {
            m_enableStats = ls[1] == "true";
        }
        else if (ls[0] == "WordWrap") {
            m_wordWrap = ls[1] == "true";
        }
    }
}

QString Setting::langCode() const
{
    return m_langCode;
}

QString Setting::layout() const
{
    return m_layout;
}

bool Setting::autoDetectMode() const
{
    return m_autoDetectMode;
}

bool Setting::alwaysOnTop() const
{
    return m_alwaysOnTop;
}

bool Setting::enableStats() const
{
    return m_enableStats;
}

bool Setting::wordWrap() const
{
    return m_wordWrap;
}

void Setting::setLangCode(const QString &langCode)
{
    m_langCode = langCode;
}

void Setting::setLayout(const QString &layout)
{
    m_layout = layout;
}

void Setting::setAutoDetectMode(const bool &enable)
{
    m_autoDetectMode = enable;
}

void Setting::setAlwaysOnTop(const bool &enable)
{
    m_alwaysOnTop = enable;
}

void Setting::setEnableStats(const bool &enable)
{
    m_enableStats = enable;
}

void Setting::setWordWrap(const bool &enable)
{
    m_wordWrap = enable;
}

QString Setting::toString() const
{
    QString result = "";

    result += "Language=" + m_langCode + "\n";
    result += "Layout=" + m_layout + "\n";
    result += "AlwaysOnTop=" + (m_alwaysOnTop ? QString("true") : QString("false")) + "\n";
    result += "EnableStats=" + (m_enableStats ? QString("true") : QString("false")) + "\n";
    result += "AutoDetectMode=" + (m_autoDetectMode ? QString("true") : QString("false")) + "\n";
    result += "WordWrap=" + (m_wordWrap ? QString("true") : QString("false")) + "\n";

    return result;
}
