#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>

#include "workarea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QMenuBar *menuBar;
    QMenu *fileMenu, *openImageMenu, *openHistoryImageMenu, *settingMenu, *saveColorBoardMenu, *aboutMenu;
    QAction *openImageByLocalAction, *openImageByUrlAction, *saveAsTxtAction, *saveAsJpgAction,
             *restartAction, *exitAction, *preferenceAction, *referenceAction, *authorAction;
    QToolBar *toolBar;
    QStatusBar *statusBar;
    QLabel *fileInfoLabel, *curInfoLabel, *showScaleRatioLabel;
    WorkArea *workArea;

    void createMenu(QMainWindow *mainWindow);
    void createStatusBar(QMainWindow *mainWindow);
    void createToolBar(QMainWindow *mainWindow);
    void createWorkArea(QMainWindow *mainWindow);

public slots:
    void setShowScaleRatioLabelText(double showScaleRatio);
    void setCurInfoLabelText(int x, int y, QString &color);
};

#endif // MAINWINDOW_H
