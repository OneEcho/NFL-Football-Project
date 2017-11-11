#include "modifystadiuminfo.h"
#include "ui_modifystadiuminfo.h"
#include "database.h"
#include <QSqlQuery>

modifyStadiumInfo::modifyStadiumInfo(adminWindow &pw, functionName func) :
    parentWindow(pw),
    functor(func),
    ui(new Ui::modifyStadiumInfo)
{
    ui->setupUi(this);

}
modifyStadiumInfo::~modifyStadiumInfo()
{
    delete ui;
}

void modifyStadiumInfo::on_PB_Update_clicked()
{
    QSqlQuery query;
    Database::getInstance();
    // changes price in souvenir table
    query.prepare(QString("UPDATE Teams SET %1 = :newValue WHERE %1 = :oldData ").arg(columnName));
    query.bindValue(":newValue", ui->newData->text());
    query.bindValue(":oldData", oldData);
    query.exec();
    (parentWindow.*functor)();
}
void modifyStadiumInfo::setDataToOverwrite(QString d){
    oldData = d;
}
void modifyStadiumInfo::setColumn(QString c) {
    columnName = c;
    if(c == "SeatingCapacity") {
        QRegExp limitForCap("^[0-9]+$");
        ui->newData->setValidator(new QRegExpValidator(limitForCap, this));
    }
    else {
        QRegExp limit("[A-Za-z(-) ,]{1,100}");
        ui->newData->setValidator(new QRegExpValidator(limit, this));

    }
}
