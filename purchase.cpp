#include "purchase.h"

Purchase::Purchase() {
    quantity = 0;
}
int Purchase::getQuantity() {
    return this->quantity;
}
void Purchase::setQuantity(double q) {
    this->quantity = q;
}
void Purchase::setSouvenir(Souvenir s) {
    souvenir.setPrice(s.getPrice());
    souvenir.setItemName(s.getItemName());
    souvenir.setTeamName(s.getTeamName());
}
double Purchase::getTotalPrice() const {
    return this->quantity * souvenir.getPrice();
}
QString Purchase::getTeamName() const {
    return this->souvenir.getTeamName();
}
QString Purchase::getSouvenir() {
    return this->souvenir.getItemName();
}

