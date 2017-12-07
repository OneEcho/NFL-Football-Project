#include "map.h"
#include <QDebug>
Map::Map()
{
    ar = new MapNode[300];
    capacity = 300;
}
void Map::setCapacity(int capacity) {
    this->capacity = capacity;
    ar = new MapNode[this->capacity];
}
// inserts value and key into the array using double hashing

void Map::put(MapNode node, QString key) {

    // gets appropriate index using double hashing
    int index = getValidIndex(key);
    // store new element in array
    ar[index] = node;
}
// recursive function to find appropriate index using double hashing

int Map::getValidIndex(QString key, int j, int k) {
    // double hashing equation

    int result = (equationOne(k) + (j * equationTwo(k))) % capacity;
    // if element is not occupied or the keys match then return the valid index
    if(!ar[result].isOccupied() || key == ar[result].getKey()) {

        return result;
    }
    else {
        // otherwise recurse and increment j
        return getValidIndex(key, j+1, k);
    }
}
int Map::translateKeyIntoInt(QString key) {
    int keyNum;
    for(int i = 0; i < key.size(); ++i) {
        keyNum *= (key[i].unicode());
    }
    return keyNum;
}
int Map::getValidIndex(QString key) {
    int keyInt = translateKeyIntoInt(key);
    return getValidIndex(key, 0, keyInt);
}

int Map::equationOne(int key) {
    return key % capacity;
}

int Map::equationTwo(int key) {
    return 17 - (key % 17);
}

void Map::print() {
    for(int i = 0; i < capacity; ++i) {
        qDebug() << "index: " << i;
        if(i < 10) {

            qDebug() <<  "   ";
        }
        else qDebug() << "  ";
        // if not occupied then display available
        if(!ar[i].isOccupied()) {
            qDebug() << "Available\n";
        }
        else {
            qDebug() << ar[i].getKey() << " " << ar[i].getValue() << endl;
        }
    }
}
void Map::remove(QString key, QString souvenir) {

    int index = find(key);
    if(index == -1) return;
    // set index to not occupied
    QVector<Souvenir> souvenirs = ar[index].getSouvenirs();
    for(int i = 0; i < souvenirs.size(); ++i) {
        if(souvenirs[i].getTeamName() == key) {
            QVector<Souvenir>::iterator it = souvenirs.begin();
            souvenirs.erase(it+i);
            return;
        }
    }

}
// searches through the list using double hashing to find the
// index storing the key being searched for

int Map::find(QString key, int j) {
    int keyAsInt = translateKeyIntoInt(key);
    int result = (equationOne(keyAsInt) + (j * equationTwo(keyAsInt))) % capacity;

    // if occupied and key matches then the index has been found
    if(ar[result].isOccupied() && key == ar[result].getKey()) {
        return result;
    }
    // else reached the end of the list
    else if(j == capacity-1) {
        return -1;
    }
    else {
        // recurse and increment j
        return find(key, j+1);
    }
}

int Map::find(QString key) {
    return find(key, 0);

}
// recursive function to find the appropriate index where the key is stored
// remove element where the key exists
Map::~Map() {
    delete [] ar;
}


