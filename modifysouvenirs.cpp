#include "modifysouvenirs.h"
#include "ui_modifysouvenirs.h"

modifysouvenirs::modifysouvenirs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifysouvenirs)
{
    ui->setupUi(this);

    //Sets up the souvenir view when window is opened
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("Souvenirs");
    model->select();

    ui->souvenirView->setModel(model);
    ui->souvenirView->sortByColumn(0, Qt::SortOrder::AscendingOrder);
    ui->souvenirView->resizeColumnsToContents();
    ui->souvenirView->horizontalHeader()->setStretchLastSection(true);

    //Sets up the team combo box
    QSqlQuery teamsList("SELECT distinct NFLTeam FROM Souvenirs ORDER BY NFLTeam");

    teamsList.exec();

    while(teamsList.next())
    {
        ui->teamComboBox->addItem(teamsList.value(0).toString());
    }
}

modifysouvenirs::~modifysouvenirs()
{
    delete ui;
}

void modifysouvenirs::on_addSouvenir_clicked()
{
    QMessageBox msgBox;
    QSqlQuery insertSouvenir;
    QSqlQuery teamSouvenirsQuery;

    if(ui->priceLineEdit->text() == "")
    {
        msgBox.setText("Please enter a price!");
        msgBox.exec();
        return;
    }

    //Performs query to pull the specified team's souvenirs
    teamSouvenirsQuery.prepare("SELECT NFLTeam, Souvenir FROM Souvenirs WHERE NFLTeam=:nflteamname");
    teamSouvenirsQuery.bindValue(":nflteamname", ui->teamComboBox->currentText());
    teamSouvenirsQuery.exec();

    QStringList teamNames;
    QStringList souvenirNames;

    while(teamSouvenirsQuery.next())
    {
        teamNames.append(teamSouvenirsQuery.value("NFLTeam").toString());
        souvenirNames.append(teamSouvenirsQuery.value("Souvenir").toString());
    }

    if(souvenirNames.contains(ui->newSouvenirLineEdit->text()))
    {
        msgBox.setText("Souvenir " + ui->newSouvenirLineEdit->text() + " already exists.");
        msgBox.exec();
        return;
    }

    //Query to insert the new souvenir into the database
    insertSouvenir.prepare("INSERT INTO Souvenirs (NFLTeam, Souvenir, Price)"
                            "VALUES (:teamName, :souvenirName, :price)");
    insertSouvenir.bindValue(":teamName", ui->teamComboBox->currentText());
    insertSouvenir.bindValue(":souvenirName", ui->newSouvenirLineEdit->text());
    insertSouvenir.bindValue(":price", ui->priceLineEdit->text());
    insertSouvenir.exec();
    updateSouvenirView();

    msgBox.setText("Souvenir " + ui->newSouvenirLineEdit->text() + " added successfully!");
    msgBox.exec();
}

void modifysouvenirs::on_deleteSouvenir_clicked()
{
    QSqlQuery deleteQuery;
    QSqlQuery teamSouvenirsQuery;
    QMessageBox msgBox;

    teamSouvenirsQuery.prepare("SELECT Souvenir FROM Souvenirs WHERE NFLTeam=:teamname");
    teamSouvenirsQuery.bindValue(":teamname", ui->teamComboBox->currentText());
    teamSouvenirsQuery.exec();

    //Query to delete the specified souvenir from the database
    deleteQuery.prepare("DELETE FROM Souvenirs WHERE NFLTeam=:teamName AND Souvenir=:souvenirName");
    deleteQuery.bindValue(":teamName", ui->teamComboBox->currentText());
    deleteQuery.bindValue(":souvenirName", ui->souvenirComboBox->currentText());
    deleteQuery.exec();
    updateSouvenirView();

    msgBox.setText("Souvenir " + ui->souvenirComboBox->currentText() + " deleted successfully!");
    msgBox.exec();
}

void modifysouvenirs::on_modifySouvenir_clicked()
{
    QSqlQuery teamSouvenirsQuery;
    QSqlQuery updateSouvenirQuery;
    QMessageBox msgBox;

    if(ui->priceLineEdit->text() == "")
    {
        msgBox.setText("Please enter a price!");
        msgBox.exec();
        return;
    }

    teamSouvenirsQuery.prepare("SELECT Souvenir FROM Souvenirs WHERE NFLTeam=:teamname");
    teamSouvenirsQuery.bindValue(":teamname", ui->teamComboBox->currentText());
    teamSouvenirsQuery.exec();

    //Query to modify the specifed souvenir
    updateSouvenirQuery.prepare("UPDATE Souvenirs SET Price=:price WHERE Souvenir=:souvenirName AND NFLTeam=:teamName");
    updateSouvenirQuery.bindValue(":price", ui->priceLineEdit->text());
    updateSouvenirQuery.bindValue(":souvenirName", ui->souvenirComboBox->currentText());
    updateSouvenirQuery.bindValue(":teamName", ui->teamComboBox->currentText());
    updateSouvenirQuery.exec();

    updateSouvenirView();

    msgBox.setText("Updated " + ui->souvenirComboBox->currentText() + " sucessfully!");
    msgBox.exec();
}

void modifysouvenirs::on_doneButton_clicked()
{
    this->close();
}
void modifysouvenirs::setGraphPointer(Graph *p) {
    graph = p;
}
void modifysouvenirs::on_addSandiegoSailorsButton_clicked()
{
    qDebug() << "ENTERED MODIFY SOUVENIRS\n";
    graph->printGraph();
    //Variables for holding data
    QString teamName, stadiumName, location, conference, surfaceType, stadiumRoofType, starPlayer, seatingCapacity;
    QStringList beginningStadium, endingStadium, distance;

    //Variables to perform querys
    QSqlQuery getSanDiegoDistances;
    QSqlQuery getSanDiegoInfo;
    QSqlQuery insertSanDiegoInfo;
    QSqlQuery insertSanDiegoDistances;

    QMessageBox msgBox;

    //Pulls needed info from database
    getSanDiegoInfo.prepare("SELECT TeamName, StadiumName, SeatingCapacity, Location, Conference, SurfaceType, StadiumRoofType, StarPlayer FROM San_Diego_Info");
    getSanDiegoDistances.prepare("SELECT BeginningStadium, EndingStadium, Distance FROM San_Diego_Distances");
    getSanDiegoInfo.exec();
    getSanDiegoDistances.exec();

    //Adds the needed info to variables
    while(getSanDiegoInfo.next())
    {
        teamName = getSanDiegoInfo.value("TeamName").toString();
        stadiumName = getSanDiegoInfo.value("StadiumName").toString();
        seatingCapacity = getSanDiegoInfo.value("SeatingCapacity").toString();
        location = getSanDiegoInfo.value("Location").toString();
        conference = getSanDiegoInfo.value("Conference").toString();
        surfaceType = getSanDiegoInfo.value("SurfaceType").toString();
        stadiumRoofType = getSanDiegoInfo.value("StadiumRoofType").toString();
        starPlayer = getSanDiegoInfo.value("StarPlayer").toString();
    }

    while(getSanDiegoDistances.next())
    {
        beginningStadium.append(getSanDiegoDistances.value("BeginningStadium").toString());
        endingStadium.append(getSanDiegoDistances.value("EndingStadium").toString());
        distance.append(getSanDiegoDistances.value("Distance").toString());
    }

    //Inserts the information to the database
    insertSanDiegoInfo.prepare("INSERT INTO Teams (TeamName, StadiumName, SeatingCapacity, Location, Conference, SurfaceType, StadiumRoofType, StarPlayer) VALUES (:teamName, :stadiumName, :seatingCapacity, :location, :conference, :surfaceType, :stadiumRoofType, :starPlayer)");
    insertSanDiegoInfo.bindValue(":teamName", teamName);
    insertSanDiegoInfo.bindValue(":stadiumName", stadiumName);
    insertSanDiegoInfo.bindValue(":seatingCapacity", seatingCapacity);
    insertSanDiegoInfo.bindValue(":location", location);
    insertSanDiegoInfo.bindValue(":conference", conference);
    insertSanDiegoInfo.bindValue(":surfaceType", surfaceType);
    insertSanDiegoInfo.bindValue(":stadiumRoofType", stadiumRoofType);
    insertSanDiegoInfo.bindValue(":starPlayer", starPlayer);
    insertSanDiegoInfo.exec();

    for(int i = 0; i < beginningStadium.size(); i++)
    {
        insertSanDiegoDistances.prepare("INSERT INTO Distances (BeginningStadium, EndingStadium, Distance) VALUES (:beginningStadium, :endingStadium, :distance)");
        insertSanDiegoDistances.bindValue(":beginningStadium", beginningStadium[i]);
        insertSanDiegoDistances.bindValue(":endingStadium", endingStadium[i]);
        insertSanDiegoDistances.bindValue(":distance", distance[i]);
        insertSanDiegoDistances.exec();
    }

    graph->updateGraph();
    graph->printGraph();

    msgBox.setText("San Diego Sailors added Successfully!");
    msgBox.exec();
}

void modifysouvenirs::on_teamComboBox_currentIndexChanged(const QString &arg1)
{
    ui->souvenirComboBox->clear();
    QSqlQuery souvenirQuery;
    souvenirQuery.prepare("SELECT DISTINCT Souvenir FROM Souvenirs WHERE NFLTeam=:teamName");
    souvenirQuery.bindValue(":teamName", arg1);
    souvenirQuery.exec();

    while(souvenirQuery.next())
    {
        ui->souvenirComboBox->addItem(souvenirQuery.value(0).toString());
    }
}

void modifysouvenirs::updateSouvenirView()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("Souvenirs");
    model->select();

    ui->souvenirView->setModel(model);
    ui->souvenirView->sortByColumn(0, Qt::SortOrder::AscendingOrder);
    ui->souvenirView->resizeColumnsToContents();
    ui->souvenirView->horizontalHeader()->setStretchLastSection(true);
}
