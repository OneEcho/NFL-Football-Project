#include "cart.h"

double Cart::getTotalSpent() {
    double spent = 0;
    for(int i = 0; i < souvenirs.size(); ++i) {
        spent += (this->souvenirs[i].getPrice() * souvenirs[i].getQuantity());
    }
}
double Cart::getTotalSpentAt(QString stadiumName) {
    double spent = 0;
    for(int i = 0; i < souvenirs.size(); ++i) {
        if(souvenirs[i].getStadiumName() == stadiumName) {
            spent += (souvenirs[i].getPrice() * souvenirs[i].getQuantity());
        }
    }
}
void Cart::addPurchase(double price, int quantity, QString itemName, QString stadiumName) {

    Souvenir purchase;
    purchase.setPrice(price);
    purchase.setItemName(itemName);
    purchase.setQuantity(quantity);
    purchase.setStadiumName(stadiumName);

}

