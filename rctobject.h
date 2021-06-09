#ifndef RCTOBJECT_H
#define RCTOBJECT_H

#include <QImage>
#include <QPoint>
#include <QVector>
#include <QString>
#include <array>
#include <memory>
#include <QJsonObject>

class RCTObject
{
public:
    RCTObject();

    //load all frames from an output folder from the blender 2.83 template
    void loadFromOutputFolder(const QString& folderPath);

    void setSizeX(const int sizeX);
    void setSizeY(const int sizeY);
    void setSizeZ(const int sizeZ);

    QJsonObject toJson() const;

    int getNumAnimationFrames() const;
    int getSizeX() const;
    int getSizeY() const;
    int getSizeZ() const;

    const QImage* getFrameSprite(const int view, const int numFrame) const;
    const QPoint* getFrameOffset(const int view, const int numFrame) const;

    QString getID() const;

    enum class SourceGame
    {
        Official,
        Custom,
        WW,
        TT,
        RCT2,
    };

    enum class Cursor
    {
        TreeDown,
        StatueDown,
        HouseDown,
    };

    enum class ObjectType
    {
        Ride,
        ScenerySmall,
        SceneryLarge,
        Water,
        TerrainSurface,
        TerrainEdge,
        Station,
        SceneryWall,
        SceneryGroup,
        ParkEntrance,
    };

    //this is inside the properties, not the same as ObjectType!!!
    enum class Type
    {
        Hypercoaster,
        JuniorRC,
        Shop,
        MinuatureRailway,
        FoodStall
    };

    enum class Category
    {
        Rollercoaster,
        Stall,
        Transport,
    };

    enum class ShopItem
    {
        Tshirt,
        Pizza,
    };

    struct RatingMultiplier
    {
        int Intensity;
        int Nausea;
        int Excitement;
    };

    enum class CarColour
    {
        White,
        BrightRed,
        Yellow,
        BordeauxRed,
        SaturatedRed
    };

    enum class EntertainerCostume
    {
        Bandit,
        Sheriff,
        Panda,
        Tiger,
        Elephant,
        Knight,
        Gorilla,
    };

    struct Palette
    {
        QString Name;
        unsigned int Index;
        QVector<QColor> Colours;
    };

    struct CarFrame
    {
        bool Flat;
        bool GentleSlopes;
        bool DiagonalSlopes;
        bool FlatBanked;
        bool FlatToGentleSlopeBankedTransitions;
        bool DiagonalGentleSlopeBankedTransitions;
        bool RestraintAnimation;
    };

    struct Car
    {
        unsigned int RotationFrameMask;
        unsigned int Spacing;
        unsigned int Mass;
        unsigned int NumSeats;
        unsigned int NumSeatRows;
        unsigned int FrictionSoundId;
        unsigned int SoundRange;
        unsigned int DrawOrder;
        QVector<CarFrame> Frames;
        bool HasAdditionalColour2;
        bool HasAdditionalColour1;
        bool HasScreamingRiders;
        bool IsPowered;
        bool VehiculeAnimation;
        QVector<int> LoadingPositions;
    };

    enum class Shape
    {
        Shape1_4
    };

    //many properties are optional depending on the object type, at the moment of writing the JSON, we will only write the revelant properties
    //also, in the GUI, only show the revelant properties depending on the object type
    struct Properties
    {
        //shared ride properties
        Category Category;
        Type Type;

        //roller coaster properties
        bool NoInversions;
        unsigned int MinCarsPerTrain;
        unsigned int MaxCarsPerTrain;
        unsigned int NumEmptyCars;
        unsigned int DefaultCar;
        QVector<unsigned int> HeadCars;
        unsigned int TailCars;
        unsigned int TabCar;
        RatingMultiplier RatingMultipliers;
        unsigned int MaxHeight;
        QVector<QVector<CarColour>> CarColours;
        QVector<Car> Cars;
        unsigned int BuildMenuPriority;

        //scenery properties
        int Price;
        int RemovalPrice;
        Cursor Cursor;
        unsigned int Height;
        bool IsStackable;
        bool HasNoSupports;
        QString SceneryGroup;

        //water properties
        QVector<Palette> Palettes;

        //Terrain properties
        bool SmoothWithSelf;
        bool SmoothWithOther;

        //Station properties
        unsigned int ScrollingMode;
        bool IsTransparent;

        //wall properties
        bool HasPrimaryColour;
        bool IsAllowedOnSlope;

        //scenery group properties
        QVector<QString> Entries;
        unsigned int Priority;
        QVector<EntertainerCostume> EntertainerCostumes;

        //stall properties
        QVector<ShopItem> Sells;
        bool DisablePainting;

        //flat ride properties
        float TabScale;
        unsigned int CarsPerFlatRide;

        //transport ride properties
        bool PlayDepartSound;
        bool NoCollisionCrashes;

    };

    struct Image
    {
        QString Path;
        int X;
        int Y;
    };

    struct LocalizedText
    {
        QString En;
        QString Fr;
        QString De;
        QString Es;
        QString It;
        QString Nl;
        QString Sv;
        QString Zh;
        QString Cs;
        QString Ja;
        QString Ko;
        QString Pl;
        QString Pt;
        QString Ru;
        QString Eo;
    };

private:
    //store the 4 views frames
    std::array<QVector<QImage>, 4> sprites;

    //store the 4 views frame offsets
    std::array<QVector<QPoint>, 4> offsets;

    //some object properties

    //clearances
    int sizeX, sizeY, sizeZ;

    //properties
    QString id;
    QStringList authors;
    QString version;
    SourceGame sourceGame;
    ObjectType objectType;
    Properties properties;
    QVector<Image> images;
    LocalizedText name;
    LocalizedText description;
    LocalizedText capacity;

    //some utility functions
    void loadViewFromFolder(const int view, const QString& folderPath);
};

#endif // RCTOBJECT_H
