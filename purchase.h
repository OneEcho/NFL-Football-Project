#ifndef PURCHASE_H
#define PURCHASE_H
#include "souvenir.h"

class Purchase
{
    Souvenir souvenir;
    int quantity;

public:
    Purchase();
    int getQuantity();
    void setQuantity(double);
    void setSouvenir(Souvenir s);
    double getTotalPrice() const;
    QString getTeamName() const;
};

#endif // PURCHASE_H
