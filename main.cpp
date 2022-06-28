#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QString>
#include "EncryptUI.h"
#include "Setting.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QTranslator trans;
    if (!trans.load(":Translator/Encrypt_SBS_vi_VN"))
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Can't load language file!"));
    }

    Setting setting;

    QFile settingFile("Setting.ini");
    if (!settingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Can't load setting, the program will start with defautl setting!"));
    }
    else {
        QTextStream in(&settingFile);
        setting.setOption(in.readAll().split("\n"));
    }
    settingFile.close();

    bool needChangeLang = true;

    while (needChangeLang) {
        needChangeLang = false;
        if (setting.langCode() == "vi_VN") {
            qApp->installTranslator(&trans);
        }
        else {
            qApp->removeTranslator(&trans);
        }

        EncryptUI *encrypt = new EncryptUI(needChangeLang, setting);
        encrypt->show();

        app.exec();

        delete encrypt;
    }

    if (!settingFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Can't save setting."));
    }
    else {
        QTextStream out(&settingFile);
        out << setting.toString();
    }

    return 0;
}
