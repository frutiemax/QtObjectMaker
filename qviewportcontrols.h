#ifndef QVIEWPORTCONTROLS_H
#define QVIEWPORTCONTROLS_H

#include <QWidget>

class QObjectViewport;
class QScrollBar;
class RCTObject;
class QRCTObjectPropertiesWidget;
class QCheckBox;

class QViewportControls : public QWidget
{
    Q_OBJECT
public:
    explicit QViewportControls(QWidget *parent = nullptr);

    void setObjectSize(const int sizeX, const int sizeY);
    void initializeFromRctObject(const RCTObject& object);

signals:

private slots:
    void changeFrame(const int frame);

    void changeSizeX(const int sizeX);
    void changeSizeY(const int sizeY);
    void changeSizeZ(const int sizeZ);

    void clearancesCheckboxChanged(const int state);
    void heightCheckboxChanged(const int state);
private:
    QObjectViewport* view0;
    QObjectViewport* view1;
    QObjectViewport* view2;
    QObjectViewport* view3;

    QScrollBar* frameScroll;

    //the object properties panel
    QRCTObjectPropertiesWidget* propertiesPanel;

    //should we ever show the clearances?
    QCheckBox* clearancesCheckbox;

    //should we ever show the height of the object?
    QCheckBox* heightCheckbox;
};

#endif // QVIEWPORTCONTROLS_H
