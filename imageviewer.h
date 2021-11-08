
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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

#include <iostream>
#include <QImage>
#include "projectwindows.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QSlider;
class QPushButton;
class QVBoxLayout;

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    template<typename T>
    static double ApproximateBrightness(T r, T g, T b); //
    //
    ImageViewer();
    bool loadFile(const QString &);
    bool ValidLocalArea(int x, int y, int width, int height);
    void ChangeContrast(int x = -1, int y = -1, int width = -1, int height = -1);
    bool ValidWidth(int x, int width);
    bool ValidHeight(int y, int height);
    void GatherLevels(QImage *targetImg, int height);
    void LineBrightness(QImage *targetImg, int height);
    void ShowBrightnessArray();

    void AdaptiveContrast( );
    static void LinearPixelContrast(QRgb *c, int x, int y,
                                 double &r, double &g, double &b,
                                 double &r_n, double &g_n, double &b_n);

    static void AdaptivePixelContrast(QRgb *c, int x, int y,
                                 double &r, double &g, double &b,
                                 double &r_n, double &g_n, double &b_n, double fctr);
    //std::vector<double> _brightnesses;


    static void AdaptiveLineContrast( int startY = 0, int width = -1, int height = 1  );

    static double onePercentOf255;
    static QImage image;
    static QImage newImage;
    double RGBLevels[256];
    const double invalid_factor = -1000000.0;
    static double contrastFactor;
    static double lineLength;
    bool isImageSet;
private slots:
    void open();
    void saveAs();
    void print();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();
    void showContrastWnd();
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void createActions();
    void createMenus();
    void updateActions();
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage, bool cacheImage = true);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QLabel *imageLabel;
    QScrollArea *scrollArea;

    double scaleFactor;

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif
    QAction *saveAsAct;
    QAction *printAct;
    QAction *copyAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *changeContrast;
};

#endif
