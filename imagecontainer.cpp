#include "imagecontainer.h"
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QPixmap>
#include <QImage>
#include <QWheelEvent>
#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QToolTip>
#include <QApplication>
#include <QClipboard>
#include <QtAlgorithms>
#include <QFileInfo>
#include <QDebug>

ImageContainer::ImageContainer(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    scaleFactor = 0.05;

    setMouseTracking(true);

    imageArea = new QScrollArea(this);
    layout->addWidget(imageArea);
    setLayout(layout);

    imageLabel = new QLabel(this);
    imageLabel->resize(0, 0);

    image = nullptr;

    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->installEventFilter(this);
    imageLabel->setMouseTracking(true);

    QGraphicsDropShadowEffect *imageLabelShadow = new QGraphicsDropShadowEffect(this);
    imageLabelShadow->setBlurRadius(40.0);
    imageLabelShadow->setColor(QColor(0, 0, 0, 160));
    imageLabelShadow->setOffset(8.0);
    imageLabel->setGraphicsEffect(imageLabelShadow);

    fileIntoContainerScaleRatio = 1.0;
    showScaleRatio = 1.0;

    imageArea->setAlignment(Qt::AlignCenter);
    imageArea->setWidget(imageLabel);
}

void ImageContainer::wheelEvent(QWheelEvent *event)
{
    if(image == nullptr) {
        return;
    }

    if(event->delta() > 0) {
        if(showScaleRatio + scaleFactor > 2) {
            return;
        }
        showScaleRatio += scaleFactor;
        imageLabel->resize(fileIntoContainerScaleRatio * showScaleRatio * imageLabel->pixmap()->size());
    }
    else {
        if(showScaleRatio - scaleFactor < 0.5) {
            return;
        }
        showScaleRatio -= scaleFactor;
        imageLabel->resize(fileIntoContainerScaleRatio * showScaleRatio * imageLabel->pixmap()->size());
    }

    emit showScaleRatioChangeSignal(showScaleRatio);
}

void ImageContainer::resizeEvent(QResizeEvent *event)
{
    if(image == nullptr) {
        return;
    }
    imageAreaWidth = imageArea->viewport()->geometry().width();
    imageAreaHeight = imageArea->viewport()->geometry().height();

    computeFileIntoContainerScaleRatio();
    imageLabel->resize(fileIntoContainerScaleRatio * showScaleRatio * imageLabel->pixmap()->size());
}

bool ImageContainer::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == imageLabel) {
        if(image == nullptr) {
            return true;
        }
        if(event->type() == QEvent::MouseMove) {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);
            QString colorValue;
            QColor color = getPixelColor(e->pos().x(), e->pos().y());

            colorValue += "#" + decToHexString(color.red()) + decToHexString(color.green()) +
                    decToHexString(color.blue());

            int x = e->pos().x() / showScaleRatio / fileIntoContainerScaleRatio;
            int y = e->pos().y() / showScaleRatio / fileIntoContainerScaleRatio;

            emit cursorInImageSignal(x, y, colorValue);
            emit cursorInImageSignal(color);
        }
        else if(event->type() == QEvent::Enter) {
            emit cursorInImageSignal();
        }
        else if(event->type() == QEvent::Leave) {
            emit cursorOutImageSignal();
        }
        else if(event->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);
            QString colorValue;
            QColor color = getPixelColor(e->pos().x(), e->pos().y());

            colorValue += "#" + decToHexString(color.red()) + decToHexString(color.green()) +
                    decToHexString(color.blue());
            QClipboard *clipBoard = QApplication::clipboard();
            clipBoard->setText(colorValue);

            emit copySuccessFromImageLabelSignal();
        }
    }
}

QLabel *ImageContainer::getImageLabel() const
{
    return imageLabel;
}

double ImageContainer::getShowScaleRatio() const
{
    return showScaleRatio;
}

double ImageContainer::getFileIntoContainerScaleRatio() const
{
    return fileIntoContainerScaleRatio;
}

QColor ImageContainer::getPixelColor(int x, int y)
{
    x /= fileIntoContainerScaleRatio * showScaleRatio;
    y /= fileIntoContainerScaleRatio * showScaleRatio;

    return image->pixelColor(x, y);
}

QString ImageContainer::decToHexString(int value)
{
    QString res;
    if(value <= 0x0f) {
        res += "0";
    }
    res += QString::number(value, 16);

    return res;
}

void ImageContainer::computeFileIntoContainerScaleRatio()
{
    int imageWidth = imageLabel->pixmap()->size().width();
    int imageHeight = imageLabel->pixmap()->size().height();
    double factor = 0.0;
    double x = 1.0 * imageAreaWidth / imageWidth;
    double y = 1.0 * imageAreaHeight / imageHeight;

    factor = qMin(x, y);
    fileIntoContainerScaleRatio = 0.95 * factor;
}

void ImageContainer::loadImage(QString fileName)
{
    if(image != nullptr) {
        delete image;
    }
    image = new QImage(fileName);
    imageLabel->setPixmap(QPixmap::fromImage(*image));

    imageAreaWidth = imageArea->viewport()->geometry().width();
    imageAreaHeight = imageArea->viewport()->geometry().height();

    computeFileIntoContainerScaleRatio();
    imageLabel->resize(fileIntoContainerScaleRatio * showScaleRatio * imageLabel->pixmap()->size());

    QFileInfo fi(fileName);
    QString info;

    info += fi.fileName() + ", ";
    info += QString::number(imageLabel->pixmap()->size().width()) + "*";
    info += QString::number(imageLabel->pixmap()->size().height());

    emit imageFileChangeSignal(info);
}
