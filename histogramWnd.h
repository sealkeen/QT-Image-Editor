#ifndef WND_H
#define WND_H

#include <QWidget>
#include <QPainter>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include "../BasicDrawingExample/renderarea.h"
#include <QGridLayout>
#include "projectwindows.h"

namespace Ui {
    class HistogramWnd;
}

class HistogramWnd : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramWnd(QWidget *parent = 0);
    ~HistogramWnd();

    RenderArea *renderArea;
private slots:
    void shapeChanged();
    void penChanged();
    void brushChanged();

private:
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
