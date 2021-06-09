#include "qrctobjectpropertieswidget.h"

#include <QGroupBox>
#include <QString>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QFormLayout>

QRCTObjectPropertiesWidget::QRCTObjectPropertiesWidget(QWidget *parent):
    QWidget(parent)
{
    objectProperties = new QGroupBox(tr("Object Properties"), this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(objectProperties);

    QLabel* sizeXLabel = new QLabel(this);
    sizeXLabel->setText("Size X:");

    QLabel* sizeYLabel = new QLabel(this);
    sizeYLabel->setText("Size Y:");

    QLabel* sizeZLabel = new QLabel(this);
    sizeZLabel->setText("Size Z:");

    sizeXSpinBox = new QSpinBox(this);
    sizeYSpinBox = new QSpinBox(this);
    sizeZSpinBox = new QSpinBox(this);

    sizeXSpinBox->setMaximumWidth(50);
    sizeYSpinBox->setMaximumWidth(50);
    sizeZSpinBox->setMaximumWidth(50);

    sizeXSpinBox->setMinimum(1);
    sizeYSpinBox->setMinimum(1);
    sizeZSpinBox->setMinimum(1);

    sizeXSpinBox->setMaximum(5);
    sizeYSpinBox->setMaximum(5);
    sizeZSpinBox->setMaximum(64);

    QFormLayout* spinLayout = new QFormLayout(this);
    spinLayout->addRow(sizeXLabel, sizeXSpinBox);
    spinLayout->addRow(sizeYLabel, sizeYSpinBox);
    spinLayout->addRow(sizeZLabel, sizeZSpinBox);

    QVBoxLayout* groupBoxLayout = new QVBoxLayout(this);
    groupBoxLayout->addLayout(spinLayout);
    groupBoxLayout->addStretch(1);
    objectProperties->setLayout(groupBoxLayout);
    setLayout(layout);

    setMaximumWidth(150);

    connect(sizeXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QRCTObjectPropertiesWidget::spinXValueChanged);
    connect(sizeYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QRCTObjectPropertiesWidget::spinYValueChanged);
    connect(sizeZSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QRCTObjectPropertiesWidget::spinZValueChanged);
}

void QRCTObjectPropertiesWidget::spinXValueChanged(const int value)
{
    emit this->sizeXChanged(value);
}

void QRCTObjectPropertiesWidget::spinYValueChanged(const int value)
{
    emit this->sizeYChanged(value);
}

void QRCTObjectPropertiesWidget::spinZValueChanged(const int value)
{
    emit this->sizeZChanged(value);
}
