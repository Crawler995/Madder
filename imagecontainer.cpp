#include "imagecontainer.h"
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QPixmap>
#include <QImage>
#include <QWheelEvent>
#include <QGraphicsDropShadowEffect>
#include <QEvent>

ImageContainer::ImageContainer(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    scaleFactor = 0.05;

    setMouseTracking(true);

    imageArea = new QScrollArea(this);
    layout->addWidget(imageArea);
    setLayout(layout);

    imageLabel = new QLabel(this);
    QImage *m = new QImage("E:/coding/Qt/Madder/a.jpg");
    imageLabel->setPixmap(QPixmap::fromImage(*m));

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
    imageAreaWidth = imageArea->viewport()->geometry().width();
    imageAreaHeight = imageArea->viewport()->geometry().height();

    fileIntoContainerScaleRatio = 0.95 * imageAreaWidth / imageLabel->pixmap()->size().width();
    imageLabel->resize(fileIntoContainerScaleRatio * showScaleRatio * imageLabel->pixmap()->size());
}

bool ImageContainer::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == imageLabel) {
        if(event->type() == QEvent::MouseMove) {
            QString color("#520520");
            QMouseEvent *e = static_cast<QMouseEvent*>(event);
            emit cursorInImageSignal(e->pos().x(), e->pos().y(), color);
        }
    }
}
