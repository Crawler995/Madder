#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QWheelEvent>

class ImageContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageContainer(QWidget *parent = 0);
protected:
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QScrollArea *imageArea;
    QLabel *imageLabel;
    double fileIntoContainerScaleRatio, showScaleRatio, scaleFactor;
    int imageAreaWidth, imageAreaHeight;

signals:
    void showScaleRatioChangeSignal(double showScaleRatio);
    void cursorInImageSignal(int x, int y, QString &color);
};

#endif // IMAGECONTAINER_H
