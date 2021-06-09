#include "qobjectviewport.h"
#include <QPainter>
#include <QPaintEvent>

QObjectViewport::QObjectViewport(QWidget* parent):
    QOpenGLWidget(parent), sizeX(1), sizeY(1), sizeZ(0), currentFrame(-1), showClearances(false), enClearances(true), enHeight(false)
{
    clearancesTimer = new QTimer(this);
    setAutoFillBackground(false);
    background = QBrush(QColor(64, 32, 64));
    grassBackground = QPixmap(":/img/GrassBackground.PNG");
    setMaximumHeight(586);

    connect(clearancesTimer, &QTimer::timeout, this, &QObjectViewport::toggleClearances);
    clearancesTimer->start(1000);
}

void QObjectViewport::toggleClearances()
{
    showClearances = !showClearances;
    this->update();
}

void QObjectViewport::updateGrass(QPaintEvent *event, QPainter &painter)
{
    QVector2D center;
    center.setX(grassBackground.width()/2);
    center.setY(grassBackground.height()/2);

    QPoint grassOffset;
    grassOffset.setX(-23);
    grassOffset.setY(-4);

    QPoint delta;
    delta.setX(-32.0f * (float)(sizeX)/2.0f);
    delta.setY(-16.0f * (float)(sizeX)/2.0f);
    grassOffset += delta;

    delta.setX(-32.0f * (float)(sizeY)/2.0f);
    delta.setY(16.0f * (float)(sizeY)/2.0f);
    grassOffset += delta;

    //calculate the source rectangle
    QRect sourceRect;
    QVector2D screenCenter;
    screenCenter.setX(event->rect().width()/2);
    screenCenter.setY(event->rect().height()/2);

    sourceRect.setTop(center.y() - screenCenter.y() + grassOffset.y());
    sourceRect.setLeft(center.x() - screenCenter.x() + grassOffset.x());
    sourceRect.setRight(center.x() + screenCenter.x() + grassOffset.x());
    sourceRect.setBottom(center.y() + screenCenter.y() + grassOffset.y());
    painter.drawPixmap(event->rect(), grassBackground, sourceRect);
}


void QObjectViewport::updateClearances(QPaintEvent* event, QPainter& painter)
{
    //configure the pen
    QPen pen(QColor(200,200,0));
    painter.setPen(pen);
    QLine line;
    QPoint point;
    point.setX(origin.x());
    point.setY(origin.y());

    QPoint delta;
    delta.setX(-32.0f * (float)(sizeX) /2.0f);
    delta.setY(-16.0f * (float)(sizeX) /2.0f);
    point += delta;

    delta.setX(-32.0f * (float)(sizeY)/2.0f);
    delta.setY(16.0f * (float)(sizeY)/2.0f);
    point += delta;
    line.setP1(point);

    delta.setX(32 * sizeX);
    delta.setY(16 * sizeX);
    point += delta;
    line.setP2(point);
    painter.drawLine(line);

    line.setP1(point);
    delta.setX(32 * sizeY);
    delta.setY(-16 * sizeY);
    point += delta;
    line.setP2(point);
    painter.drawLine(line);

    line.setP1(point);
    delta.setX(-32 * sizeX);
    delta.setY(-16 * sizeX);
    point += delta;
    line.setP2(point);
    painter.drawLine(line);

    line.setP1(point);
    delta.setX(-32 * sizeY);
    delta.setY(16 * sizeY);
    point += delta;
    line.setP2(point);
    painter.drawLine(line);
}

void QObjectViewport::updateHeight(QPaintEvent* event, QPainter& painter)
{
    QPen pen(QColor(0,200,200));
    painter.setPen(pen);

    QPoint point;
    QPoint delta;
    QLine line;

    point.setX(origin.x());
    point.setY(origin.y());

    delta.setX(-32.0f * (float)(sizeX) /2.0f);
    delta.setY(-16.0f * (float)(sizeX) /2.0f);
    point += delta;

    delta.setX(-32.0f * (float)(sizeY)/2.0f);
    delta.setY(16.0f * (float)(sizeY)/2.0f);
    point += delta;

    //raise the rectangle
    delta.setX(0);
    delta.setY(-sizeZ * 4);
    point += delta;
    line.setP1(point);

    delta.setX(32 * sizeX);
    delta.setY(16 * sizeX);
    point += delta;
    line.setP2(point);
    painter.drawLine(line);

    line.setP1(point);
    delta.setX(32 * sizeY);
    delta.setY(-16 * sizeY);
    point += delta;
    line.setP2(point);
    painter.drawLine(line);

    line.setP1(point);
    delta.setX(-32 * sizeX);
    delta.setY(-16 * sizeX);
    point += delta;
    line.setP2(point);
    painter.drawLine(line);

    line.setP1(point);
    delta.setX(-32 * sizeY);
    delta.setY(16 * sizeY);
    point += delta;
    line.setP2(point);
    painter.drawLine(line);
}

void QObjectViewport::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    origin.setX(event->rect().width()/2);
    origin.setY(event->rect().height()/2);

    //offset the origin toward the low end of the viewport
    origin.setY(origin.y() + 32 * 2);


    //always draw the image from its center
    updateGrass(event, painter);

    //draw clearances
    if(showClearances && enClearances)
        updateClearances(event, painter);

    //draw the current frame
    if(currentFrame != -1)
    {
        painter.drawPixmap(frameOffsets[currentFrame].x() + origin.x(), frameOffsets[currentFrame].y() + origin.y(), *frameSprites[currentFrame]);
    }

    //draw the height delimiters
    if(enHeight && showClearances)
    {
        updateHeight(event, painter);
    }
}

void QObjectViewport::addFrame(const QImage& sprite, const QPoint& offset)
{
    QPixmap* pixmap = new QPixmap();
    QImage copy = sprite;
    replaceFirstPixel(copy);
    pixmap->convertFromImage(copy);
    frameSprites.push_back(pixmap);
    frameOffsets.push_back(offset);
    currentFrame++;
}

void QObjectViewport::clearFrames()
{
    frameSprites.clear();
    frameOffsets.clear();
    currentFrame = -1;
}

void QObjectViewport::setFrameNumber(const int frame)
{
    if(frameSprites.empty())
    {
        currentFrame = -1;
        return;
    }

    if(frame >= static_cast<int>(frameSprites.size()))
    {
        currentFrame = frameSprites.size() - 1;
        return;
    }
    if(frame < 0)
    {
        currentFrame = 0;
        return;
    }
    currentFrame = frame;
}

void QObjectViewport::replaceFirstPixel(QImage& sprite) const
{
    //get the color of the first pixel
    auto colorKey = sprite.pixelColor(0,0);

    //iterate through all the pixels, check if it is equal to colorKey, if it is, put the alpha to 0
    for(int x = 0; x < sprite.size().width(); x++)
    {
        for(int y = 0; y < sprite.size().height(); y++)
        {
            auto pixel = sprite.pixelColor(x,y);
            if(pixel == colorKey)
            {
                sprite.setPixelColor(x, y, QColor(colorKey.red(), colorKey.green(), colorKey.blue(), 0));
            }
        }
    }
}

void QObjectViewport::setClearanceX(const int sizeX)
{
    this->sizeX = sizeX;
    this->update();
}

void QObjectViewport::setClearanceY(const int sizeY)
{
    this->sizeY = sizeY;
    this->update();
}

void QObjectViewport::setClearanceZ(const int sizeZ)
{
    this->sizeZ = sizeZ;
    this->update();
}

void QObjectViewport::enableClearances()
{
    enClearances = true;
    this->update();
}

void QObjectViewport::disableClearances()
{
    enClearances = false;
    this->update();
}

void QObjectViewport::showHeight()
{
    enHeight = true;
    this->update();
}

void QObjectViewport::hideHeight()
{
    enHeight = false;
    this->update();
}




