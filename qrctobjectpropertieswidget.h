#ifndef QRCTOBJECTPROPERTIESWIDGET_H
#define QRCTOBJECTPROPERTIESWIDGET_H

#include <QWidget>

class QGroupBox;
class QSpinBox;
class QRCTObjectPropertiesWidget: public QWidget
{
Q_OBJECT
public:
    explicit QRCTObjectPropertiesWidget(QWidget *parent = nullptr);

signals:
    void sizeXChanged(const int value);
    void sizeYChanged(const int value);
    void sizeZChanged(const int value);

private:
    QGroupBox* objectProperties;

    QSpinBox* sizeXSpinBox;
    QSpinBox* sizeYSpinBox;
    QSpinBox* sizeZSpinBox;

private slots:
    void spinXValueChanged(const int value);
    void spinYValueChanged(const int value);
    void spinZValueChanged(const int value);
};

#endif // QRCTOBJECTPROPERTIESWIDGET_H
