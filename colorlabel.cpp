#include "colorlabel.h"
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QClipboard>
#include <QApplication>

ColorLabel::ColorLabel(QColor color, QWidget *parent) : QWidget(parent)
{
    colorLabel = new QLabel(this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(colorLabel);
    setLayout(layout);

    setColor(color);

    QGraphicsDropShadowEffect *colorLabelShadow = new QGraphicsDropShadowEffect(this);
    colorLabelShadow->setBlurRadius(16.0);
    colorLabelShadow->setColor(QColor(0, 0, 0, 100));
    colorLabelShadow->setOffset(3.0);
    colorLabel->setGraphicsEffect(colorLabelShadow);
}

void ColorLabel::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);
}

void ColorLabel::mousePressEvent(QMouseEvent *event)
{
    emit copySuccessSignalFromColorLabelSignal();

    QClipboard *clipBoard = QApplication::clipboard();

    QString colorValue;
    colorValue += "#" + decToHexString(color.red()) + decToHexString(color.green()) +
            decToHexString(color.blue());

    clipBoard->setText(colorValue);
}

void ColorLabel::setColor(QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    colorLabel->setAutoFillBackground(true);
    colorLabel->setPalette(palette);

    this->color = color;
}

QString ColorLabel::decToHexString(int value)
{
    QString res;
    if(value <= 0x0f) {
        res += "0";
    }
    res += QString::number(value, 16);

    return res;
}
