#ifndef ENCRYPTUI_H
#define ENCRYPTUI_H

#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QFormLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>
#include <QStatusBar>
#include <QTranslator>
#include <QTextStream>
#include <QFont>
#include <QFontDialog>

#include "EncryptCore.h"

#define FILE_NEED_DECODE_EXTENSION ".desbs"
#define FILE_NEED_ENCODE_EXTENSION ".ensbs"

class EncryptUI : public QMainWindow {
    Q_OBJECT

public:
    EncryptUI(bool& changeLang, QString& langCode, QString& layout, bool& autoDetectMode, bool& onTop, bool& enableStats, QWidget* parent = nullptr);
    ~EncryptUI();

private slots:
    void open();
    void saveAll();
    void saveInput(const bool& isSavedAs = false);
    void saveOutput(const bool& isSaveAs = false);
    void saveAllAs();
    void saveInputAs();
    void saveOutputAs();
    void doNotSave();
    void close();

    void wordWrapEnable(bool isChecked);
    void selectFont();

    void copyInputToClipboard();
    void copyOutputToClipboard();
    void enableAutoDetectMode(bool isChecked);
    void enableStayOnTop(bool isChecked);
    void enableStats(bool isChecked);
    void changeLanguage();
    void changeLayout();

    void aboutQt();
    void aboutApp();
    void showLicense();


    void inputChanged();
    void changeMode(const QString& mode);
    bool detectDecodeMode(const QString& contentToTest);
    bool detectEncodeMode(const QString& contentToTest);

private:
//    void closeEvent(QCloseEvent* event);

    void initMenuFile();
    void initMenuEdit();
    void initMenuFormat();
    void initMenuSetting();
    void initMenuHelp();

    void initConfirmWindow();

    void initUI();

    void initStatusBar();

    int inputSize() const;
    int outputSize() const;

    void resetCopyButton();
    bool autoDetectMode(QString contentToTest);
    void showStats();

    QString getFileName(const QString& path);

    QAction *m_pOpen;
    QAction *m_pSaveAll;
    QAction *m_pSaveInput;
    QAction *m_pSaveInputAs;
    QAction *m_pSaveAllAs;
    QAction *m_pSaveOutput;
    QAction *m_pSaveOutputAs;
    QAction *m_pCloseOpeningFile;
    QAction *m_pQuit;

    QAction *m_pSelectAllInput;
    QAction *m_pSelectAllOutput;
    QAction *m_pClearAll;

    QAction *m_pWordWrap;
    QAction *m_pFont;

    QAction *m_pAutoDetectMode;
    QAction *m_pEnableStayOnTop;
    QAction *m_pEnableStats;
    QAction *m_pLanguageEn_EN;
    QAction *m_pLanguageVi_VN;
    QAction *m_pSetLayoutVertical;
    QAction *m_pSetLayoutHorizontal;

    QAction *m_pAboutQt;
    QAction *m_pAboutApp;
    QAction *m_pLicense;

    QComboBox *m_pModeSelect;
    QLabel *m_pLabelInput;
    QPlainTextEdit *m_pInputContent;
    QPushButton *m_pCopyInputToClipboard;
    QLabel *m_pLabelOutput;
    QPlainTextEdit *m_pOutputContent;
    QPushButton *m_pCopyOutputToClipboard;
    QBoxLayout *m_pLayoutIO;

    QAction *m_pCurrentLayout;
    QAction *m_pCurrentLanguage;

    bool &m_rNeedChangeLang;
    QString &m_rLanguageCode; //Format lang_COUNTRY. Ex: en_EN
    QString &m_rLayout;
    bool &m_rAutoDetectMode;
    bool &m_rOnTop;
    bool &m_rEnableStats;

    bool m_isAutoDetectRunning;

    bool m_haveFileOpening;
    bool m_isInputSaved;
    bool m_isOutputSaved;
    bool m_inputHaveChanged;
    bool m_outputHaveChanged;
    QString m_inputDir;
    QString m_outputDir;

    EncryptCore m_encryptCore;
    QDialog *m_pConfirmWindow;
};

#endif // ENCRYPTUI_H
