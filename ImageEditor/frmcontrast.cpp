#include "frmcontrast.h"

int PlaceDown(int& vai, int offset = 24)
{
    vai += offset;
    return vai;
}

void FrmContrast::InitializeComponent()
{
    Qt::WindowFlags flags;
    flags |= Qt::MSWindowsFixedSizeDialogHint;
    setWindowFlags(flags);

    //QGridLayout *layout = new QGridLayout(this);

    //this->setVisible(true);

    //sliderBar = new QSlider(this);
    btnSetContrast = new QPushButton(this); btnSetContrast->setText("Set Contrast");
    btnShowHistogram = new QPushButton(this); btnShowHistogram->setText("Show Histogram");
    numericField = new QDoubleSpinBox(this);
    chkLocalArea = new QCheckBox(this); chkLocalArea->setText("Local Area");
    lblValue = new QLabel(this); lblValue->setText("Contrast factor");
    lblCoord = new QLabel(this); lblCoord->setText("Coordinates (x, y)");
    lblAdaptiveContrast = new QLabel(this); lblAdaptiveContrast->setText("Adaptive Contrast Square Height");
    lblWidthHeight = new QLabel(this);
    numX = new QSpinBox(this); numY = new QSpinBox(this);
    localAreaWidth = new QSpinBox(this);
    localAreaHeight = new QSpinBox(this);
    rdbFactorContrast = new QRadioButton(this); rdbFactorContrast->setText("Set Factor Contrast");
    rdbLocalContrast = new QRadioButton(this); rdbLocalContrast->setText("Set Local Contrast");
    rdbAdaptiveContrast = new QRadioButton(this); rdbAdaptiveContrast->setText("Set Adaptive Contrast");
    spnbxAdaptiveWidth = new QSpinBox(this);
    btnShowMainWindow = new QPushButton(this); btnShowMainWindow->setText("Show Main Window");

    int YCoord = 0;
    //sliderBar->setGeometry(10, 0, 120, 50);
    lblValue->setGeometry(10, YCoord, 200, 18);
    numericField->setGeometry(10, PlaceDown(YCoord, 26), 80, 20);
    btnSetContrast->setGeometry(10, PlaceDown(YCoord, 26), 150, 26);

    chkLocalArea->move(10, PlaceDown(YCoord)); //setGeometry(10, 76, 180, 18);
    lblCoord->setGeometry(10, PlaceDown(YCoord), 110, 20);
    numX->setGeometry(10, PlaceDown(YCoord), 90, 20);
    numY->setGeometry(132, YCoord, 90, 20);
    localAreaWidth->setGeometry(10, PlaceDown(YCoord, 50), 90, 20);
    localAreaHeight->setGeometry(10, PlaceDown(YCoord), 90, 20);
    numX->setMaximum(10000); localAreaWidth->setMaximum(10000);
    numY->setMaximum(10000); localAreaHeight->setMaximum(10000);

    rdbFactorContrast->setGeometry(10, PlaceDown(YCoord), 200, 20);
    rdbLocalContrast->setGeometry(10, PlaceDown(YCoord), 200, 20);
    rdbAdaptiveContrast->setGeometry(10, PlaceDown(YCoord), 200, 20);
    btnShowHistogram->setGeometry(10, PlaceDown(YCoord), 110, 20);

    lblAdaptiveContrast->setGeometry(10, PlaceDown(YCoord), 110, 20);
    spnbxAdaptiveWidth->move(10, PlaceDown(YCoord));
    btnShowMainWindow->move(10, PlaceDown(YCoord));

    numericField->setMinimum( -10000000.0 );

    this->setGeometry(0, 35, 300, 400);
    this->setWindowTitle("Contrast");

    connect(btnSetContrast, &QPushButton::released, this, &FrmContrast::SetFactor);
    connect(chkLocalArea, &QCheckBox::toggled, this, &FrmContrast::EnableLocalArea);
    connect(btnShowHistogram, &QPushButton::released, this, &FrmContrast::CreateHistogram);
    connect(btnShowMainWindow, &QPushButton::released, this, &FrmContrast::ShowMainWindow);
    EnableLocalArea();

    ProjectWide::g_fContrast = (void*)this;

    ((QMainWindow*)ProjectWide::g_iV)->move(
                this->x()
               + this->width()
               , this->y());
}
void FrmContrast::ShowMainWindow()
{
    imageViewer->raise();
}

void FrmContrast::CreateHistogram()
{
    ProjectWide::oneLinePaint = false;
    QImage *pImg = (imageViewer->newImage.isNull()) ?
            &imageViewer->image :
            &imageViewer->newImage;
    imageViewer->GatherLevels(pImg, (int)(pImg->height()/2));

    for (int i = 0; i < 256; i++)
    {
        histogramWnd->renderArea->RGBLevels[i] =
                (int)imageViewer->RGBLevels[i];
    }
}

void FrmContrast::EnableLocalArea()
{
    bool checkState = chkLocalArea->isChecked() ? true : false;

    lblCoord->setEnabled(checkState);
    numX->setEnabled(checkState);
    numY->setEnabled(checkState);
    localAreaWidth->setEnabled(checkState);
    localAreaHeight->setEnabled(checkState);
}

FrmContrast::FrmContrast(ImageViewer *imgV)
{
    imageViewer = imgV;
    InitializeComponent();
}

void FrmContrast::SetFactor()
{
    imageViewer->contrastFactor = this->numericField->value();
    if ( rdbAdaptiveContrast->isChecked() )
        imageViewer->AdaptiveContrast();
    else if ( chkLocalArea->isChecked() )
        imageViewer->ChangeContrast( numX->value(), numY->value(),
                    localAreaWidth->value(), localAreaHeight->value());
    else
        imageViewer->ChangeContrast();

    //lblValue->setText("QString(this->sliderBar->value())");

    QString message("Trying to change the contrast by " + QString::number(numericField->value()));
    lblValue->setText(message);
}
