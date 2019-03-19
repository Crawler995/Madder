#include "colorboard.h"
#include "colorlabel.h"
#include <QLabel>
#include <QGridLayout>
#include <QVector>
#include <QGraphicsEffect>

ColorBoard::ColorBoard(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    computeMainColor();

    QLabel *text = new QLabel(tr("图片主色"), this);
    text->setAlignment(Qt::AlignCenter);
    layout->addWidget(text, 0, 0, 1, 3);

    QVector<QColor>::const_iterator it;
    for(it = colors.constBegin(); it != colors.constEnd(); it++) {
        int index = it - colors.begin();

        ColorLabel *colorLabel = new ColorLabel(*it, this);

        colorLabels.push_back(colorLabel);
        layout->addWidget(colorLabel, index * 3 + 1, 0, 3, 2);

        connect(colorLabel, SIGNAL(copySuccessSignalFromColorLabelSignal()),
                SLOT(sendCopySuccessSignal()));

        QLabel *colorValueLabel = new QLabel(this);
        QString colorValue;
        colorValue += "#";
        colorValue += decToHexString((*it).red());
        colorValue += decToHexString((*it).green());
        colorValue += decToHexString((*it).blue());

        colorValueLabel->setText(colorValue);
        colorValueLabel->setAlignment(Qt::AlignCenter);
        colorValueLabels.push_back(colorValueLabel);

        layout->addWidget(colorValueLabel, index * 3 + 1, 2, 3, 1);
    }

    layout->setColumnMinimumWidth(0, 50);
    setLayout(layout);
}

QVector<ColorLabel *> ColorBoard::getColorLabels() const
{
    return colorLabels;
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

void ColorBoard::sendCopySuccessSignal()
{
    emit copySuccessFromColorBoradSignal();
}
