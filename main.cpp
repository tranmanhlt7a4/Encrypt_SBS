#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QString>
#include "EncryptUI.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QTranslator trans;
    if (!trans.load(":Translator/Encrypt_SBS_vi_VN"))
    {
        QMessageBox::critical(nullptr, "Error", "Can't load language file!");
    }


    bool needChangeLang = true;
    QString langCode = "en_EN";
    QString layout = "Horizontal";
    bool autoDetectMode = true;
    bool onTop = false;
    bool enableStats = false;
;
    while (needChangeLang) {
        needChangeLang = false;
        if (langCode == "vi_VN") {
            qApp->installTranslator(&trans);
        }
        else {
            qApp->removeTranslator(&trans);
        }

        EncryptUI *encrypt = new EncryptUI(needChangeLang, langCode, layout, autoDetectMode, onTop, enableStats);
        encrypt->show();

        app.exec();

        delete encrypt;
    }

    return 0;
}
