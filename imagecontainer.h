#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QWheelEvent>
#include <QColor>
#include <QImage>
#include <QMouseEvent>

class ImageContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageContainer(QWidget *parent = 0);
    double getFileIntoContainerScaleRatio() const;
    double getShowScaleRatio() const;
    QLabel *getImageLabel() const;

protected:
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QScrollArea *imageArea;
    QImage *image;
    QLabel *imageLabel;
    double fileIntoContainerScaleRatio, showScaleRatio, scaleFactor;
    int imageAreaWidth, imageAreaHeight;

    QColor getPixelColor(int x, int y);
    QString decToHexString(int value);

signals:
    void showScaleRatioChangeSignal(double showScaleRatio);
    void cursorInImageSignal(int x, int y, QString &color);
    void cursorInImageSignal(QColor &color);
    void cursorInImageSignal();
};

#endif // IMAGECONTAINER_H
