#include "database.h"
#include <QFileInfo>
#include <QSqlError>
#include <math.h>

/*! \fn Database::instance
 * This ensures that our database is a singleton.
 * We only need one copy of the database */
Database* Database::instance = nullptr;

/*! \fn Database::Database
 * //This is our private constructor
 * This ensures that our database is a singleton.
 * We only need one copy of the database */
Database::Database() : QSqlDatabase(addDatabase("QSQLITE"))
{
    this->setDatabaseName("./Database/NFL_Teams.db");/*! \brief add the path to the Database inside quotes*/
    QFileInfo existingDatabaseInfo("./Database/NFL_Teams.db");/*! \brief add th path to the Database inside quotes*/

    if(existingDatabaseInfo.exists()){
        qDebug() << "Database file is open.\n";
        this->open();
        QSqlQuery query;
        query.prepare("PRAGMA foreign_keys = ON");
        query.exec();
    }else{
        qDebug() << "Database file is not open.\n";
    }
}
/*! Database* Database::getInstance(){
 * \fn This returns the instance as a singleton pattern */
Database* Database::getInstance(){
    if(instance == nullptr)     /*! \brief if the instance is still a nullptr*/
    {
        instance = new Database;/*! \brief create a new instance*/
    }
    return instance; /*! \brief if the instance exists, it'll return a copy of the isntance
    // Or if the new instance has been made, it will return that*/
}


