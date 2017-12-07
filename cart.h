#ifndef CART_H
#define CART_H
#include "purchase.h"
#include <QVector>
#include <QMainWindow>

class Cart
{
private:

    QVector<Purchase> souvenirs;

public:
    void addPurchase(double price, int quantity, QString itemName, QString teamNmae);
    double getTotalSpent();
    double getTotalSpentAt(QString stadium);
    int getQuantity(int i);
    QString getSouvenirName(int i);
    double getTotalPrice(int i);
    QString getTeamName(int i);
    int size();


};

#endif // CART_H
