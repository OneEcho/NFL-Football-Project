#ifndef SOUVENIR_H
#define SOUVENIR_H
#include <QString>
class Souvenir
{
private:
    QString itemName;
    double price;
    QString teamName;
public:
    Souvenir();
    void setItemName(QString);
    void setPrice(double price);
    void setTeamName(QString name);
    QString getItemName() const;
    double getPrice() const;
    QString getTeamName() const;

};

#endif // SOUVENIR_H
