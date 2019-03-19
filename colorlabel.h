#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QWidget>
#include <QColor>
#include <QLabel>

class ColorLabel : public QWidget
{
    Q_OBJECT
public:
    explicit ColorLabel(QColor color, QWidget *parent = 0);
protected:
    void enterEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QLabel *colorLabel;
    QColor color;

    void setColor(QColor color);
    QString decToHexString(int value);
signals:
    void copySuccessSignalFromColorLabelSignal();
};

#endif // COLORLABEL_H
