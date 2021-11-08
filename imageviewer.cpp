
/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif

#include "imageviewer.h"

double ImageViewer::contrastFactor;
QImage ImageViewer::image;
QImage ImageViewer::newImage;
double ImageViewer::onePercentOf255 = 0.0039215686274509803921568627451;

ImageViewer::ImageViewer()
   : imageLabel(new QLabel)
   , scrollArea(new QScrollArea)
     //sliderBar(new QSlider) ,
     //btnSetContrast(new QPushButton) ,
   , scaleFactor(1)
{
    ProjectWide::g_brightnessVector = std::vector<int>();
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);

    //sliderBar->setVisible(true);
    //btnSetContrast->setVisible(true);
    //btnSetContrast->setText(QString("Set Contrast"));
    //btnSetContrast->move(frmContrast->x(),
    //                            frmContrast->y()+frmContrast->height());
    //frmContrast->setVisible(true);
    //frmContrast->
    //frmContrast->setLayout(layout);

    setCentralWidget(scrollArea);

    createActions();
    installEventFilter(this);

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    //CommonStaticClass::g_iV = this;
    //connect(btnSetContrast, &QPushButton::released, this, &ImageViewer::ChangeContrast);
    contrastFactor = invalid_factor;
    isImageSet = false;
}

int Red(QRgb col)
{
    return col & 0xFF;
}
int Green(QRgb col)
{
    return (col << 8) & 0xFF;
}
int Blue(QRgb col)
{
    return (col << 16) & 0xFF;
}
//imageviewer.cpp:115:40: warning: unused parameter 'obj'
bool ImageViewer::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        int yOffset = scrollArea->geometry().y();
        int imageX = mouseEvent->pos().x() - imageLabel->pos().x();
        int imageY = mouseEvent->pos().y() - imageLabel->pos().y() - yOffset;
        statusBar()->showMessage
            (QString("Coordinates clicked : (%1,%2) "
                     "Brightness: (R:%3 G:%4 B:%5)")
             .arg( imageX )
             .arg( imageY )
             .arg( qRed(image.pixel(imageX, imageY)) )
             .arg( qGreen(image.pixel(imageX, imageY)) )
             .arg( qBlue(image.pixel(imageX, imageY)) )
             );
        if (newImage.isNull() || newImage.width() < 2) {
            LineBrightness(&image, imageY);
            QApplication::setActiveWindow((QWidget*)ProjectWide::g_histogramWnd);
            QApplication::setActiveWindow(this);
            return true;
        } else {
            LineBrightness(&newImage, imageY);
            QApplication::setActiveWindow((QWidget*)ProjectWide::g_histogramWnd);
            QApplication::setActiveWindow(this);
            return true;
        }
    }
    return false;
}

bool ImageViewer::loadFile( const QString &fileName )
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr( "Cannot load %1: %2" )
                                 .arg( QDir::toNativeSeparators(fileName), reader.errorString()) );
        return false;
    }

    setImage(newImage);

    setWindowFilePath(fileName);

    if (newImage.width() < 1920 && newImage.height() < 1080)
        this->resize(newImage.width(), newImage.height());
    else
        this->resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
            .arg(QDir::toNativeSeparators(fileName))
            .arg(image.width())
            .arg(image.height())
            .arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}

void ImageViewer::setImage(const QImage &newImg, bool cacheImage)
{
    if(cacheImage)
        image = newImg;
    imageLabel->setPixmap(QPixmap::fromImage(newImg));
    scaleFactor = 1.0;

    scrollArea->setVisible(true);
    printAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
    isImageSet = true;
}

bool ImageViewer::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(newImage)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}


static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void ImageViewer::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
    // edit 2020-11-30 19:28
    //fitToWindowAct->setChecked(true);
    //fitToWindow();
}

void ImageViewer::showContrastWnd()
{
    (
    (QMainWindow*)
            ProjectWide::g_fContrast
    )->raise();
}

void ImageViewer::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::print()
{
    //Q_ASSERT(imageLabel->pixmap());
#ifdef notdefined
    !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}

void ImageViewer::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(image);
#endif // !QT_NO_CLIPBOARD
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
        if (mimeData->hasImage()) {
            const QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull())
                return image;
        }
    }
    return QImage();
}
#endif // !QT_NO_CLIPBOARD

void ImageViewer::paste()
{
#ifndef QT_NO_CLIPBOARD
    const QImage newImage = clipboardImage();
    if (newImage.isNull()) {
        statusBar()->showMessage(tr("No image in clipboard"));
    } else {
        setImage(newImage);
        setWindowFilePath(QString());
        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
            .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
        statusBar()->showMessage(message);
    }
#endif // !QT_NO_CLIPBOARD
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}

void ImageViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}

void ImageViewer::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &ImageViewer::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &ImageViewer::saveAs);
    saveAsAct->setEnabled(false);

    printAct = fileMenu->addAction(tr("&Print..."), this, &ImageViewer::print);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    copyAct = editMenu->addAction(tr("&Copy"), this, &ImageViewer::copy);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setEnabled(false);

    changeContrast = editMenu->addAction(tr("&Set Contrast"), this, &ImageViewer::showContrastWnd);
    changeContrast->setEnabled(true);

    QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &ImageViewer::paste);
    pasteAct->setShortcut(QKeySequence::Paste);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &ImageViewer::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &ImageViewer::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &ImageViewer::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    viewMenu->addSeparator();

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &ImageViewer::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &ImageViewer::about);
    helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);
}

void ImageViewer::updateActions()
{
    saveAsAct->setEnabled(!image.isNull());
    copyAct->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    //Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

template<typename T>
void SetBetween0and255(T& value)
{
    if(value > 255)
        value = 255;
    if(value < 0)
        value = 0;
}

void CreateHistogram(QMap<QRgb,long>& histo, QImage& image)
{
    QRgb c = 0;
    int l, k;
    histo.clear();
    for ( k = 0; k < image.width(); ++k )
        for ( l = 0; l < image.height(); ++l ) {
            c = image.pixel(k,l);
            if ( !histo.contains(c) ) {
                histo.insert(c,0);
                histo[c] = histo[c] + 1;
            }
        }
    //computation of occurences
//    for ( k = 0; k < image.width(); ++k )
//        for ( l = 0; l < image.height(); ++l ) {
//            c = image.pixel(k,l);
//        }
//    auto s = histo.size();
}

bool ImageViewer::ValidHeight( int y, int height )
{
    if ( ( y+height ) <= image.height() )
        return true;
    else
        return false;
}

bool ImageViewer::ValidWidth(int x, int width)
{
    if( ( x+width ) <= image.width() )
        return true;
    else
        return false;
}

bool ImageViewer::ValidLocalArea(int x, int y, int width, int height)
{
    if (image.isNull())
        return false;
    else if ( ValidWidth(x, width) && ValidHeight(y, height) )
        return true;
    return false;
}

void ImageViewer::LinearPixelContrast( QRgb *c, int x, int y,
                                          double &r, double &g, double &b,
                                          double &r_n, double &g_n, double &b_n)
{
    (*c) = newImage.pixel(x,y);
    r = qRed(*c); g = qGreen(*c); b = qBlue(*c);

    r_n = r * onePercentOf255; r_n -= 0.5;
    r_n *= contrastFactor ;
    r_n += 0.5; r_n *= 255;
    g_n = g * onePercentOf255; g_n -= 0.5;
    g_n *= contrastFactor;
    g_n += 0.5; g_n *= 255;
    b_n = b * onePercentOf255; b_n -= 0.5;
    b_n *= contrastFactor;
    b_n += 0.5; b_n *= 255;
    SetBetween0and255(r_n); SetBetween0and255(g_n); SetBetween0and255(b_n);
    newImage.setPixel(x,y,qRgb((int)r_n, (int)g_n, (int)b_n));
}

void ImageViewer::AdaptivePixelContrast( QRgb *c, int x, int y,
                                          double &r, double &g, double &b,
                                          double &r_n, double &g_n, double &b_n, double fctr = 0xFFFFFFFF  )
{
    (*c) = newImage.pixel(x,y);
    r = qRed(*c); g = qGreen(*c); b = qBlue(*c);

    r_n = r * onePercentOf255; r_n -= 0.5;
    r_n *= fctr ;
    r_n += 0.5; r_n *= 255;
    g_n = g * onePercentOf255; g_n -= 0.5;
    g_n *= fctr;
    g_n += 0.5; g_n *= 255;
    b_n = b * onePercentOf255; b_n -= 0.5;
    b_n *= fctr;
    b_n += 0.5; b_n *= 255;
    SetBetween0and255(r_n); SetBetween0and255(g_n); SetBetween0and255(b_n);
    newImage.setPixel(x,y,qRgb((int)r_n, (int)g_n, (int)b_n));
}

double Reverse(double gain)
{
    if(gain != 0)
        return (1 / gain);
    else
        return gain;
}

template<typename T>
double ImageViewer::ApproximateBrightness(T r, T g, T b)
{
    return (0.2126*r + 0.7152*g + 0.0722*b);
}

void ImageViewer::AdaptiveLineContrast( int startY, int lineWidth , int lineHeight )
{
        double r_n = 0.0, g_n = 0.0, b_n = 0.0;
        double r = 0.0, g = 0.0, b = 0.0;
        int avgBright = 0;
        int count = 0;
        double brightness = 0;
        QRgb pixel;
        for ( int x = 0 ; x < lineWidth; x++ ) {
          for ( int y = startY ; y < startY+lineHeight; y++ ) {
              pixel = image.pixel(x, y);
              brightness = ImageViewer::ApproximateBrightness( qRed( pixel ), qGreen( pixel ), qBlue( pixel ) );
              avgBright += brightness;
              count++;
          }
        }

        avgBright /= count;

        for ( int y = startY ; y <= startY + lineHeight; y++ )
        {
            for ( int x = 0 ; x < lineWidth; x++ ) {
                pixel = image.pixel(x, y);
                r = qRed( pixel ), g = qGreen( pixel ), b = qBlue( pixel );
                brightness = ImageViewer::ApproximateBrightness( r, g, b );
                if ( brightness > avgBright ) {
                    ImageViewer::LinearPixelContrast( &pixel, x, y, r, g, b, r_n, g_n, b_n );
                } else if ( brightness < avgBright ) {
                    Reverse(ImageViewer::contrastFactor);
                    ImageViewer::LinearPixelContrast( &pixel, x, y, r, g, b, r_n, g_n, b_n );
                    Reverse(ImageViewer::contrastFactor);
                }
            }
        }
}

void ImageViewer::ChangeContrast(int startX, int startY, int width, int height)
{
    if (startX != -1) {
        if ( !ValidLocalArea(startX, startY, width, height) )
            return;
    } else {
        startX=0; startY=0;
        height = image.height();
        width = image.width();
    }

    if (contrastFactor == invalid_factor || image.isNull() || !isImageSet)
        return;
    //changing contrast of an newImage by factor getted from horizontal slider ui:
    //double factor = (double)( (double)sliderBar->value() )/100 ;    // to change also

    newImage = image.copy();
    double r = 0.0, g = 0.0, b = 0.0;
    double r_n = 0.0, g_n = 0.0, b_n = 0.0;
    QRgb c = 0;
    for ( int y = startY; y < (startY+height); y++ ) {
        for ( int x = startX; x < (startX+width); x++ ) {
            LinearPixelContrast(&c, x, y, r, g, b, r_n, g_n,b_n);
        }
    }
    setImage(newImage, false);
}

void ImageViewer::AdaptiveContrast()
{
    int startX = 0, startY = 0;
    int height = image.height();
    int width = image.width();

    if ( contrastFactor == invalid_factor || image.isNull() || !isImageSet )
        return;
    newImage = image.copy();

    double r = 0.0, g = 0.0, b = 0.0;
    double r_n = 0.0, g_n = 0.0, b_n = 0.0;
    QRgb c = 0;

    for ( int y = startY; y < ( startY + height ); y++ ) {
        //TODO: image height calibration
        AdaptiveLineContrast(y, newImage.width(), 1);
    }
    setImage(newImage, false);
}

void ImageViewer::ShowBrightnessArray()
{
    std::cout << "\nbrightness array :" << std::endl;
    for ( int i = 0; i < 256; i++ )
    {
        std::cout << ( RGBLevels[i] ) << " ";
    }
}

void ImageViewer::GatherLevels(QImage *targetImg, int height)
{
    for ( int i = 0; i < 256; i++ )
        RGBLevels[i] = 0;
    if ( targetImg->isNull() || !isImageSet )
        return;
    //changing contrast of an newImage by factor getted from horizontal slider ui:
    //double factor = (double)( (double)sliderBar->value() )/100 ;    // to change also

    //QMap<QRgb,long> histo; // histogram map
    //CreateHistogram( histo, image );
    //compute average value
    //QMap<QRgb,long>::iterator j;
    //auto sz = histo.size();
    double r, g, b;
    double higherRate = 0.0;
    QRgb c;
    int brightness = 0;
    for ( int w = 0 ; w < targetImg->width(); w++) {
        for ( int h = 0; h < targetImg->height(); h++) {
            c = targetImg->pixel(w, h);
            r = qRed(c); g = qGreen(c); b = qBlue(c);

            brightness = ImageViewer::ApproximateBrightness(r,g,b); //

            //SetBetween0and255(brightness);
            RGBLevels[(int)brightness]+=1;

            if ( higherRate < RGBLevels[(int)brightness] )
                higherRate = RGBLevels[(int)brightness];
        }
    }

    ShowBrightnessArray();

    // factor = brightest / 256
    double factor = ((double)higherRate / 256);
    for (int i = 0; i < 256; i++) {
        RGBLevels[i] = (RGBLevels[i] / factor);
    }

    ShowBrightnessArray();
}

void ImageViewer::LineBrightness(QImage *targetImg, int yPosition)
{
    ProjectWide::oneLinePaint = true;
    ProjectWide::g_brightnessVector.clear();
    for ( int i = 0; i < 256; i++ )
        RGBLevels[i] = 0;
    if ( image.isNull() || !isImageSet )
        return;
    double r, g, b;
    QRgb c;
    int brightness = 0;
    for ( int w = 0; w < targetImg->height(); w++) {
        c = targetImg->pixel(w, yPosition);
        r = qRed(c);
        g = qGreen(c);
        b = qBlue(c);
        brightness = ApproximateBrightness(r, g, b); //

        //SetBetween0and255(brightness);
        ProjectWide::g_brightnessVector.push_back((int)brightness);
    }
}

//void Nothing()
//{
//    int reds[256] {0};
//    int greens[256] {0};
//    int blues[256] {0};

//    QRgb c;
//    QColor col;
//    int r; int g; int b;
//    double r_n; double g_n; double b_n;

//    for (int w = 0; w < targetImg->width(); w++)
//    {
//        c = targetImg->pixel(w, height);
//        col = QColor(c);
//        col.getRgb(&r,&g,&b);
//        SetBetween0and255(r);
//        SetBetween0and255(g);
//        SetBetween0and255(b);
//        reds[r]++; greens[g]++; blues[b]++;
//    }
//    int Y = 0;
//    for (int w = 0 ; w < targetImg->width(); w++)
//    {
//        ///source: https://otvet.mail.ru/question/223481617
//        Y = (0.30*reds[w] + 0.59*greens[w] + 0.11*blues[w]);
//        (*visibleBrightness)[w] = (char)Y;
//    }
//}
