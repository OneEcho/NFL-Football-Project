#include "souvenir.h"

Souvenir::Souvenir()
{
    this->quantity = 0;
    this->stadium= "";
    this->price = 0;
    this->itemName = "";

}
void Souvenir::setItemName(QString item) {
    this->itemName = item;
}
void Souvenir::setPrice(double price) {
    this->price = price;
}
void Souvenir::setQuantity(int quantity) {
    this->quantity = quantity;
}
void Souvenir::setStadiumName(QString name) {
    this->stadium = name;
}
QString Souvenir::getStadiumName() const {
    return this->stadium;
}
int Souvenir::getQuantity() const {
    return this->quantity;
}
QString Souvenir::getItemName() const {
    return this->itemName;
}
double Souvenir::getPrice() const {
    return this->price;
}


