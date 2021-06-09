#include "rctobject.h"
#include <QDir>
#include <QCollator>
#include <algorithm>
#include <QJsonArray>

RCTObject::RCTObject():
    sizeX(1), sizeY(1), sizeZ(0)
{
    id = "custom.lmbtree0";
    authors.push_back("Lucas Malo Belanger");
}

//https://stackoverflow.com/questions/36005814/load-images-from-folder-with-qt
//https://stackoverflow.com/questions/11933883/sort-filenames-naturally-with-qt/11947242
//https://stackoverflow.com/questions/27318631/parsing-through-a-csv-file-in-qt
void RCTObject::loadFromOutputFolder(const QString& folderPath)
{
    //first clear any existing data
    for(int i = 0; i < 4; i++)
    {
        offsets[i].clear();
        sprites[i].clear();
    }
    loadViewFromFolder(0, folderPath);
    loadViewFromFolder(1, folderPath);
    loadViewFromFolder(2, folderPath);
    loadViewFromFolder(3, folderPath);
}

void RCTObject::loadViewFromFolder(const int view, const QString& folderPath)
{
    QDir dir(folderPath);

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::NoSort);

    //get the images first
    QStringList filter;
    filter << QString::number(view) + QLatin1String("_*");
    dir.setNameFilters(filter);

    QCollator collator;
    collator.setNumericMode(true);

    auto entryInfoList = dir.entryInfoList();

    std::sort(
        entryInfoList.begin(),
        entryInfoList.end(),
        [&collator](const QFileInfo &file1, const QFileInfo &file2)
        {
            return collator.compare(file1.fileName(), file2.fileName()) < 0;
        });

    for(const auto& entry : entryInfoList)
    {
        //load the image
        QImage image(entry.absoluteFilePath());
        sprites[view].push_back(image);
    }

    //read the csv for this view for the offsets
    filter.clear();
    filter << QString::number(view) + QLatin1String(".csv");
    dir.setNameFilters(filter);

    entryInfoList = dir.entryInfoList();

    //there should only be one file
    QString csvPath = entryInfoList[0].absoluteFilePath();

    //read the csv file
    QFile file(csvPath);
    file.open(QIODevice::ReadOnly);

    QStringList words;
    QPoint offset;
    while(!file.atEnd())
    {
        QString line = file.readLine();
        words = line.split(',');
        offset.setX(words[0].toInt());
        offset.setY(words[1].toInt());
        offsets[view].push_back(offset);
    }
}

void RCTObject::setSizeX(const int sizeX)
{
    this->sizeX = sizeX;
}

void RCTObject::setSizeY(const int sizeY)
{
    this->sizeY = sizeY;
}

void RCTObject::setSizeZ(const int sizeZ)
{
    this->sizeZ = sizeZ;
}

QJsonObject RCTObject::toJson() const
{
    QJsonObject json;

    //just a large 2x2 object for now
    json["id"] = id;

    QJsonArray authors;
    for(const auto& author : this->authors)
        authors.push_back(author);
    json["authors"] = authors;

    json["sourceGame"] = "custom";
    json["objectType"] = "scenery_large";

    QJsonObject properties;
    properties["price"] = 165;
    properties["removalPrice"] = -145;
    properties["cursor"] = "CURSOR_TREE_DOWN";
    properties["isPhotogenic"] = false;

    QJsonArray tiles;
    QJsonObject tile0, tile1, tile2, tile3;

    tile0["x"] = 0;
    tile0["y"] = 0;
    tile0["clearance"] = 144;
    tile0["hasSupports"] = true;
    tile0["walls"] = 9;

    tile1["x"] = 0;
    tile1["y"] = 0;
    tile1["clearance"] = 144;
    tile1["hasSupports"] = true;
    tile1["walls"] = 9;

    tile2["x"] = 0;
    tile2["y"] = 32;
    tile2["clearance"] = 144;
    tile2["hasSupports"] = true;
    tile2["walls"] = 3;

    tile3["x"] = 32;
    tile3["y"] = 32;
    tile3["clearance"] = 144;
    tile3["hasSupports"] = true;
    tile3["walls"] = 6;

    tiles.push_back(tile0);
    tiles.push_back(tile1);
    tiles.push_back(tile2);
    tiles.push_back(tile3);
    properties["tiles"] = tiles;

    json["properties"] = properties;

    //we need to define the images
    QJsonArray images;

    QString path;
    int offsetX, offsetY;
    for(int i = 0; i < 4; i++)
    {
        //figure out what to do with the animations
        path = "images/" + QString::number(i) + ".png";
        offsetX = offsets[i][0].x();
        offsetY = offsets[i][0].y();

        QJsonObject imageJson;
        imageJson["path"] = path;
        imageJson["x"] = offsetX;
        imageJson["y"] = offsetY;
        images.push_back(imageJson);
        /*for(int j = 0; j < getNumAnimationFrames(); j++)
        {
            path = "images/" + QString::number(i) + "_" + QString::number(j) + ".png";
            offsetX = offsets[i][j].x();
            offsetY = offsets[i][j].y();

            QJsonObject imageJson;
            imageJson["path"] = path;
            imageJson["x"] = offsetX;
            imageJson["y"] = offsetY;
            images.push_back(imageJson);
        }*/
    }
    json["images"] = images;

    //finally, put the localized text
    QJsonObject strings;
    QJsonArray name;
    QJsonObject nameEN;
    nameEN["en-GB"] = "LMB - Large Tree";
    name.push_back(nameEN);
    strings["name"] = name;
    json["strings"] = strings;
    return json;
}

int RCTObject::getNumAnimationFrames() const
{
    //TO-DO : figure out what to do for animations
    return 1;
    //return this->offsets[0].size();
}

int RCTObject::getSizeX() const
{
    return sizeX;
}

int RCTObject::getSizeY() const
{
    return sizeY;
}

int RCTObject::getSizeZ() const
{
    return sizeZ;
}

const QImage* RCTObject::getFrameSprite(const int view, const int numFrame) const
{
    if(numFrame < 0)
        return nullptr;
    if(view < 0)
        return nullptr;
    if(view >= 4)
        return nullptr;
    if(numFrame >= sprites[view].size())
        return nullptr;
    return &sprites[view][numFrame];
}

const QPoint* RCTObject::getFrameOffset(const int view, const int numFrame) const
{
    if(numFrame < 0)
        return nullptr;
    if(view < 0)
        return nullptr;
    if(view >= 4)
        return nullptr;
    if(numFrame >= offsets[view].size())
        return nullptr;
    return &offsets[view][numFrame];
}

QString RCTObject::getID() const
{
    return id;
}
