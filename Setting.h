#ifndef SETTING_H
#define SETTING_H

#include <QString>
#include <QMap>
#include <QStringList>

class Setting {
public:
    Setting();
    void setOption(const QStringList& listOption);

    QString langCode() const;
    QString layout() const;
    bool autoDetectMode() const;
    bool alwaysOnTop() const;
    bool enableStats() const;
    bool wordWrap() const;

    void setLangCode(const QString& langCode);
    void setLayout(const QString& layout);
    void setAutoDetectMode(const bool& enable);
    void setAlwaysOnTop(const bool& enable);
    void setEnableStats(const bool& enable);
    void setWordWrap(const bool& enable);

    QString toString() const;

private:
    QString m_langCode;
    QString m_layout;
    bool m_autoDetectMode;
    bool m_alwaysOnTop;
    bool m_enableStats;
    bool m_wordWrap;
};

#endif // SETTING_H
