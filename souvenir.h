#ifndef SOUVENIR_H
#define SOUVENIR_H
#include <QString>
class Souvenir
{
private:
    QString itemName;
    double price;
    int quantity;
    QString teamName;
public:
    Souvenir();
    void setItemName(QString);
    void setPrice(double price);
    void setQuantity(int quantity);
    void setTeamName(QString name);
    int getQuantity() const;
    QString getItemName() const;
    double getPrice() const;
    QString getTeamName() const;

};

#endif // SOUVENIR_H
