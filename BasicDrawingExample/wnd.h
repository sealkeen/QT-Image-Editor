#ifndef WND_H
#define WND_H

#include <QWidget>
#include <QPainter>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include "renderarea.h"
#include <QGridLayout>

namespace Ui {
    class HistogramWnd;
}

class Wnd : public QWidget
{
    Q_OBJECT

public:
    explicit Wnd(QWidget *parent = 0);
    ~Wnd();

private slots:
    void shapeChanged();
    void penChanged();
    void brushChanged();

private:
    RenderArea *renderArea;
    QLabel *shapeLabel;
    QLabel *penWidthLabel;
    QLabel *penStyleLabel;
    QLabel *penCapLabel;
    QLabel *penJoinLabel;
    QLabel *brushStyleLabel;
    QLabel *otherOptionsLabel;
    QComboBox *shapeComboBox;
    QSpinBox *penWidthSpinBox;
    QComboBox *penStyleComboBox;
    QComboBox *penCapComboBox;
    QComboBox *penJoinComboBox;
    QComboBox *brushStyleComboBox;
    QCheckBox *antialiasingCheckBox;
    QCheckBox *transformationsCheckBox;
    Ui::HistogramWnd *ui;
};

#endif // WND_H
