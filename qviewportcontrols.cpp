#include "qviewportcontrols.h"
#include <QHBoxLayout>
#include <QScrollBar>
#include "qobjectviewport.h"
#include "rctobject.h"
#include "qrctobjectpropertieswidget.h"
#include <QCheckBox>

QViewportControls::QViewportControls(QWidget *parent) :
    QWidget(parent)
{
    view0 = new QObjectViewport(this);
    view1 = new QObjectViewport(this);
    view2 = new QObjectViewport(this);
    view3 = new QObjectViewport(this);

    clearancesCheckbox = new QCheckBox(this);
    clearancesCheckbox->setText("Show clearances");
    clearancesCheckbox->setChecked(true);
    heightCheckbox = new QCheckBox(this);
    heightCheckbox->setText("Show height");

    propertiesPanel = new QRCTObjectPropertiesWidget(this);

    frameScroll = new QScrollBar(this);
    QHBoxLayout* viewsLayout = new QHBoxLayout(this);
    viewsLayout->addWidget(view0);
    viewsLayout->addWidget(view1);
    viewsLayout->addWidget(view2);
    viewsLayout->addWidget(view3);

    QWidget* viewsWidget = new QWidget(this);
    viewsWidget->setLayout(viewsLayout);

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addWidget(viewsWidget);
    vlayout->addWidget(clearancesCheckbox);
    vlayout->addWidget(heightCheckbox);
    vlayout->addWidget(frameScroll);

    QWidget* viewsAndScrollbar = new QWidget(this);
    viewsAndScrollbar->setLayout(vlayout);

    QHBoxLayout* topLayout = new QHBoxLayout(this);
    QHBoxLayout* propertiesLayout = new QHBoxLayout(this);
    propertiesLayout->addWidget(propertiesPanel);

    topLayout->addLayout(propertiesLayout);
    topLayout->addWidget(viewsAndScrollbar);
    setLayout(topLayout);

    frameScroll->setOrientation(Qt::Orientation::Horizontal);
    frameScroll->setMaximum(0);

    connect(frameScroll, &QScrollBar::valueChanged, this, &QViewportControls::changeFrame);
    connect(propertiesPanel, &QRCTObjectPropertiesWidget::sizeXChanged, this, &QViewportControls::changeSizeX);
    connect(propertiesPanel, &QRCTObjectPropertiesWidget::sizeYChanged, this, &QViewportControls::changeSizeY);
    connect(propertiesPanel, &QRCTObjectPropertiesWidget::sizeZChanged, this, &QViewportControls::changeSizeZ);

    connect(clearancesCheckbox, &QCheckBox::stateChanged, this, &QViewportControls::clearancesCheckboxChanged);
    connect(heightCheckbox, &QCheckBox::stateChanged, this, &QViewportControls::heightCheckboxChanged);
}

void QViewportControls::initializeFromRctObject(const RCTObject& object)
{
    //clear all frames from the views
    view0->clearFrames();
    view1->clearFrames();
    view2->clearFrames();
    view3->clearFrames();

    //add the frames
    for(int i = 0; i < object.getNumAnimationFrames(); i++)
    {
        auto sprite0 = object.getFrameSprite(0, i);
        auto offset0 = object.getFrameOffset(0, i);
        view0->addFrame(*sprite0, *offset0);

        auto sprite1 = object.getFrameSprite(1, i);
        auto offset1 = object.getFrameOffset(1, i);
        view1->addFrame(*sprite1, *offset1);

        auto sprite2 = object.getFrameSprite(2, i);
        auto offset2 = object.getFrameOffset(2, i);
        view2->addFrame(*sprite2, *offset2);

        auto sprite3 = object.getFrameSprite(3, i);
        auto offset3 = object.getFrameOffset(3, i);
        view3->addFrame(*sprite3, *offset3);
    }

    frameScroll->setMaximum(object.getNumAnimationFrames() - 1);
    frameScroll->setValue(0);
}

void QViewportControls::changeFrame(const int frame)
{
    view0->setFrameNumber(frame);
    view1->setFrameNumber(frame);
    view2->setFrameNumber(frame);
    view3->setFrameNumber(frame);
}

void QViewportControls::changeSizeX(const int sizeX)
{
    //we need to take care of the view rotations here
    view0->setClearanceX(sizeX);
    view1->setClearanceY(sizeX);
    view2->setClearanceX(sizeX);
    view3->setClearanceY(sizeX);
}

void QViewportControls::changeSizeY(const int sizeY)
{
    //again take care of the view rotations
    view0->setClearanceY(sizeY);
    view2->setClearanceY(sizeY);
    view1->setClearanceX(sizeY);
    view3->setClearanceX(sizeY);
}

void QViewportControls::changeSizeZ(const int sizeZ)
{
    view0->setClearanceZ(sizeZ);
    view1->setClearanceZ(sizeZ);
    view2->setClearanceZ(sizeZ);
    view3->setClearanceZ(sizeZ);
}

void QViewportControls::clearancesCheckboxChanged(const int state)
{
    if(this->clearancesCheckbox->isChecked())
    {
        view0->enableClearances();
        view1->enableClearances();
        view2->enableClearances();
        view3->enableClearances();
    }
    else
    {
        view0->disableClearances();
        view1->disableClearances();
        view2->disableClearances();
        view3->disableClearances();
    }
}

void QViewportControls::heightCheckboxChanged(const int state)
{
    if(this->heightCheckbox->isChecked())
    {
        view0->showHeight();
        view1->showHeight();
        view2->showHeight();
        view3->showHeight();
    }
    else
    {
        view0->hideHeight();
        view1->hideHeight();
        view2->hideHeight();
        view3->hideHeight();
    }
}
