#ifndef CART_H
#define CART_H
#include "souvenir.h"
#include <QVector>
#include <QMainWindow>

class Cart
{
private:

    QVector<Souvenir> souvenirs;

public:
    void addPurchase(double price, int quantity, QString itemName, QString teamName);
    double getTotalSpent();
    double getTotalSpentAt(QString college);
};

#endif // CART_H
