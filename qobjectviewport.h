#ifndef QOBJECTVIEWPORT_H
#define QOBJECTVIEWPORT_H

#include <QOpenGLWidget>
#include <QVector2D>
#include <vector>
#include <QImage>
#include <QTimer>
class QObjectViewport : public QOpenGLWidget
{
    Q_OBJECT
public:
    QObjectViewport(QWidget* parent);

    void addFrame(const QImage& sprite, const QPoint& offset);
    void clearFrames();
    void setFrameNumber(const int frame);
    void setClearanceX(const int sizeX);
    void setClearanceY(const int sizeY);
    void setClearanceZ(const int sizeZ);

    void enableClearances();
    void disableClearances();

    void showHeight();
    void hideHeight();
public slots:

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void updateClearances(QPaintEvent* event, QPainter& painter);
    void updateHeight(QPaintEvent* event, QPainter& painter);
    void updateGrass(QPaintEvent* event, QPainter& painter);

    QBrush background;
    QPixmap grassBackground;

    int sizeX;
    int sizeY;
    int sizeZ;

    //the object clearances
    QLine line0, line1, line2, line3;

    //the timer that updates the line clearances toggle
    QTimer* clearancesTimer;
    bool showClearances;
    bool enClearances;
    bool enHeight;

    //QRect sourceRect;
    QVector2D origin;

    //frames
    std::vector<QPixmap*> frameSprites;
    std::vector<QPoint> frameOffsets;
    int currentFrame;

    //some image utility functions
    void replaceFirstPixel(QImage& sprite) const;

    void toggleClearances();
};

#endif // QOBJECTVIEWPORT_H
