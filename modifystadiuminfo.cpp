#include "modifystadiuminfo.h"
#include "ui_modifystadiuminfo.h"
#include "database.h"
#include <QSqlQuery>
#include "QMessageBox"

modifyStadiumInfo::modifyStadiumInfo(adminWindow &pw, functionName func):
    parentWindow(pw),
    functor(func),
    ui(new Ui::modifyStadiumInfo)
{
    ui->setupUi(this);
    this->setWindowTitle("Update Stadium Information");
}
modifyStadiumInfo::~modifyStadiumInfo()
{
    delete ui;
}

void modifyStadiumInfo::on_PB_Update_clicked()
{
//    QSqlQuery query;
//    Database::getInstance();
//    // changes price in souvenir table
//    qDebug() << "column name: " << this->columnName;
//    qDebug() << "old value: " << this->oldData;
//    qDebug() << "new value: " << ui->newData->text();


//    query.prepare(QString("UPDATE Teams SET %1 = :newValue WHERE TeamName = :teamName AND StadiumName = :stadiumName").arg(columnName));
//    query.bindValue(":newValue", ui->newData->text());
//    query.bindValue(":teamName", teamName);
//    query.bindValue(":stadiumName", stadiumName);
//    query.exec();
//    (parentWindow.*functor)();
//    this->close();
    updateData(ui->newData->text());
}
void modifyStadiumInfo::setData(QString eraseData, QString team, QString stadium, QString column, QString header) {

    if(column == "SeatingCapacity") {
        ui->stackedWidget->setCurrentWidget(ui->TypeNewInfoPage);
        ui->columnName->setText("Enter Updated Seating Capacity");
        QRegExp limitForCap("^[0-9]+$");
        ui->newData->setValidator(new QRegExpValidator(limitForCap, this));
    }
    else if(column == "StadiumRoofType") {
        ui->stackedWidget->setCurrentWidget(ui->ClickPage);
        QStringList list = {"<Select Roof Type Here>", "Open", "Retractable", "Fixed"};
        ui->comboBox->addItems(list);
        ui->label_dropDownHeader->setText("Update Roof Type");
    }
    else if(column == "Conference") {
        ui->stackedWidget->setCurrentWidget(ui->ConferencePage);
        if(oldData == "American Football League") {
            ui->conference_label->setText("Would you like to change the Conference to National Football League?");
        }
        else {
            ui->conference_label->setText("Would you like to change the Conference to American Football League?");
        }
    }
    else {

        QRegExp limit("[A-Za-z(-) ,]{1,100}");
        ui->newData->setValidator(new QRegExpValidator(limit, this));
        ui->stackedWidget->setCurrentWidget(ui->TypeNewInfoPage);
        ui->columnName->setText("Enter Updated " + header);

    }
    columnName = column;
    stadiumName = stadium;
    teamName = team;
    headerName = header;
    oldData = eraseData;
}
void modifyStadiumInfo::on_pushButton_2_clicked()
{
    this->close();
}

void modifyStadiumInfo::on_pushButton_clicked()
{
    QString input = ui->comboBox->currentText();
    if(input == "<Select Roof Type Here>") {
        QMessageBox::critical(this, "Incorrect Selection", "Please Select a Valid Roof Type!");
    }
    else {
        updateData(input);
    }
}

void modifyStadiumInfo::on_PB_Conference_clicked()
{
    if(oldData == "American Football Conference") {
        updateData("National Football Conference");
    }
    else {
        updateData("American Football Conference");
    }
    ui->PB_Conference->setFocus();
}
void modifyStadiumInfo::updateData(QString newData) {
    QSqlQuery query;
    Database::getInstance();
    // changes price in souvenir table

    int errorCount = 0;
    for(int i = 0;i < newData.size(); ++i) {
        if(newData.at(i) == " ") {
            errorCount++;
        }
    }
    qDebug() << "newData.size(): " << newData.size();
    qDebug() << "errorCount " << errorCount;

    if(errorCount == newData.size()) {
        QMessageBox::critical(this, "Invalid", "Please Enter a Valid " + headerName);
        ui->newData->clear();

    }
    else {
        query.prepare(QString("UPDATE Teams SET %1 = :newValue WHERE TeamName = :teamName AND StadiumName = :stadiumName").arg(columnName));
        query.bindValue(":newValue", newData);
        query.bindValue(":teamName", teamName);
        query.bindValue(":stadiumName", stadiumName);
        query.exec();
        (parentWindow.*functor)();
        this->close();
    }
}

void modifyStadiumInfo::on_cancel_conference_clicked()
{
    this->close();
}

void modifyStadiumInfo::on_PB_Conference_pressed()
{
    on_PB_Conference_clicked();
}
