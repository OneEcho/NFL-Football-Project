#include "mapnode.h"

MapNode::MapNode()
{
    occupied = false;
}
void MapNode::setAvailable() {
    occupied = false;
}
void MapNode::setKey(QString k) {
    key = k;
}
QString MapNode::getKey() const {
    return key;
}
bool MapNode::isOccupied() {
    return occupied;
}
MapNode::MapNode(QString value) {
    this->value = value;
    occupied = true;
}
QString MapNode::getValue() const {
    return value;
}
void MapNode::setValue(QString val) {
    this->value = val;
    occupied = true;
}
// overloads assignment operator
MapNode MapNode::operator=(const MapNode& arg) {
    this->setValue(arg.getValue());
    this->setKey(arg.getKey());
    return *this;
}
QVector<Souvenir> MapNode::getSouvenirs() {
    return this->souvenirs;
}
void MapNode::addSouvenir(Souvenir s) {
    souvenirs.push_back(s);
}
