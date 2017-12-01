#include "cart.h"

double Cart::getTotalSpent() {
    double spent = 0;
    for(int i = 0; i < souvenirs.size(); ++i) {
        spent += (this->souvenirs[i].getPrice() * souvenirs[i].getQuantity());
    }
}
double Cart::getTotalSpentAt(QString teamName) {
    double spent = 0;
    for(int i = 0; i < souvenirs.size(); ++i) {
        if(souvenirs[i].getTeamName() == teamName) {
            spent += (souvenirs[i].getPrice() * souvenirs[i].getQuantity());
        }
    }
}
void Cart::addPurchase(double price, int quantity, QString itemName, QString teamName) {

    Souvenir purchase;
    purchase.setPrice(price);
    purchase.setItemName(itemName);
    purchase.setQuantity(quantity);
    purchase.setTeamName(teamName);

}

