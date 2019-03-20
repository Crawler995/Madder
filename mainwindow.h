#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>
#include <QMouseEvent>

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
    QLabel *fileInfoLabel, *curInfoLabel, *showScaleRatioLabel, *colorValueLabel, *helpTextLabel;
    WorkArea *workArea;
    QString curFileName;

    void createMenu(QMainWindow *mainWindow);
    void createStatusBar(QMainWindow *mainWindow);
    void createToolBar(QMainWindow *mainWindow);
    void createWorkArea(QMainWindow *mainWindow);

    void connectSlots();

public slots:
    void setShowScaleRatioLabelText(double showScaleRatio);
    void setCurInfoLabelText(int x, int y, QString &color);
    void setColorValueLabel(QColor &color);
    void setHelpTextLabelCursorInImage();
    void setHelpTextLabelCursorOutImage();
    void setHelpTextLabelCopySuccess();
    void setFileInfoLabelText(QString info);

    void openFileDialog();
    void showNewSelectedImage(QString curFileName);
    void createNewSelectedImageColorBoard();

signals:
    void imageFileChangeSignal(QString curFileName);
    void imageFileChangeSignal();
};

#endif // MAINWINDOW_H
