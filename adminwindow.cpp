#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "database.h"
#include "modifystadiuminfo.h"
#include <QMessageBox>

adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Admin Window");

}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::newShow()
{

    ui->Pages->setCurrentIndex(0);
     this->show();
}

void adminWindow::on_passwordLineEdit_returnPressed()
{
    if(ui->usernameLineEdit->text() == "admin" &&
       ui->passwordLineEdit->text() == "password")
    {
        ui->Pages->setCurrentIndex(1);
//        ui->Pages->setCurrentIndex(3);
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
        //this->show();
        QRegExp limit("[A-Za-z(-) ,]{1,100}");
        QRegExp limitForCap("^[0-9]+$");

        QSqlQueryModel* model = new QSqlQueryModel();

         ui->lineEditConference->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditTeamName->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditSeatingCap->setValidator(new QRegExpValidator(limitForCap, this));
         ui->lineEditSurfaceType->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditLocation->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditStadiumName->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditRoofType->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditStarPleyer->setValidator(new QRegExpValidator(limit, this));

//        ui->Pages->setCurrentWidget(ui->modifyStadiumPage);
        model = Database::getInstance()->getStadiumInfo();
        model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Team Name") );
        model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Stadium Name") );
        model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Seating Capacity") );
        model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Location") );
        model->setHeaderData( 4, Qt::Horizontal, QObject::tr("Conference") );
        model->setHeaderData( 5, Qt::Horizontal, QObject::tr("Surface Type") );
        model->setHeaderData( 6, Qt::Horizontal, QObject::tr("Stadium Roof Type") );
        model->setHeaderData( 7, Qt::Horizontal, QObject::tr("Star Player") );
        ui->stadiumTableView->verticalHeader()->setHidden(true);
        ui->stadiumTableView->setModel(model);
        ui->stadiumTableView->resizeColumnsToContents();
        ui->stadiumTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    }
    else
    {
        QMessageBox::critical(this, "Incorrect Login", "Incorrect login information. Try again.");
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }


}

void adminWindow::on_cancelLoginButton_clicked()
{
    this->hide();
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
}

void adminWindow::on_loginButton_clicked()
{
    if(ui->usernameLineEdit->text() == "admin" &&
       ui->passwordLineEdit->text() == "password")
    {
        ui->Pages->setCurrentIndex(1);
//        ui->Pages->setCurrentIndex(3);
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
        //this->show();
        QRegExp limit("[A-Za-z(-) ,]{1,100}");
        QRegExp limitForCap("^[0-9]+$");

        QSqlQueryModel* model = new QSqlQueryModel();

         ui->lineEditConference->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditTeamName->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditSeatingCap->setValidator(new QRegExpValidator(limitForCap, this));
         ui->lineEditSurfaceType->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditLocation->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditStadiumName->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditRoofType->setValidator(new QRegExpValidator(limit, this));
         ui->lineEditStarPleyer->setValidator(new QRegExpValidator(limit, this));

//        ui->Pages->setCurrentWidget(ui->modifyStadiumPage);
        model = Database::getInstance()->getStadiumInfo();
        model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Team Name") );
        model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Stadium Name") );
        model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Seating Capacity") );
        model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Location") );
        model->setHeaderData( 4, Qt::Horizontal, QObject::tr("Conference") );
        model->setHeaderData( 5, Qt::Horizontal, QObject::tr("Surface Type") );
        model->setHeaderData( 6, Qt::Horizontal, QObject::tr("Stadium Roof Type") );
        model->setHeaderData( 7, Qt::Horizontal, QObject::tr("Star Player") );
        ui->stadiumTableView->verticalHeader()->setHidden(true);
        ui->stadiumTableView->setModel(model);
        ui->stadiumTableView->resizeColumnsToContents();
        ui->stadiumTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    }
    else
    {
        QMessageBox::critical(this, "Incorrect Login", "Incorrect login information. Try again.");
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }

}

void adminWindow::on_usernameLineEdit_returnPressed()
{
    if(ui->usernameLineEdit->text() == "admin" &&
       ui->passwordLineEdit->text() == "password")
    {
        ui->Pages->setCurrentIndex(1);
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
    else
    {
        QMessageBox::critical(this, "Incorrect Login", "Incorrect login information. Try again.");
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }

}

void adminWindow::on_pushButtonAddStadium_clicked()
{
    bool invalidInfo = false;

    QString inputData[8];
    inputData[0] = ui->lineEditTeamName->text();
    inputData[1] = ui->lineEditStadiumName->text();
    inputData[2] = ui->lineEditSeatingCap->text();
    inputData[3] = ui->lineEditLocation->text();
    inputData[4] = ui->lineEditConference->text();
    inputData[5] = ui->lineEditSurfaceType->text();
    inputData[6] = ui->lineEditRoofType->text();
    inputData[7] = ui->lineEditStarPleyer->text();
    int index = 0;
    while(!invalidInfo && index < 8) {
        if(inputData[index] == "") {
            invalidInfo = true;
            QMessageBox::critical(this, "Invalid Information", "Please insert information Into all columns");
            ui->lineEditConference->setText("<Enter Conference>");
            ui->lineEditTeamName->setText("<Enter Team>");
            ui->lineEditSeatingCap->setText("<Enter Capacity>");
            ui->lineEditSurfaceType->setText("<Enter Surface Type>");
            ui->lineEditLocation->setText("<Enter Location>");
            ui->lineEditStadiumName->setText("<Enter Stadium>");
            ui->lineEditRoofType->setText("<Enter Roof Type>");
            ui->lineEditStarPleyer->setText("<Enter Star Player>");

        }
        index++;
    }

    if(!invalidInfo) {
        QSqlQuery query;
        query.prepare("INSERT INTO `Teams` VALUES (:teamName,:stadiumName,:capacity,:location,:conference,:surfaceType,:roofType,:starPlayer)");
        query.bindValue(":teamName", inputData[0]);
        query.bindValue(":stadiumName", inputData[1]);
        query.bindValue(":capacity", inputData[2]);
        query.bindValue(":location", inputData[3]);
        query.bindValue(":conference", inputData[4]);
        query.bindValue(":surfaceType", inputData[5]);
        query.bindValue(":roofType", inputData[6]);
        query.bindValue(":starPlayer", inputData[7]);
        query.exec();
    }
    this->newShow();

}
void adminWindow::on_stadiumTableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex campusIndex = index.sibling(index.row(), index.column());
    QString dataToOverwrite = ui->stadiumTableView->model()->data(campusIndex).toString();
    qDebug() << "dataToOverwrite: " << dataToOverwrite;

    // convert selection into a qstring
    QString columnName;
    switch(index.column()) {
        case 0: columnName = "TeamName";
            break;
        case 1: columnName = "StadiumName";
            break;
        case 2: columnName = "SeatingCapacity";
             break;
        case 3: columnName = "Location";
             break;
        case 4: columnName = "Conference";
             break;
        case 5: columnName = "SurfaceType";
            break;
        case 6: columnName = "StadiumRoofType";
            break;
        case 7: columnName = "StarPlayer";
            break;
        default: columnName = "";
    }
    modifyStadiumInfo *newInput = new modifyStadiumInfo(*this, &adminWindow::newShow);
    // store index from tableview selection
    newInput->setColumn(columnName);
    newInput->setDataToOverwrite(dataToOverwrite);


    newInput->exec();
    delete newInput;
}
