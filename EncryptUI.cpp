#include "EncryptUI.h"

EncryptUI::EncryptUI(bool& needChangeLang, Setting& setting, QWidget *parent)
    : QMainWindow(parent),
      m_rNeedChangeLang(needChangeLang),
      m_isAutoDetectRunning(false),
      m_haveFileOpening(false),
      m_isInputSaved(false),
      m_isOutputSaved(false),
      m_inputHaveChanged(false),
      m_outputHaveChanged(false),
      m_inputDir(""),
      m_outputDir(""),
      m_encryptCore(tr("Encode"), setting.autoDetectMode()),
      m_rSetting(setting)
{
    initUI();

    initMenuFile();
    initMenuEdit();
    initMenuFormat();
    initMenuSetting();
    initMenuHelp();
    initConfirmWindow();

    initStatusBar();

    setWindowIcon(QIcon(":Image/encrypt"));
    setWindowTitle(tr("Encrypt SBS"));
}

EncryptUI::~EncryptUI()
{ 
    delete m_pOpen;
    delete m_pSaveInput;
    delete m_pSaveInputAs;
    delete m_pSaveOutput;
    delete m_pSaveOutputAs;
    delete m_pCloseOpeningFile;
    delete m_pQuit;

    delete m_pSelectAllInput;
    delete m_pSelectAllOutput;
    delete m_pClearAll;

    delete m_pWordWrap;
    delete m_pFont;

    delete m_pAutoDetectMode;
    delete m_pEnableStayOnTop;
    delete m_pEnableStats;
    delete m_pLanguageEn_EN;
    delete m_pLanguageVi_VN;
    delete m_pSetLayoutVertical;
    delete m_pSetLayoutHorizontal;

    delete m_pAboutQt;
    delete m_pAboutApp;
    delete m_pLicense;

    delete m_pModeSelect;
    delete m_pLabelInput;
    delete m_pInputContent;
    delete m_pCopyInputToClipboard;
    delete m_pLabelOutput;
    delete m_pOutputContent;
    delete m_pCopyOutputToClipboard;
    delete m_pLayoutIO;

    delete m_pConfirmWindow;

    m_pOpen = nullptr;
    m_pSaveOutput = nullptr;
    m_pSaveOutputAs = nullptr;
    m_pCloseOpeningFile = nullptr;
    m_pQuit = nullptr;

    m_pSelectAllInput = nullptr;
    m_pSelectAllOutput = nullptr;
    m_pClearAll = nullptr;

    m_pWordWrap = nullptr;
    m_pFont = nullptr;

    m_pAutoDetectMode = nullptr;
    m_pEnableStayOnTop = nullptr;
    m_pEnableStats = nullptr;
    m_pLanguageEn_EN = nullptr;
    m_pLanguageVi_VN = nullptr;
    m_pSetLayoutVertical = nullptr;
    m_pSetLayoutHorizontal = nullptr;

    m_pAboutQt = nullptr;
    m_pAboutApp = nullptr;
    m_pLicense = nullptr;

    m_pModeSelect = nullptr;
    m_pLabelInput = nullptr;
    m_pInputContent = nullptr;
    m_pCopyInputToClipboard = nullptr;
    m_pLabelOutput = nullptr;
    m_pOutputContent = nullptr;
    m_pCopyOutputToClipboard = nullptr;
    m_pLayoutIO = nullptr;

    m_pCurrentLayout = nullptr;
    m_pCurrentLanguage = nullptr;

    m_pConfirmWindow = nullptr;
}

void EncryptUI::open()
{
    if (m_haveFileOpening) {
        close();
    }

    m_inputDir = QFileDialog::getOpenFileName(this, tr("Open"), QString(), tr("Encrypt SBS files (*.enc);; Text Documents (*.txt);; All files (*.*)"));

    if (m_inputDir.isEmpty()) {
        return;
    }


    QFile inputFile(m_inputDir);

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Can't open this file!"));
        return;
    }

    m_pLabelInput->setText(getFileName(m_inputDir));

    QTextStream in(&inputFile);
    m_pInputContent->setPlainText(in.readAll());

    inputFile.close();

    statusBar()->showMessage(tr("Open file: ") + m_inputDir, 6000);

    m_haveFileOpening = true;
    m_isInputSaved = true;
    m_inputHaveChanged = false;
}

void EncryptUI::saveAll()
{
    m_cancelClicked = false;
    m_pConfirmWindow->accept();
    saveInput();
    saveOutput();
}

void EncryptUI::saveInput(const bool &isSavedAs)
{
    m_cancelClicked = false;
    m_pConfirmWindow->accept();

    if (!m_inputHaveChanged && !isSavedAs) {
        return;
    }

    if (!m_isInputSaved || isSavedAs) {
        m_inputDir = QFileDialog::getSaveFileName(this, tr("Save input"), QString(), tr("Encrypt SBS files (*.enc);; Text Documents (*.txt);; All files (*.*)"));

        if (m_inputDir.isEmpty()) {
            QMessageBox::warning(this, tr("Attention"), tr("The input is not saved!"));
            return;
        }
    }

    QFile inputFile(m_inputDir);
    if (!inputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Can't save input file!"));
        return;
    }

    m_pLabelInput->setText(getFileName(m_inputDir));

    QTextStream out(&inputFile);
    out << m_pInputContent->toPlainText();

    inputFile.close();

    statusBar()->showMessage(tr("Input file: ") + m_inputDir, 6000);

    m_haveFileOpening = true;
    m_isInputSaved = true;
    m_inputHaveChanged = false;
}

void EncryptUI::saveOutput(const bool &isSaveAs)
{
    m_cancelClicked = false;
    m_pConfirmWindow->accept();

    if (!m_outputHaveChanged && !isSaveAs) {
        return;
    }

    if (!m_isOutputSaved || isSaveAs) {
        m_outputDir = QFileDialog::getSaveFileName(this, tr("Save output"), QString(), tr("Encrypt SBS files (*.enc);; Text Documents (*.txt);; All files (*.*)"));

        if (m_outputDir.isEmpty()) {
            QMessageBox::warning(this, tr("Attention"), tr("The output is not saved!"));
            return;
        }
    }

    QFile outputFile(m_outputDir);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Can't save output file!"));
        return;
    }

    m_pLabelOutput->setText(getFileName(m_outputDir));

    QTextStream out(&outputFile);
    out << m_pOutputContent->toPlainText();

    outputFile.close();

    statusBar()->showMessage(tr("Output file: ") + m_outputDir, 6000);

    m_haveFileOpening = true;
    m_isOutputSaved = true;
    m_outputHaveChanged = false;
}

void EncryptUI::saveAllAs()
{
    m_cancelClicked = false;
    m_pConfirmWindow->accept();

    saveInputAs();
    saveOutputAs();
}

void EncryptUI::saveInputAs()
{
    m_cancelClicked = false;
    m_pConfirmWindow->accept();

    saveInput(true);
}

void EncryptUI::saveOutputAs()
{
    m_cancelClicked = false;
    m_pConfirmWindow->accept();

    saveOutput(true);
}

void EncryptUI::doNotSave()
{
    m_cancelClicked = false;
    m_pConfirmWindow->accept();

    m_pInputContent->clear();
    m_pOutputContent->clear();
    m_pLabelInput->setText(tr("Input"));
    m_pLabelOutput->setText(tr("Output"));
    m_pInputContent->setFocus();

    m_haveFileOpening = false;
    m_inputDir = "";
    m_outputDir = "";
    m_isInputSaved = true;
    m_isOutputSaved = true;
}

void EncryptUI::close()
{
    if (!m_haveFileOpening) {
        statusBar()->showMessage(tr("Nothing to close!"), 6000);
        return;
    }

    if (m_inputHaveChanged && m_outputHaveChanged) {
        m_pConfirmWindow->exec();
    }

    m_pLabelInput->setText(tr("Input"));
    m_pLabelOutput->setText(tr("Output"));
    m_pInputContent->clear();
    m_pOutputContent->clear();

    m_haveFileOpening = false;
    m_isInputSaved = false;
    m_isOutputSaved = false;
    m_inputHaveChanged = false;
    m_outputHaveChanged = false;
    m_inputDir = "";
    m_outputDir = "";
}

void EncryptUI::cancel()
{
    m_cancelClicked = true;
    m_pConfirmWindow->accept();
}

void EncryptUI::wordWrapEnable(bool isChecked)
{
    m_rSetting.setWordWrap(isChecked);
    if (isChecked) {
        m_pInputContent->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        m_pOutputContent->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    }
    else {
        m_pInputContent->setWordWrapMode(QTextOption::NoWrap);
        m_pOutputContent->setWordWrapMode(QTextOption::NoWrap);
    }
}

void EncryptUI::selectFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(), this, tr("Font"));

    if (!ok) {
        return;
    }


    m_pInputContent->setFont(font);
    m_pOutputContent->setFont(font);
}

void EncryptUI::copyInputToClipboard()
{
    if (m_pInputContent->toPlainText().isEmpty()) {
        statusBar()->showMessage(tr("Nothing to coppy"), 6000);
        return;
    }

    bool yes = true;
    m_pInputContent->copyAvailable(yes);

    if (!yes) {
        m_pCopyInputToClipboard->setText(tr("Can't copy input to clipboard!"));
        m_pCopyInputToClipboard->setIcon(QIcon(":Image/cross"));
        statusBar()->showMessage("There's an error occurred!");
        return;
    }

    m_pInputContent->selectAll();
    m_pInputContent->copy();
    m_pInputContent->moveCursor(QTextCursor::End);
    m_pInputContent->setFocus();

    statusBar()->showMessage(tr("Coppied input to clipboard"), 6000);

    m_pCopyInputToClipboard->setText(tr("Coppied"));
        m_pCopyInputToClipboard->setIcon(QIcon(":Image/tick"));
}

void EncryptUI::copyOutputToClipboard()
{
    if (m_pOutputContent->toPlainText().isEmpty()) {
        statusBar()->showMessage(tr("Nothing to coppy"), 6000);
        return;
    }

    bool yes = true;
    m_pOutputContent->copyAvailable(yes);

    if (!yes) {
        m_pCopyOutputToClipboard->setText(tr("Can't copy output to clipboard!"));
        m_pCopyOutputToClipboard->setIcon(QIcon(":Image/cross"));
        statusBar()->showMessage("There's an error occurred!");
        return;
    }

    m_pOutputContent->selectAll();
    m_pOutputContent->copy();
    m_pOutputContent->moveCursor(QTextCursor::End);
    m_pInputContent->setFocus();

    statusBar()->showMessage(tr("Coppied output to clipboard"), 6000);

    m_pCopyOutputToClipboard->setText(tr("Coppied"));
    m_pCopyOutputToClipboard->setIcon(QIcon(":Image/tick"));
}

void EncryptUI::enableAutoDetectMode(bool isChecked)
{
    m_rSetting.setAutoDetectMode(isChecked);
    m_encryptCore.setAutoDetectEnable(isChecked);
}

void EncryptUI::enableStayOnTop(bool isChecked)
{
    m_rSetting.setAlwaysOnTop(isChecked);

    if (isChecked) {
        setWindowFlag(Qt::WindowStaysOnTopHint);
        show();

        statusBar()->showMessage(tr("Enabled always stay on top mode"), 6000);
    }
    else {
        setWindowFlag(Qt::WindowStaysOnTopHint, false);
        show();

        statusBar()->showMessage(tr("Disabled always stay on top mode"), 6000);
    }
}

void EncryptUI::enableStats(bool isChecked)
{
    m_rSetting.setEnableStats(isChecked);
    setWindowTitle(tr("Encrypt SBS"));
    inputChanged();
}

void EncryptUI::changeLanguage()
{
    QAction *language = qobject_cast<QAction*>(sender());

    if (language == nullptr || language == m_pCurrentLanguage) {
        m_pCurrentLanguage->setChecked(true);
        return;
    }

    if (m_rSetting.langCode() == "en_EN") {
        m_rSetting.setLangCode("vi_VN");
    }
    else {
        m_rSetting.setLangCode("en_EN");
    }

    int ans = QMessageBox::question(this, tr("Confirm restart"), tr("Do you want to restart now to apply this change?"), QMessageBox::Yes | QMessageBox::No);

    if (ans == QMessageBox::Yes) {
        m_rNeedChangeLang = true;
        qApp->quit();
    }
    else {
        m_rNeedChangeLang = false;
    }

    language->setChecked(false);
}

void EncryptUI::changeLayout()
{
    QAction *layout = qobject_cast<QAction*>(sender());

    if (layout == nullptr || layout == m_pCurrentLayout) {
        m_pCurrentLayout->setChecked(true);
        return;
    }

    m_pCurrentLayout->setChecked(false);
    m_pCurrentLayout = layout;
    m_pLayoutIO->setDirection(m_pLayoutIO->direction() == QBoxLayout::TopToBottom ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
    m_rSetting.setLayout(m_pCurrentLayout->text() == "&Horizontal" ? "Horizontal" : "Vertical");
}

void EncryptUI::aboutQt()
{
    QString content = tr("<big><b>About Qt</b></big><br><br>");
    content += tr("This program uses Qt version 6.3.0.<br><br>");
    content += tr("Qt is a C++ toolkit for cross-platform application development. <br><br>");
    content += tr("Qt provides single-source portability across all major desktop operating systems. It is also  available for embedded Linux and other embedded and mobile operating systems.<br><br>");
    content += tr("Qt is available under multiple licensing options designed to accommodate the needs of our various users.<br><br>");
    content += tr("Qt licensed under our commercial license agreement is appropriate for development of proprietary/commercial software where you do not want to share any source code with third parties or otherwise cannot comply with the terms of GNU (L)GPL.<br><br>");
    content += tr("Qt licensed under GNU (L)GPL is appropriate for the development of Qt applications provided you can comply with the terms and conditions of the respective licenses.<br><br>");
    content += tr("Please see <a href=\"https://qt.io/licensing\">qt.io/licensing</a> for an overview of Qt licensing.<br><br>");
    content += tr("Copyright (C) 2021 The Qt Company Ltd and other contributors.<br><br>");
    content += tr("Qt and the Qt logo are trademarks of The Qt Company Ltd.<br><br>");
    content += tr("Qt is The Qt Company Ltd product developed as an open source project. See <a href=\"https://qt.io\">qt.io</a> for more information.<br><br>");

    QMessageBox *aboutQt = new QMessageBox(this);
    aboutQt->setIconPixmap(QPixmap(":Image/logoQt"));
    aboutQt->setText(content);
    aboutQt->setWindowIcon(QIcon(":Image/logoQt"));
    aboutQt->setWindowTitle(tr("About Qt"));
    aboutQt->setStandardButtons(QMessageBox::Ok);
    aboutQt->exec();

}

void EncryptUI::aboutApp()
{
    QMessageBox::information(this, tr("About Encrypt SBS"), tr("This app is used to encode and decode text files to keep your data secret<br />This is the open source program. All source code can be found at <a href=\"https://github.com/tranmanhlt7a4/Encypt_SBS\">here.</a>"));
}

void EncryptUI::showLicense()
{
    QFile licenseFile(":License/License_" + m_rSetting.langCode());
    QTextStream in(&licenseFile);
    QString content("");

    if (!licenseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Can't load the license!\nReinstalling this program may fix it."));
        return;
    }
    else {
        content = in.readAll();
    }

    QMessageBox *licenseInfo = new QMessageBox(this);
    licenseInfo->setIconPixmap(QPixmap(":Image/license"));
    licenseInfo->setText(content);
    licenseInfo->setWindowTitle(tr("Encypt SBS"));
    licenseInfo->setStandardButtons(QMessageBox::Ok);
    licenseInfo->exec();
}

void EncryptUI::inputChanged()
{
    if (m_isAutoDetectRunning) {
        return;
    }

    resetCopyButton();

    if (m_pInputContent->toPlainText().isEmpty()) {
        m_pOutputContent->clear();
    }

    QString content = m_encryptCore.start(m_pInputContent->toPlainText());

    if (!content.isEmpty()) {
        if (m_rSetting.autoDetectMode()) {
            if (!autoDetectMode(content)) {
                m_pOutputContent->setPlainText(content);
            }
            else {
                QString temp = m_pInputContent->toPlainText();
                m_pInputContent->setPlainText(temp);
                m_pInputContent->moveCursor(QTextCursor::End);
                m_encryptCore.setCurrentMode(m_pModeSelect->currentText());
                m_pOutputContent->setPlainText(m_encryptCore.start(temp));
                m_isAutoDetectRunning = false;
            }
        }
        else {
            m_pOutputContent->setPlainText(content);
        }
    }

    showStats();

    if (m_haveFileOpening) {
        m_inputHaveChanged = true;
        m_outputHaveChanged = true;
    }
}

void EncryptUI::changeMode(const QString &mode)
{
    if (m_isAutoDetectRunning) {
        return;
    }

    m_encryptCore.setCurrentMode(mode);
    m_pInputContent->setFocus();
    m_pInputContent->clear();
    m_pOutputContent->clear();
    statusBar()->showMessage(tr("Mode: ") + m_pModeSelect->currentText(), 6000);
}

bool EncryptUI::detectDecodeMode(const QString &contentToTest)
{
    if (contentToTest.isEmpty()) {
        return false;
    }

    QStringList ls = m_pInputContent->toPlainText().simplified().split(" ");

    if (ls.size() < 4) {
        return false;
    }
    else {
        for (int i = 0; i < ls.size(); i++) {
            bool ok = false;

            ls[i].toInt(&ok);

            if (!ok) {
                return false;
            }
        }

        m_isAutoDetectRunning = true;
        m_pModeSelect->setCurrentIndex(1);

        statusBar()->showMessage(tr("Auto detect: Decode mode detected!"), 6000);
        return true;
    }
}

bool EncryptUI::detectEncodeMode(const QString &contentToTest)
{
    if (contentToTest.isEmpty()) {
        return false;
    }

    if (contentToTest == tr("Error: The input is invalid!")) {
        m_isAutoDetectRunning = true;
        m_pModeSelect->setCurrentIndex(0);

        statusBar()->showMessage(tr("Auto detect: Encode mode detected!"), 6000);
        return true;
    }

    return false;
}

void EncryptUI::closeEvent(QCloseEvent *event)
{
    m_cancelClicked = false;

    if (m_haveFileOpening && (m_inputHaveChanged || m_outputHaveChanged)) {
        m_pConfirmWindow->exec();
    }

    if (!m_cancelClicked) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void EncryptUI::initMenuFile()
{
    m_pOpen = new QAction(tr("&Open..."));
        m_pOpen->setIcon(QIcon(":Image/open"));
    m_pSaveInput = new QAction(tr("Save input..."));
        m_pSaveInput->setIcon(QIcon(":Image/save"));
    m_pSaveInputAs = new QAction(tr("Save input as..."));
        m_pSaveInputAs->setIcon(QIcon(":Image/saveAs"));
    m_pSaveOutput = new QAction(tr("&Save output..."));
        m_pSaveOutput->setIcon(QIcon(":Image/save"));
    m_pSaveOutputAs = new QAction(tr("Save output &as..."));
        m_pSaveOutputAs->setIcon(QIcon(":Image/saveAs"));
    m_pSaveAll = new QAction(tr("Save all..."));
    m_pSaveAllAs = new QAction(tr("Save all as..."));
    m_pCloseOpeningFile = new QAction(tr("&Close file"));
    m_pQuit = new QAction(tr("&Quit"));
        m_pQuit->setIcon(QIcon(":Image/quit"));

    QMenu *pMenuFile = menuBar()->addMenu(tr("&File"));
        pMenuFile->addAction(m_pOpen);
        pMenuFile->addSeparator();
        pMenuFile->addAction(m_pSaveInput);
        pMenuFile->addAction(m_pSaveInputAs);
        pMenuFile->addSeparator();
        pMenuFile->addAction(m_pSaveOutput);
        pMenuFile->addAction(m_pSaveOutputAs);
        pMenuFile->addSeparator();
        pMenuFile->addAction(m_pSaveAll);
        pMenuFile->addAction(m_pSaveAllAs);
        pMenuFile->addSeparator();
        pMenuFile->addAction(m_pCloseOpeningFile);
        pMenuFile->addSeparator();
        pMenuFile->addAction(m_pQuit);

    connect(m_pOpen, SIGNAL(triggered(bool)), this, SLOT(open()));
    connect(m_pSaveAll, SIGNAL(triggered(bool)), this, SLOT(saveAll()));
    connect(m_pSaveAllAs, SIGNAL(triggered(bool)), this, SLOT(saveAllAs()));
    connect(m_pSaveInput, SIGNAL(triggered(bool)), this, SLOT(saveInput()));
    connect(m_pSaveInputAs, SIGNAL(triggered(bool)), this, SLOT(saveInputAs()));
    connect(m_pSaveOutput, SIGNAL(triggered(bool)), this, SLOT(saveOutput()));
    connect(m_pSaveOutputAs, SIGNAL(triggered(bool)), this, SLOT(saveOutputAs()));
    connect(m_pCloseOpeningFile, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(m_pQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
}

void EncryptUI::initMenuEdit()
{
    m_pSelectAllInput = new QAction(tr("Select all &input"));
    m_pSelectAllOutput = new QAction(tr("Select all &output"));
    m_pClearAll = new QAction(tr("&Clear all"));

    QMenu *pMenuEdit = menuBar()->addMenu(tr("&Edit"));
        pMenuEdit->addAction(m_pSelectAllInput);
        pMenuEdit->addAction(m_pSelectAllOutput);
        pMenuEdit->addSeparator();
        pMenuEdit->addAction(m_pClearAll);

    connect(m_pSelectAllInput, SIGNAL(triggered(bool)), m_pInputContent, SLOT(selectAll()));
    connect(m_pSelectAllOutput, SIGNAL(triggered(bool)), m_pOutputContent, SLOT(selectAll()));
    connect(m_pClearAll, SIGNAL(triggered(bool)), m_pInputContent, SLOT(clear()));
}

void EncryptUI::initMenuFormat()
{
    m_pWordWrap = new QAction(tr("Word wrap"));
        m_pWordWrap->setCheckable(true);
        m_pWordWrap->setChecked(m_rSetting.wordWrap());
        wordWrapEnable(m_rSetting.wordWrap());
    m_pFont = new QAction(tr("Font..."));

    QMenu *pMenuFormat = menuBar()->addMenu(tr("&Format"));
        pMenuFormat->addAction(m_pWordWrap);
        pMenuFormat->addAction(m_pFont);

    connect(m_pWordWrap, SIGNAL(triggered(bool)), this, SLOT(wordWrapEnable(bool)));
    connect(m_pFont, SIGNAL(triggered(bool)), this, SLOT(selectFont()));
}

void EncryptUI::initMenuSetting()
{
    m_isAutoDetectRunning = false;

    m_pAutoDetectMode = new QAction(tr("Auto detect mode"));
        m_pAutoDetectMode->setCheckable(true);
        m_pAutoDetectMode->setChecked(m_rSetting.autoDetectMode());
    m_pEnableStayOnTop = new QAction(tr("Always on &top"));
        m_pEnableStayOnTop->setCheckable(true);
        m_pEnableStayOnTop->setChecked(m_rSetting.alwaysOnTop());
        enableStayOnTop(m_rSetting.alwaysOnTop());
    m_pEnableStats = new QAction(tr("Show stats"));
        m_pEnableStats->setCheckable(true);
        m_pEnableStats->setChecked(m_rSetting.enableStats());

    QMenu *pMenuSetting = menuBar()->addMenu(tr("&Setting"));
        pMenuSetting->addAction(m_pAutoDetectMode);
        pMenuSetting->addAction(m_pEnableStayOnTop);
        pMenuSetting->addAction(m_pEnableStats);

    connect(m_pAutoDetectMode, SIGNAL(triggered(bool)), this, SLOT(enableAutoDetectMode(bool)));
    connect(m_pEnableStayOnTop, SIGNAL(triggered(bool)), this, SLOT(enableStayOnTop(bool)));
    connect(m_pEnableStats, SIGNAL(triggered(bool)), this, SLOT(enableStats(bool)));

    m_pLanguageEn_EN = new QAction(tr("English"));
        m_pLanguageEn_EN->setCheckable(true);
        m_pLanguageEn_EN->setIcon(QIcon(":Image/englishFlag"));
    m_pLanguageVi_VN = new QAction(tr("Tiếng Việt"));
        m_pLanguageVi_VN->setCheckable(true);
        m_pLanguageVi_VN->setIcon(QIcon(":Image/vietnamFlag"));

    if (m_rSetting.langCode() == "en_EN") {
        m_pLanguageEn_EN->setChecked(true);
        m_pLanguageVi_VN->setChecked(false);
        m_pCurrentLanguage = m_pLanguageEn_EN;
    }
    else {
        m_pLanguageVi_VN->setChecked(true);
        m_pLanguageEn_EN->setChecked(false);
        m_pCurrentLanguage = m_pLanguageVi_VN;
    }
    QMenu *pMenuLanguage = pMenuSetting->addMenu(tr("&Language"));
        pMenuLanguage->addAction(m_pLanguageEn_EN);
        pMenuLanguage->addAction(m_pLanguageVi_VN);

    connect(m_pLanguageEn_EN, SIGNAL(triggered(bool)), this, SLOT(changeLanguage()));
    connect(m_pLanguageVi_VN, SIGNAL(triggered(bool)), this, SLOT(changeLanguage()));

    m_pSetLayoutHorizontal = new QAction(tr("&Horizontal"));
        m_pSetLayoutHorizontal->setCheckable(true);
    m_pSetLayoutVertical = new QAction(tr("&Vertical"));
        m_pSetLayoutVertical->setCheckable(true);

    if (m_rSetting.layout() == "Horizontal") {
        m_pSetLayoutHorizontal->setChecked(true);
        m_pCurrentLayout = m_pSetLayoutHorizontal;
    }
    else {
        m_pSetLayoutVertical->setChecked(true);
        m_pCurrentLayout = m_pSetLayoutVertical;
    }
    QMenu *pMenuLayout = pMenuSetting->addMenu(tr("&Layout"));
        pMenuLayout->addAction(m_pSetLayoutHorizontal);
        pMenuLayout->addAction(m_pSetLayoutVertical);

    connect(m_pSetLayoutVertical, SIGNAL(triggered(bool)), this, SLOT(changeLayout()));
    connect(m_pSetLayoutHorizontal, SIGNAL(triggered(bool)), this, SLOT(changeLayout()));
}

void EncryptUI::initMenuHelp()
{
    m_pAboutQt = new QAction(tr("About &Qt"));
        m_pAboutQt->setIcon(QIcon(":Image/logoQt"));
    m_pAboutApp = new QAction(tr("&About app"));
        m_pAboutApp->setIcon(QIcon(":Image/aboutApp"));
    m_pLicense = new QAction(tr("&License"));
        m_pLicense->setIcon(QIcon(":Image/license"));

    QMenu *pMenuAbout = menuBar()->addMenu(tr("&Help"));
        pMenuAbout->addAction(m_pAboutQt);
        pMenuAbout->addAction(m_pAboutApp);
        pMenuAbout->addAction(m_pLicense);

    connect(m_pAboutQt, SIGNAL(triggered(bool)), this, SLOT(aboutQt()));
    connect(m_pAboutApp, SIGNAL(triggered(bool)), this, SLOT(aboutApp()));
    connect(m_pLicense, SIGNAL(triggered(bool)), this, SLOT(showLicense()));
}

void EncryptUI::initConfirmWindow()
{
    m_pConfirmWindow = new QDialog(this);

    QLabel *info = new QLabel(tr("Files have change. Do you want to do?"));

    QPushButton *pSaveAll = new QPushButton(tr("Save all"));
    QPushButton *pSaveInput = new QPushButton(tr("Only save input"));
    QPushButton *pSaveOutput = new QPushButton(tr("Only save output"));
    QPushButton *pSaveAllAs = new QPushButton(tr("Save all as"));
    QPushButton *pSaveInputAs = new QPushButton(tr("Save input as"));
    QPushButton *pSaveOutputAs = new QPushButton(tr("Save output as"));
    QPushButton *pDoNotSave = new QPushButton(tr("Don't save"));
    QPushButton *pCancel = new QPushButton(tr("Cancel"));

    QGridLayout *mainLayout = new QGridLayout;
        mainLayout->addWidget(info, 1, 1, 1, 4, Qt::AlignCenter);
        mainLayout->addWidget(pSaveAll, 2, 1);
        mainLayout->addWidget(pSaveInput, 2, 2);
        mainLayout->addWidget(pSaveOutput, 2, 3);
        mainLayout->addWidget(pDoNotSave, 2, 4);
        mainLayout->addWidget(pSaveAllAs, 3, 1);
        mainLayout->addWidget(pSaveInputAs, 3, 2);
        mainLayout->addWidget(pSaveOutputAs, 3, 3);
        mainLayout->addWidget(pCancel, 3, 4);

    connect(pSaveAll, SIGNAL(clicked(bool)), this, SLOT(saveAll()));
    connect(pSaveInput, SIGNAL(clicked(bool)), this, SLOT(saveInput()));
    connect(pSaveOutput, SIGNAL(clicked(bool)), this, SLOT(saveOutput()));
    connect(pSaveAllAs, SIGNAL(clicked(bool)), this, SLOT(saveAllAs()));
    connect(pSaveInputAs, SIGNAL(clicked(bool)), this, SLOT(saveInputAs()));
    connect(pSaveOutputAs, SIGNAL(clicked(bool)), this, SLOT(saveOutputAs()));
    connect(pDoNotSave, SIGNAL(clicked(bool)), this, SLOT(doNotSave()));
    connect(pCancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));

    m_pConfirmWindow->setLayout(mainLayout);
}

void EncryptUI::initUI()
{
    m_pModeSelect = new QComboBox;
        m_pModeSelect->addItem(tr("Encode"));
        m_pModeSelect->addItem(tr("Decode"));

    QFormLayout *pLayoutMode = new QFormLayout;
        pLayoutMode->addRow(tr("&Mode: "), m_pModeSelect);

    m_pLabelInput = new QLabel(tr("Input"));
    m_pInputContent = new QPlainTextEdit;
        m_pInputContent->setFocus();
    m_pCopyInputToClipboard = new QPushButton(tr("Copy input to clipboard"));
        m_pCopyInputToClipboard->setIcon(QIcon(":Image/copy"));
    QVBoxLayout *pLayoutI = new QVBoxLayout;
        pLayoutI->addWidget(m_pLabelInput);
        pLayoutI->addWidget(m_pInputContent);
        pLayoutI->addWidget(m_pCopyInputToClipboard);

    m_pLabelOutput = new QLabel(tr("Output"));
    m_pOutputContent = new QPlainTextEdit;
        m_pOutputContent->setReadOnly(true);
    m_pCopyOutputToClipboard = new QPushButton(tr("Copy output to clipboard"));
        m_pCopyOutputToClipboard->setIcon(QIcon(":Image/copy"));
    QVBoxLayout *pLayoutO = new QVBoxLayout;
        pLayoutO->addWidget(m_pLabelOutput);
        pLayoutO->addWidget(m_pOutputContent);
        pLayoutO->addWidget(m_pCopyOutputToClipboard);

    connect(m_pCopyInputToClipboard, SIGNAL(clicked()), this, SLOT(copyInputToClipboard()));
    connect(m_pCopyOutputToClipboard, SIGNAL(clicked()), this, SLOT(copyOutputToClipboard()));

    if (m_rSetting.layout() == "Horizontal") {
        m_pLayoutIO = new QBoxLayout(QBoxLayout::LeftToRight);
    }
    else {
        m_pLayoutIO = new QBoxLayout(QBoxLayout::TopToBottom);
    }
        m_pLayoutIO->addLayout(pLayoutI);
        m_pLayoutIO->addLayout(pLayoutO);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
        pMainLayout->addLayout(pLayoutMode);
        pMainLayout->addLayout(m_pLayoutIO);

    QWidget *pWidgetCentral = new QWidget;
        pWidgetCentral->setLayout(pMainLayout);

    setCentralWidget(pWidgetCentral);

    connect(m_pInputContent, SIGNAL(textChanged()), this, SLOT(inputChanged()));
    connect(m_pModeSelect, SIGNAL(currentTextChanged(const QString&)), this, SLOT(changeMode(const QString&)));
}

void EncryptUI::initStatusBar()
{
    statusBar()->showMessage(tr("Welcome to Encrypt SBS!"), 6000);
}

int EncryptUI::inputSize() const
{
    if (m_pInputContent->toPlainText().isEmpty()) {
        return 0;
    }

    return m_pInputContent->toPlainText().simplified().split(" ").size();
}

int EncryptUI::outputSize() const
{
    if (m_pOutputContent->toPlainText().isEmpty()) {
        return 0;
    }

    return m_pOutputContent->toPlainText().simplified().split(" ").size();
}

void EncryptUI::resetCopyButton()
{
    m_pCopyInputToClipboard->setText(tr("Copy input to clipboard"));
        m_pCopyInputToClipboard->setIcon(QIcon(":Image/copy"));
    m_pCopyOutputToClipboard->setText(tr("Copy output to clipboard"));
        m_pCopyOutputToClipboard->setIcon(QIcon(":Image/copy"));
}

bool EncryptUI::autoDetectMode(QString contentToTest)
{   
    if (m_rSetting.autoDetectMode()) {
        if(detectEncodeMode(contentToTest)) {
            return true;
        }

        if (detectDecodeMode(contentToTest)) {
            return true;
        }
    }

    return false;
}

void EncryptUI::showStats()
{
    if (m_rSetting.enableStats()) {
        setWindowTitle(tr("Encrypt SBS") + " - " + tr("Input: %n word(s)", "", inputSize()) + " : " + tr("Output: %n word(s)", "", outputSize()));
    }
}

QString EncryptUI::getFileName(const QString &path)
{
    return path.right(path.size() - 1 - path.lastIndexOf("/"));
}
