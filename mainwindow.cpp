#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>
#include <QToolBar>

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

    connect(workArea->getImageContainer(),
            SIGNAL(showScaleRatioChangeSignal(double)),
            SLOT(setShowScaleRatioLabelText(double)));
    connect(workArea->getImageContainer(),
            SIGNAL(cursorInImageSignal(int,int,QString&)),
            SLOT(setCurInfoLabelText(int,int,QString&)));
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

    fileInfoLabel = new QLabel(tr("a.jpg, 1920 * 1080"), statusBar);
    curInfoLabel = new QLabel(tr("(x: 244, y: 444), #4f556e"), statusBar);
    showScaleRatioLabel = new QLabel(tr("缩放：100%"), statusBar);

    fileInfoLabel->setAlignment(Qt::AlignCenter);
    curInfoLabel->setAlignment(Qt::AlignCenter);
    showScaleRatioLabel->setAlignment(Qt::AlignCenter);

    statusBar->addPermanentWidget(fileInfoLabel, 1);
    statusBar->addWidget(curInfoLabel, 1);
    statusBar->addWidget(showScaleRatioLabel, 1);
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
