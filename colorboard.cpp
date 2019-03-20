#include "colorboard.h"
#include "colorlabel.h"
#include <QLabel>
#include <QGridLayout>
#include <QVector>
#include <QGraphicsEffect>

ColorBoard::ColorBoard(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout(this);

    text = new QLabel(tr("先打开一张图片。"), this);
    text->setAlignment(Qt::AlignCenter);
    layout->addWidget(text, 0, 0, 1, 3);
}

QVector<ColorLabel *> ColorBoard::getColorLabels() const
{
    return colorLabels;
}

void ColorBoard::setColorLabels()
{
    if(colors.empty()) {
        createColorLabels();
    }
    else {
        changeColorLabels();
    }
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
    colors.push_back(QColor("#feb3b0"));
    colors.push_back(QColor("#3b3c5a"));
    colors.push_back(QColor("#0d0e23"));
    colors.push_back(QColor("#706182"));
    colors.push_back(QColor("#fce4fc"));
    colors.push_back(QColor("#706182"));
    colors.push_back(QColor("#706182"));
}

void ColorBoard::createColorLabels()
{
    text->setText(tr("图片主色"));

    computeMainColor();

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

void ColorBoard::changeColorLabels()
{
    colors.clear();
    computeMainColor();

    if(colorLabels.size() == colors.size()) {
        QVector<QColor>::const_iterator it;
        for(it = colors.constBegin(); it != colors.constEnd(); it++) {
            int index = it - colors.constBegin();

            QString colorValue;
            colorValue += "#";
            colorValue += decToHexString((*it).red());
            colorValue += decToHexString((*it).green());
            colorValue += decToHexString((*it).blue());

            colorValueLabels[index]->setText(colorValue);

            colorLabels[index]->setColor(*it);
        }
    }
}

void ColorBoard::sendCopySuccessSignal()
{
    emit copySuccessFromColorBoradSignal();
}
