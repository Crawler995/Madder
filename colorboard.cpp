#include "colorboard.h"
#include <QLabel>
#include <QGridLayout>
#include <QVector>
#include <QGraphicsEffect>

ColorBoard::ColorBoard(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    computeMainColor();

    QVector<QColor>::const_iterator it;
    for(it = colors.constBegin(); it != colors.constEnd(); it++) {
        int index = it - colors.begin();

        QLabel *colorLabel = new QLabel(this);

        QPalette palette;
        palette.setColor(QPalette::Background, *it);
        colorLabel->setAutoFillBackground(true);
        colorLabel->setPalette(palette);

        QGraphicsDropShadowEffect *colorLabelShadow = new QGraphicsDropShadowEffect(this);
        colorLabelShadow->setBlurRadius(20.0);
        colorLabelShadow->setColor(QColor(0, 0, 0, 160));
        colorLabelShadow->setOffset(3.0);
        colorLabel->setGraphicsEffect(colorLabelShadow);

        colorLabels.push_back(colorLabel);
        layout->addWidget(colorLabel, index, 0, 1, 2);

        QLabel *colorValueLabel = new QLabel(this);
        QString colorValue;
        colorValue += "#";
        colorValue += decToHexString((*it).red());
        colorValue += decToHexString((*it).green());
        colorValue += decToHexString((*it).blue());

        colorValueLabel->setText(colorValue);
        colorValueLabel->setAlignment(Qt::AlignCenter);
        colorValueLabels.push_back(colorValueLabel);

        layout->addWidget(colorValueLabel, index, 2, 1, 1);
    }

    layout->setColumnMinimumWidth(0, 50);
    setLayout(layout);
}

QString ColorBoard::decToHexString(int value)
{
    QString res;
    if(value <= 0x0f) {
        res += "0";
    }
    res += QString::number(value, 16);

    return res;
}

void ColorBoard::computeMainColor()
{
    colors.push_back(QColor("#2c1213"));
    colors.push_back(QColor("#910601"));
    colors.push_back(QColor("#fefdeb"));
    colors.push_back(QColor("#b41100"));
    colors.push_back(QColor("#601a1c"));
}
