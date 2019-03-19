#ifndef COLORBOARD_H
#define COLORBOARD_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QVector>

class ColorBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBoard(QWidget *parent = 0);
private:
    QGridLayout *layout;
    QVector<QColor> colors;
    QVector<QLabel*> colorLabels, colorValueLabels;

    QString decToHexString(int value);
    void computeMainColor();
};

#endif // COLORBOARD_H
