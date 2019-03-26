#include "mainwindow.h"
#include "imagedownloader.h"
#include "util.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>
#include <QToolBar>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QVector>
#include <QFileDialog>
#include <QInputDialog>
#include <QDialog>
#include <QProgressDialog>
#include <QMessageBox>
#include <QNetworkReply>
#include <QThread>
#include <QPalette>
#include <QDebug>

#define SCREEN_WIDTH QApplication::desktop()->screenGeometry().width()
#define SCREEN_HEIGHT QApplication::desktop()->screenGeometry().height()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    createMenu(this);
    createStatusBar(this);
    createWorkArea(this);
    createToolBar(this);

    setMouseTracking(true);
    workArea->setMouseTracking(true);

    connectSlots();

    setCustomStyle();
}

MainWindow::~MainWindow(){}

void MainWindow::setCustomStyle()
{
    QString styles;
    styles += "QMainWindow{background-color: #ffffff;}"

              "QMenuBar{background-color: #e8e8e8; border:none; padding: 0px;}"
              "QMenuBar::item{background-color: #e8e8e8; padding: 3px 10px 3px 10px;}"
              "QMenuBar::item:selected{background-color: #ffffff;}"

              "QMenu{background-color: #ffffff;}"
              "QMenu::item{background-color: #ffffff; padding: 3px 30px 3px 30px;}"
              "QMenu::item:selected{background-color: #e8e8e8; color: #000000;}"
              "QMenu::icon{padding-left: 8px;}"

              "QToolBar{border: 2px solid #e8e8e8; padding: 0px;}"
              "QToolButton{padding: 3px 5px 3px 5px; background-color: #ffffff;border: none;}"
              "QToolButton:hover{background-color: #e8e8e8;}"

              "QScrollArea{background-color: #ffffff; border: 2px solid #e8e8e8;}"

              "QStatusBar{background-color: #e8e8e8;}"
              "QStatusBar::item{border: none;}"

              "QPushButton{background-color: #ffffff; border: 2px solid #e8e8e8; width: 76px; height: 25px;}"
              "QPushButton:hover{background-color: #e8e8e8;}"

              "QDialog{background-color: #ffffff;}"

              "QLineEdit{background-color: #ffffff; border: 2px solid #e8e8e8;}"

              "QProgressBar{background-color: #ffffff; border: 2px solid #e8e8e8; text-align: center;}"
              "QProgressBar:chunk{background-color: #e8e8e8;}"

              "QMessageBox{background-color: #ffffff;}";

    setStyleSheet(styles);
}

/**
 * @brief MainWindow::createMenu
 * @param mainWindow the parent mainWindow.
 * Originally it can be omitted and use "this" in the function, but it will cause name conflict.
 * So use the parameter to avoid it.
 *
 * Create all menu of the application.
 */
void MainWindow::createMenu(QMainWindow *mainWindow)
{
    menuBar = mainWindow->menuBar();

    fileMenu = menuBar->addMenu(tr("文件"));
    settingMenu = menuBar->addMenu(tr("设置"));
    aboutMenu = menuBar->addMenu(tr("关于"));

    openImageMenu = fileMenu->addMenu(QIcon(":/icon/icon/image.png"), tr("打开图片"));
    openImageByLocalAction = openImageMenu->addAction(QIcon(":/icon/icon/folder.png"), tr("打开本地图片"));
    openImageByUrlAction = openImageMenu->addAction(QIcon(":/icon/icon/cloud-download.png"), tr("打开网络图片"));

    openHistoryImageMenu = fileMenu->addMenu(QIcon(":/icon/icon/time-circle.png"), tr("最近文件"));

    saveColorBoardMenu = fileMenu->addMenu(QIcon(":/icon/icon/save.png"), tr("保存色板"));
    saveAsTxtAction = saveColorBoardMenu->addAction(QIcon(":/icon/icon/file-text.png"), tr("保存为txt"));
    saveAsJpgAction = saveColorBoardMenu->addAction(QIcon(":/icon/icon/file-image.png"), tr("保存为jpg"));

    restartAction = fileMenu->addAction(QIcon(":/icon/icon/reload.png"), tr("重新生成"));

    exitAction = fileMenu->addAction(QIcon(":/icon/icon/close-square.png"), tr("退出"));

    preferenceAction = settingMenu->addAction(QIcon(":/icon/icon/setting.png"), tr("首选项"));

    referenceAction = aboutMenu->addAction(QIcon(":/icon/icon/cloud.png"), tr("参考"));
    authorAction = aboutMenu->addAction(QIcon(":/icon/icon/user.png"), tr("作者"));
}

/**
 * @brief MainWindow::createStatusBar
 * @param mainWindow the parent mainWindow. Explanation same as above.
 *
 * Create the status bar and all label in it.
 */
void MainWindow::createStatusBar(QMainWindow *mainWindow)
{
    statusBar = mainWindow->statusBar();

    fileInfoLabel = new QLabel(tr(""), statusBar);
    curInfoLabel = new QLabel(tr(""), statusBar);
    showScaleRatioLabel = new QLabel(tr(""), statusBar);
    colorValueLabel = new QLabel(statusBar);
    helpTextLabel = new QLabel(tr(""));

    fileInfoLabel->setAlignment(Qt::AlignCenter);
    curInfoLabel->setAlignment(Qt::AlignCenter);
    showScaleRatioLabel->setAlignment(Qt::AlignCenter);
    colorValueLabel->setAlignment(Qt::AlignCenter);
    helpTextLabel->setAlignment(Qt::AlignCenter);

    statusBar->addPermanentWidget(fileInfoLabel, 3);
    statusBar->addWidget(curInfoLabel, 4);
    statusBar->addWidget(colorValueLabel, 1);
    statusBar->addWidget(helpTextLabel, 5);
    statusBar->addWidget(showScaleRatioLabel, 2);
}

/**
 * @brief MainWindow::createToolBar
 * @param mainWindow the parent mainWindow. Explanation same as above.
 *
 * Create tool bar which corresponds the menu.
 */
void MainWindow::createToolBar(QMainWindow *mainWindow)
{
    toolBar = new QToolBar(this);
    toolBar->setMovable(false);
    mainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

    toolBar->addAction(openImageByLocalAction);
    toolBar->addAction(openImageByUrlAction);
    toolBar->addAction(saveAsTxtAction);
    toolBar->addAction(saveAsJpgAction);
    toolBar->addAction(restartAction);
    toolBar->addAction(exitAction);
    toolBar->addSeparator();
    toolBar->addAction(preferenceAction);
    toolBar->addSeparator();
    toolBar->addAction(referenceAction);
    toolBar->addAction(authorAction);
}

/**
 * @brief MainWindow::createWorkArea
 * @param mainWindow the parent mainWindow. Explanation same as above.
 *
 * Create work area and make it be the central widget. Work area includes the image container and
 * color board which are the core widget of the application.
 */
void MainWindow::createWorkArea(QMainWindow *mainWindow)
{
    workArea = new WorkArea(mainWindow);
    mainWindow->setCentralWidget(workArea);
}

/**
 * @brief MainWindow::openFileDialog
 *
 * When click the button "open local file", the application will open a file dialog to let users select images.
 * If users cancel it, the file dialog will return a null string and the function doesn't need to
 * do anything more.
 * If users select a image, the application will show it and create the color board of it.
 */
void MainWindow::openFileDialog()
{
    curFileName = QFileDialog::getOpenFileName(this, tr("打开图片"), "E:/waste",
                                               tr("Images((*.png *.jpg *.jpeg *.bmp))"));
    if(curFileName == "") {
        return;
    }

    showNewSelectedImage(curFileName);
    createNewSelectedImageColorBoard();
}

/**
 * @brief MainWindow::openDownloadDialog
 *
 * When click the button "open online image", the application will open a dialog to let users
 * input the image url.
 * When click the determine button, the application will begin to download the image.
 */
void MainWindow::openDownloadDialog()
{
    QInputDialog dialog(this);
    dialog.setWindowTitle(tr("图片URL"));
    dialog.setLabelText(tr("输入图片URL（即链接）"));
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setOkButtonText(tr("确定"));
    dialog.setCancelButtonText(tr("取消"));

    Qt::WindowFlags flag = Qt::Dialog;
    flag |= Qt::WindowCloseButtonHint;
    dialog.setWindowFlags(flag);

    dialog.resize(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6);

    if(dialog.exec() == QInputDialog::Accepted) {
        beginDownload(dialog.textValue());
    }
}

/**
 * @brief MainWindow::openDownloadImage
 *
 * It's a slot function.
 * When the image downloads and is saved successfully, the downloader will send a signal,
 * and trigger the slot function, to show the download image and create the color board of it.
 * Before load the image, delete the downloader, downloadThead, progressDialog, in order to avoid
 * the memory leak.
 */
void MainWindow::openDownloadImage(QString fileName)
{
    delete downloader;

    downloadThread->exit();
    downloadThread->wait();
    delete downloadThread;

    progressDialog->close();
    delete progressDialog;

    showNewSelectedImage(fileName);
    createNewSelectedImageColorBoard();
}

/**
 * @brief MainWindow::openOpenImageFailedMessageBox
 *
 * It's a slot function.
 * If opening image failed, create a messagebox to show that open failed.
 */
void MainWindow::openOpenImageFailedMessageBox()
{
    QMessageBox openImageFailedMessageBox(this);
    openImageFailedMessageBox.setText(tr("打开图片失败。"));
    openImageFailedMessageBox.setInformativeText(tr("可能是图片格式错误，请重试。"));
    openImageFailedMessageBox.setIcon(QMessageBox::Critical);

    openImageFailedMessageBox.exec();
}

/**
 * @brief MainWindow::openDownloadImageFailedMessageBox
 *
 * It's a slot function.
 * If download failed, the download thread will send a signal to main thread, and trigger this
 * function, to create a messagebox to show that download failed.
 */
void MainWindow::openDownloadImageFailedMessageBox()
{
    delete downloader;

    downloadThread->exit();
    downloadThread->wait();
    delete downloadThread;

    progressDialog->close();
    delete progressDialog;

    QMessageBox openImageFailedMessageBox(this);
    openImageFailedMessageBox.setText(tr("下载图片失败。"));
    openImageFailedMessageBox.setInformativeText(tr("可能是URL错误或网络问题，请重试。"));
    openImageFailedMessageBox.setIcon(QMessageBox::Critical);

    openImageFailedMessageBox.exec();
}

/**
 * @brief MainWindow::updateDownloadProgress
 * @param alreadyDownloadSize the size already downloads.
 * @param totalSize the file size.
 *
 * It's a slot function.
 * When the download is doing, the data reply will send a signal, and trigger the slot function,
 * to update the download progress bar.
 */
void MainWindow::updateDownloadProgress(qint64 alreadyDownloadSize, qint64 totalSize)
{
    QString labelText;
    labelText += "下载图片中..." + QString::number(alreadyDownloadSize / 1000) + "kb / " +
            QString::number(totalSize / 1000) + "kb";
    progressDialog->setLabelText(labelText);
    progressDialog->setValue(static_cast<int>(100.0 * alreadyDownloadSize / totalSize));
}

/**
 * @brief MainWindow::showNewSelectedImage
 * @param curFileName the name of file which will be opened.
 *
 * Show the image in the image container.
 */
void MainWindow::showNewSelectedImage(QString curFileName)
{
    workArea->getImageContainer()->loadImage(curFileName);
}

/**
 * @brief MainWindow::createNewSelectedImageColorBoard
 *
 * create the color board of the new load image.
 */
void MainWindow::createNewSelectedImageColorBoard()
{
    workArea->getColorBoard()->setColorLabels();
}

/**
 * @brief MainWindow::beginDownload
 * @param urlString the image url.
 *
 * Create a progress dialog and a image downloader.
 * The image downloader will be moved to another thread, in order to avoid a several seconds block that
 * after the progress dialog appears and before the progress bar appears.
 * According to the console, I think the serval seconds may be used to initialize the network service.
 */
void MainWindow::beginDownload(QString urlString)
{
    progressDialog = new QProgressDialog(this);
    progressDialog->setWindowTitle(tr("下载图片"));
    progressDialog->setLabelText(tr("下载图片中..."));
    progressDialog->setCancelButtonText(tr("取消"));
    progressDialog->setMinimumDuration(500);

    Qt::WindowFlags flag = Qt::Dialog;
    flag |= Qt::WindowCloseButtonHint;
    progressDialog->setWindowFlags(flag);

    progressDialog->show();
    progressDialog->setValue(0);

    downloadThread = new QThread();
    downloader = new ImageDownloader(this);
    downloader->moveToThread(downloadThread);

    connect(this, SIGNAL(beginDownLoadSignal(QString)),
            downloader, SLOT(beginDownload(QString)), Qt::QueuedConnection);

    downloadThread->start();

    emit beginDownLoadSignal(urlString);

    connect(downloader, SIGNAL(downloadFinishedAndSavedSignal(QString)), this,
            SLOT(openDownloadImage(QString)), Qt::QueuedConnection);
    connect(downloader, SIGNAL(downloadImageFailedSignal()),
            SLOT(openDownloadImageFailedMessageBox()), Qt::QueuedConnection);
}

/**
 * @brief MainWindow::connectSlots
 * connect a lot of signals and slots(not all).
 */
void MainWindow::connectSlots()
{
    connect(workArea->getImageContainer(),
            SIGNAL(showScaleRatioChangeSignal(double)),
            SLOT(setShowScaleRatioLabelText(double)));
    connect(workArea->getImageContainer(),
            SIGNAL(cursorInImageSignal(int,int,QString&)),
            SLOT(setCurInfoLabelText(int,int,QString&)));
    connect(workArea->getImageContainer(),
            SIGNAL(cursorInImageSignal(QColor&)),
            SLOT(setColorValueLabel(QColor&)));
    connect(workArea->getImageContainer(),
            SIGNAL(cursorInImageSignal()),
            SLOT(setHelpTextLabelCursorInImage()));
    connect(workArea->getImageContainer(),
            SIGNAL(cursorOutImageSignal()),
            SLOT(setHelpTextLabelCursorOutImage()));
    connect(workArea->getColorBoard(),
            SIGNAL(copySuccessFromColorBoradSignal()),
            SLOT(setHelpTextLabelCopySuccess()));
    connect(workArea->getImageContainer(),
            SIGNAL(copySuccessFromImageLabelSignal()),
            SLOT(setHelpTextLabelCopySuccess()));
    connect(workArea->getImageContainer(),
            SIGNAL(imageFileChangeSignal(QString)),
            SLOT(setFileInfoLabelText(QString)));
    connect(workArea->getImageContainer(),
            SIGNAL(openImageFailedSignal()),
            SLOT(openOpenImageFailedMessageBox()));

    connect(openImageByLocalAction,
            SIGNAL(triggered()),
            SLOT(openFileDialog()));
    connect(openImageByUrlAction,
            SIGNAL(triggered()),
            SLOT(openDownloadDialog()));
}

/**
 * @brief MainWindow::setShowScaleRatioLabelText
 * @param showScaleRatio the show scale ratio depends on the mouse wheel.
 *
 * It's a slot function.
 * When the users scroll the mouse wheel, the show scale ratio changes, send a signal, trigger
 * the function to change the ratio text in status bar.
 */
void MainWindow::setShowScaleRatioLabelText(double showScaleRatio)
{
    int percent = static_cast<int>(showScaleRatio * 100);
    QString text;
    text = text + "缩放：" + QString::number(percent) + "%";

    showScaleRatioLabel->setText(text);
}

/**
 * @brief MainWindow::setCurInfoLabelText
 * @param x the mouse cursor x pos.
 * @param y the mouse cursor y pos.
 * @param color the color of the point which mouse cursor points.
 *
 * It's a slot function.
 * When the mouse cursor is hovered in the image, the image container will send a signal, trigger
 * the function to change the information in status bar.
 */
void MainWindow::setCurInfoLabelText(int x, int y, QString &color)
{
    QString info;
    info = info + "(x: " + QString::number(x) + ", y: " + QString::number(y) + "), " + color;

    curInfoLabel->setText(info);
}

/**
 * @brief MainWindow::setColorValueLabel
 * @param color the color of the point which mouse cursor points.
 *
 * It's a slot function.
 * When the mouse cursor is hovered in the image, the image container will send a signal, trigger
 * the function to change the color label background color in status bar.
 */
void MainWindow::setColorValueLabel(QColor &color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    colorValueLabel->setAutoFillBackground(true);
    colorValueLabel->setPalette(palette);
}

/**
 * @brief MainWindow::setHelpTextLabelCursorInImage
 *
 * It's a slot function.
 * When the mouse cursor is hovered in the image, the image container will send a signal, trigger
 * the function to change the help text in status bar and prompt users that can double click the
 * image to copy the color value to the clipboard.
 */
void MainWindow::setHelpTextLabelCursorInImage()
{
    helpTextLabel->setText(tr("双击将鼠标所指点颜色复制到剪贴板。"));
    helpTextLabel->setStyleSheet("color: black");
}

/**
 * @brief MainWindow::setHelpTextLabelCursorOutImage
 *
 * It's a slot function.
 * When the mouse cursor isn't hovered in the image, the image container will send a signal, trigger
 * the function to change the help text in status bar and prompt users that can click the color label
 * in color board to copy the color value to the clipboard.
 */
void MainWindow::setHelpTextLabelCursorOutImage()
{
    helpTextLabel->setText(tr("单击右侧色板将颜色复制到剪贴板。"));
    helpTextLabel->setStyleSheet("color: black");
}

/**
 * @brief MainWindow::setHelpTextLabelCopySuccess
 *
 * It's a slot function.
 * When the mouse double click in the color label in the color board, the color board will send a signal,
 * trigger the function to change the help text in status bar and prompt users that copy successfully.
 */
void MainWindow::setHelpTextLabelCopySuccess()
{
    helpTextLabel->setText(tr("复制颜色成功！"));
    helpTextLabel->setStyleSheet("color: green");
}

/**
 * @brief MainWindow::setFileInfoLabelText
 * @param info the file name and size.
 *
 * It's a slot function.
 * When the image loads, the image container will send a signal, trigger the function to change the
 * label text in status bar and prompt users the file name and size.
 */
void MainWindow::setFileInfoLabelText(QString info)
{
    fileInfoLabel->setText(info);
}
