#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "database.h"
#include "modifystadiuminfo.h"
#include <QMessageBox>

adminWindow::adminWindow(MainWindow &parent, functionName funcT) :
    parentWindow(parent),
    func(funcT),
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
        openStadiumModifyPage();
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
    this->openStadiumModifyPage();

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
    const int AR_SIZE = 8;
    QString inputData[AR_SIZE];
    inputData[0] = ui->lineEditTeamName->text();
    inputData[1] = ui->lineEditStadiumName->text();
    inputData[2] = ui->lineEditSeatingCap->text();
    inputData[3] = ui->lineEditLocation->text();
    inputData[4] = ui->comboBoxConference->currentText();
    inputData[5] = ui->lineEditSurfaceType->text();
    inputData[6] = ui->comboBoxRoofType->currentText();
    inputData[7] = ui->lineEditStarPleyer->text();
    int index = 0;
    int errorCount = 0;
    while(!invalidInfo && index < AR_SIZE) {
        for(int k = 0; k < inputData[index].size(); ++k) {
            if(inputData[index].at(k) == " ") {
                errorCount++;
            }
        }
        if(errorCount == inputData[index].size()) {
            invalidInfo = true;
            QMessageBox::critical(this, "Invalid Information", "Please Insert Valid Information");

        }
        index++;
        errorCount = 0;
    }
    if((inputData[4] == "<Select Conference Here>" || inputData[6] == "<Select Roof Type Here>")&& !invalidInfo) {
        QMessageBox::critical(this, "Invalid Information", "Please Select Valid Options In Drop Down Menus");

    }
    else if(!invalidInfo){

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
            graphPointer->updateGraph();

    }
    this->openStadiumModifyPage();


}
void adminWindow::on_stadiumTableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex teamNameIndex = index.sibling(index.row(), 0);
    QModelIndex stadiumNameIndex = index.sibling(index.row(),1);

    QString header;
    QModelIndex columnToUpdate = index.sibling(index.row(), index.column());
    QString dataToOverwrite = ui->stadiumTableView->model()->data(columnToUpdate).toString();
    QString teamName = ui->stadiumTableView->model()->data(teamNameIndex).toString();
    QString stadiumName = ui->stadiumTableView->model()->data(stadiumNameIndex).toString();
    qDebug() << "dataToOverwrite: " << dataToOverwrite;
      qDebug() << "teamName: " << teamName;
      qDebug() << "stadiumName: " << stadiumName;

    // convert selection into a qstring
    QString columnName = "";
    switch(index.column()) {
        case 0: columnName = "TeamName";
                header = "Team Name";
            break;
        case 1: columnName = "StadiumName";
                header = "Stadium Name";
            break;
        case 2: columnName = "SeatingCapacity";
                header = "Seating Capacity";
             break;
        case 3: columnName = "Location";
                header = "Location";
             break;
        case 4: columnName = "Conference";
                header = "Conference";
             break;
        case 5: columnName = "SurfaceType";
                header = "Surface Type";
            break;
        case 6: columnName = "StadiumRoofType";
                header = "Roof Type";
            break;
        case 7: columnName = "StarPlayer";
                header = "Star Player";
            break;
        default: columnName = "";
    }
    if(columnName != "") {

        modifyStadiumInfo *newInput = new modifyStadiumInfo(*this, &adminWindow::updateModifyTable);
        // store index from tableview selection
        newInput->setGraphPointerModify(graphPointer);
        newInput->setData(dataToOverwrite, teamName, stadiumName, columnName, header);

        newInput->exec();

        delete newInput;
    }

}
void adminWindow::setPointerToGraph(Graph *p) {
    graphPointer = p;
}
void adminWindow::openStadiumModifyPage() {
    ui->Pages->setCurrentIndex(1);

    QRegExp limit("[A-Za-z(-) ,]{1,100}");
    QRegExp limitForCap("^[0-9]+$");
    QStringList conferences = {"<Select Conference Here>", "American Football Conference", "National Football Conference"};
    QStringList roofTypes = {"<Select Roof Type Here>", "Open", "Retractable", "Fixed"};

    ui->comboBoxConference->clear();
    ui->comboBoxRoofType->clear();
    ui->comboBoxConference->addItems(conferences);
    ui->comboBoxRoofType->addItems(roofTypes);
    ui->comboBoxConference->setCurrentIndex(0);
    ui->comboBoxRoofType->setCurrentIndex(0);
    ui->lineEditLocation->clear();
    ui->lineEditSeatingCap->clear();
    ui->lineEditStadiumName->clear();
    ui->lineEditStarPleyer->clear();
    ui->lineEditSurfaceType->clear();
    ui->lineEditTeamName->clear();
    QSqlQueryModel* model = new QSqlQueryModel();

     ui->lineEditTeamName->setValidator(new QRegExpValidator(limit, this));
     ui->lineEditSeatingCap->setValidator(new QRegExpValidator(limitForCap, this));
     ui->lineEditSurfaceType->setValidator(new QRegExpValidator(limit, this));
     ui->lineEditLocation->setValidator(new QRegExpValidator(limit, this));
     ui->lineEditStadiumName->setValidator(new QRegExpValidator(limit, this));
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
void adminWindow::on_modifySouvenirsButton_clicked()
{
    modifySouvenirsWindow = new modifysouvenirs(*this, &adminWindow::updateTable);
    modifySouvenirsWindow->show();
    modifySouvenirsWindow->setGraphPointer(graphPointer);
}
void adminWindow::updateTable() {
    QSqlQueryModel *model = Database::getInstance()->getStadiumInfo();
    ui->stadiumTableView->setModel(model);
    (parentWindow.*func)();
}
void adminWindow::updateModifyTable() {
    (parentWindow.*func)();

    QSqlQueryModel *model = new QSqlQueryModel();
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
