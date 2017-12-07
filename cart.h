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
    void addPurchase(double price, int quantity, QString itemName, QString stadiumName);
    double getTotalSpent();
    double getTotalSpentAt(QString stadium);
};

#endif // CART_H
