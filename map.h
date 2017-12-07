#ifndef MAP_H
#define MAP_H
#include "mapnode.h"


class Map
{
    MapNode* ar; // bucket array
    int capacity;	   // size of array
    // recursive method that performs double hashing to find appropriate index
    int getValidIndex(QString key,
                      int j,// IN - incrementing value for hash function
                      int k); // IN - integer version of key
    int translateKeyIntoInt(QString key);
    // helper method for ^
    int getValidIndex(QString key);
    // first equation of the double hashing
    int equationOne(int key);
    // second equation of the double hashing
    int equationTwo(int key);
    int find(QString key);
    // recursive method to find matching key in bucket array
    int find(QString key,
             int j);   	// incrementing value for double hashing
    // recursive method to find matching key in bucket array
    // using quadratic hashing

public:
    Map();
    ~Map();
    // initialize array size and create array
    Map(int capacity);
    void setCapacity(int capacity);
    // insert value into the list after double hashing
    void put(MapNode, QString);
    // insert value into the list after quadratic hashing
    // prints that array
    void print();
    // removes by key
    void remove(QString key, QString souvenirName);

};

#endif // MAP_H
