#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qobjectviewport.h"
#include "rctobject.h"
#include "qviewportcontrols.h"

#include <QVBoxLayout>
#include <QTimer>
#include <QFileDialog>
#include "qrctobjectpropertieswidget.h"
#include <QJsonObject>
#include <QJsonDocument>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //viewport = new QObjectViewport(this);
    viewportControls = new QViewportControls(this);
    //QVBoxLayout* layout = new QVBoxLayout;
    //layout->addWidget(viewport);
    //setLayout(layout);
    resize(600, 480);
    setCentralWidget(viewportControls);
    //QImage tree(":/img/0_1.bmp");
    //viewport->addFrame(tree, QPoint{-58,-122});

    //QTimer* timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, viewport, &QObjectViewport::update);
    //timer->start(50);

    fileMenu = menuBar()->addMenu(tr("&File"));

    //add the import folder action
    QAction* importFolderAct = new QAction(tr("&Import Folder"), this);
    connect(importFolderAct, &QAction::triggered, this, &MainWindow::importFolder);
    fileMenu->addAction(importFolderAct);

    //add the export to json folder action
    QAction* exportJsonAct = new QAction(tr("&Export to Json"), this);
    connect(exportJsonAct, &QAction::triggered, this, &MainWindow::exportToJson);
    fileMenu->addAction(exportJsonAct);
}

//https://stackoverflow.com/questions/3941917/can-the-open-file-dialog-be-used-to-select-a-folder
void MainWindow::importFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QString(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(dir.isEmpty())
        return;

    //load all the sprites and offsets
    rctObject.loadFromOutputFolder(dir);

    //initialize the viewport controls widget from the loaded object
    viewportControls->initializeFromRctObject(rctObject);

    //refresh the view
    //viewport->clearFrames();

    //lets show only view 0
    /*int numFrames = rctObject.getNumAnimationFrames();
    for(int i = 0; i < numFrames; i++)
    {
        viewport->addFrame(*rctObject.getFrameSprite(0, i), *rctObject.getFrameOffset(0, i));
    }*/
}

void MainWindow::exportToJson()
{
    //ask for an output directory
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QString(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(dir.isEmpty())
        return;

    //save the json file to that directory
    QFile jsonFile(dir + "/" + rctObject.getID() + ".json");
    jsonFile.open(QIODevice::WriteOnly);

    auto json = rctObject.toJson();
    QJsonDocument jsonDoc(json);
    jsonFile.write(jsonDoc.toJson());
    jsonFile.close();

    //save the images to a .png format in the images folder relative to the chosen directory
    QString imagesFolder(dir + "/images");
    if(!QDir(imagesFolder).exists())
        QDir().mkdir(imagesFolder);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < rctObject.getNumAnimationFrames(); j++)
        {
            auto image = rctObject.getFrameSprite(i, j);

            QString filePath = imagesFolder + "/" + QString::number(i) + ".png";
            image->save(filePath);
        }
    }
}

MainWindow::~MainWindow()
{
}

