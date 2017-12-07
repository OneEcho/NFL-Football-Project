#include "cart.h"
#include <QTableWidgetItem>
#include "QSqlQuery"

double Cart::getTotalSpent() {
    double spent = 0;
    for(int i = 0; i < souvenirs.size(); ++i) {
        spent += this->souvenirs[i].getTotalPrice();
    }
}
double Cart::getTotalSpentAt(QString teamName) {
    double spent = 0;
    for(int i = 0; i < souvenirs.size(); ++i) {
        if(souvenirs[i].getTeamName() == teamName) {
            spent += this->souvenirs[i].getTotalPrice();
        }
    }
}
void Cart::addPurchase(double price, int quantity, QString itemName, QString teamName) {

    Souvenir souvenir;
    Purchase addPurchase;
    souvenir.setPrice(price);
    souvenir.setItemName(itemName);
    souvenir.setTeamName(teamName);
    addPurchase.setSouvenir(souvenir);
    addPurchase.setQuantity(quantity);
    souvenirs.push_back(addPurchase);

}
int Cart::size() {
    return souvenirs.size();
}
double Cart::getTotalPrice(int i) {
    return souvenirs[i].getTotalPrice();
}
int Cart::getQuantity(int i) {
    return souvenirs[i].getQuantity();
}
QString Cart::getTeamName(int i) {

    return souvenirs[i].getTeamName();

}
QString Cart::getSouvenirName(int i) {
    return souvenirs[i].getSouvenir();
}

