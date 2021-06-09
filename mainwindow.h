#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QAction>
#include "rctobject.h"

class QViewportControls;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void importFolder();
    void exportToJson();
private:
    //QObjectViewport* viewport;
    QViewportControls* viewportControls;

    QMenu* fileMenu;

    //the working rct object
    RCTObject rctObject;
};
#endif // MAINWINDOW_H
