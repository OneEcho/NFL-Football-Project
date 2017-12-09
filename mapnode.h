#ifndef MAPNODE_H
#define MAPNODE_H
#include "souvenir.h"
#include <QVector>

class MapNode
{
    QString value;
    QVector<Souvenir> souvenirs;
    QString key;
    bool occupied;
public:
    MapNode();
    MapNode(QString value);
    QString getValue() const;
    void setValue(QString val);
    void setKey(QString k);
    QString getKey() const;
    bool isOccupied();	 // is element occupied?
    void setAvailable(); // set occupied to false
    void addSouvenir(Souvenir s);
    MapNode operator=(const MapNode& arg);
    QVector<Souvenir> getSouvenirs();
};

#endif // MAPNODE_H
