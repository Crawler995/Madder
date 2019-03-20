#include "mainwindow.h"
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
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(960, 540);
    createMenu(this);
    createStatusBar(this);
    createWorkArea(this);
    createToolBar(this);

    setMouseTracking(true);
    workArea->setMouseTracking(true);

    connectSlots();
}

MainWindow::~MainWindow()
{

}

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

void MainWindow::createStatusBar(QMainWindow *mainWindow)
{
    statusBar = mainWindow->statusBar();

    fileInfoLabel = new QLabel(tr(""), statusBar);
    curInfoLabel = new QLabel(tr(""), statusBar);
    showScaleRatioLabel = new QLabel(tr("缩放：100%"), statusBar);
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

void MainWindow::createToolBar(QMainWindow *mainWindow)
{
    toolBar = new QToolBar(this);
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

void MainWindow::createWorkArea(QMainWindow *mainWindow)
{
    workArea = new WorkArea(mainWindow);
    mainWindow->setCentralWidget(workArea);
}

void MainWindow::openFileDialog()
{
    curFileName = QFileDialog::getOpenFileName(this, tr("打开图片"), "E:/waste",
                                               tr("Images((*.png *.jpg *.jped *.bmp))"));
    if(curFileName == "") {
        return;
    }
    emit imageFileChangeSignal(curFileName);
    emit imageFileChangeSignal();
}

void MainWindow::showNewSelectedImage(QString curFileName)
{
    workArea->getImageContainer()->loadImage(curFileName);
}

void MainWindow::createNewSelectedImageColorBoard()
{
    workArea->getColorBoard()->setColorLabels();
}

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

    connect(openImageByLocalAction,
            SIGNAL(triggered()),
            SLOT(openFileDialog()));
    connect(this, SIGNAL(imageFileChangeSignal(QString)),
            SLOT(showNewSelectedImage(QString)));
    connect(this, SIGNAL(imageFileChangeSignal()),
            SLOT(createNewSelectedImageColorBoard()));
}

void MainWindow::setShowScaleRatioLabelText(double showScaleRatio)
{
    int percent = static_cast<int>(showScaleRatio * 100);
    QString text;
    text = text + "缩放：" + QString::number(percent) + "%";

    showScaleRatioLabel->setText(text);
}

void MainWindow::setCurInfoLabelText(int x, int y, QString &color)
{
    QString info;
    info = info + "(x: " + QString::number(x) + ", y: " + QString::number(y) + "), " + color;

    curInfoLabel->setText(info);
}

void MainWindow::setColorValueLabel(QColor &color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    colorValueLabel->setAutoFillBackground(true);
    colorValueLabel->setPalette(palette);
}

void MainWindow::setHelpTextLabelCursorInImage()
{
    helpTextLabel->setText(tr("双击将鼠标所指点颜色复制到剪贴板。"));
    helpTextLabel->setStyleSheet("color: black");
}

void MainWindow::setHelpTextLabelCursorOutImage()
{
    helpTextLabel->setText(tr("单击右侧色板将颜色复制到剪贴板。"));
    helpTextLabel->setStyleSheet("color: black");
}

void MainWindow::setHelpTextLabelCopySuccess()
{
    helpTextLabel->setText(tr("复制颜色成功！"));
    helpTextLabel->setStyleSheet("color: green");
}

void MainWindow::setFileInfoLabelText(QString info)
{
    fileInfoLabel->setText(info);
}
