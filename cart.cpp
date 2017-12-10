#include "cart.h"
#include <QTableWidgetItem>
#include "QSqlQuery"

double Cart::getTotalSpent() {
    double spent = 0;
    for(int i = 0; i < souvenirs.size(); ++i) {
        spent += this->souvenirs[i].getTotalPrice();
    }
    return spent;
}
double Cart::getTotalSpentAt(QString teamName) {
    double spent = 0;
    for(int i = 0; i < souvenirs.size(); ++i) {
        if(souvenirs[i].getTeamName() == teamName) {
            spent += this->souvenirs[i].getTotalPrice();
        }
    }
    return spent;
}
void Cart::addPurchase(double price, int quantity, QString itemName, QString teamName) {

    Souvenir souvenir;
    Purchase addPurchase;
    for(int i = 0; i < souvenirs.size(); ++i) {
        if((souvenirs[i].getTeamName() == teamName) && (itemName == souvenirs[i].getSouvenir())) {
            souvenirs[i].setQuantity(souvenirs[i].getQuantity()+quantity);
            return;
        }
    }
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

