#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlQueryModel>


/*! \class Database
 *  Our database is a singleton because we dont want accidental copies
 */
class Database: public QSqlDatabase
{
public:
    /*! * \fn getInstance*/
    /*! * \return*/
    static Database* getInstance();

    /*! * \fn addToDatabase*/
    void addToDatabase();

private:
    /*!  \fn Database*/
    /*!  private constructor so it cannot be accessed publicly
    */
    Database();
    /*! * \var instance*/
    /*!  static variable of our singleton instance
     */
    static Database* instance;
};

#endif // DATABSE_H
