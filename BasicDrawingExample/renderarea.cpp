#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    shape = Line;
    antialiased = false;
    transformed = false;
    pixmap.load(":/images/qt-logo.png");

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    for (int i = 0; i < 256 ; i++)
        RGBLevels[i] = 0;
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

void RenderArea::setShape(Shape shape)
{
    this->shape = shape;
    update();
}

void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void RenderArea::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

void RenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void RenderArea::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}

void RenderArea::PaintBrightness(QPainter *painter)
{
    for (int x = 0; x < 256; x += 1) {
            painter->save();
            painter->translate(x, 0);
            painter->drawLine(//rect.bottomLeft(),
                             QPoint(5, 25+256),
                             /*rect.topRight()*/
                             QPoint(5, (256)-RGBLevels[x])
                    );
            painter->restore();
    }
}

void RenderArea::LineBrightness(QPainter *painter)
{
    for ( int x = 0; x < ProjectWide::g_brightnessVector.size() ; x++ ) {
            painter->save();
            painter->translate(x, 0);
            painter->drawLine(//rect.bottomLeft(),
                             QPoint(5, 25+256),
                             /*rect.topRight()*/
                             QPoint(5, (256)-ProjectWide::g_brightnessVector[x])
                    );
            painter->restore();
    }
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this); // =QPainter
    painter.setPen(pen);
    painter.setBrush(brush);
    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    if (RGBLevels == nullptr)
        return;
    if ( !ProjectWide::oneLinePaint ) {
        PaintBrightness(&painter);
    } else {
        LineBrightness(&painter);
    }
    ProjectWide::g_brightnessVector.clear();

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
