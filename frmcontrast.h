#ifndef FRMCONTRAST_H
#define FRMCONTRAST_H

#include <QMainWindow>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

#include <QImage>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include "imageviewer.h"
//#include "C:\Users\Sealkeen\Downloads\Program\OpenCV4.5Windows\opencv\build\include\opencv2\opencv.hpp"
#include "histogramWnd.h"
#include "projectwindows.h"

class FrmContrast : public QMainWindow
{
    Q_OBJECT

public:
    void InitializeComponent();
    FrmContrast(ImageViewer *ImgV);

    void CreateHistogram();
    void SetFactor();
    void EnableLocalArea();
    void ShowMainWindow();
    //QSlider *sliderBar;
    QPushButton *btnSetContrast;
    QPushButton *btnShowHistogram;
    QDoubleSpinBox *numericField;
    QCheckBox *chkLocalArea;
    QLabel *lblValue;
    QLabel *lblCoord;
    QLabel *lblWidthHeight;
    QLabel *lblAdaptiveContrast;
    QSpinBox *numX; QSpinBox *localAreaWidth;
    QSpinBox *numY; QSpinBox *localAreaHeight;
    QRadioButton *rdbFactorContrast;
    QRadioButton *rdbLocalContrast;
    QRadioButton *rdbAdaptiveContrast;
    ImageViewer *imageViewer;
    HistogramWnd *histogramWnd;
    QSpinBox *spnbxAdaptiveWidth;
    QPushButton *btnShowMainWindow;

//private slots:

private:
#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

};

#endif // FRMCONTRAST_H
