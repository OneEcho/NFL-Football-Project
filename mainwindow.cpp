#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include <QSortFilterProxyModel>
#include <QPalette>
#include <QMessageBox>

/*!
 * \fn MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    stadiumMap()
{

    this->admin = new adminWindow;

    ui->setupUi(this);

    Database* DB = Database::getInstance();


    this->populateConferenceDropDownBox("Both");
    this->populateDijkstrasDropDownBox();
    ui->tabWidget->setCurrentIndex(0);
    ui->souvenirTable->hide();
    ui->quantityLabel->hide();
    ui->spinBox->hide();
    ui->cartTable->hide();
    ui->purchaseButton->hide();

    // get the palette
    QPalette palette = ui->lcdNumber->palette();
    ui->lcdNumber->setAutoFillBackground(true);


    palette.setColor(QPalette::Normal, QPalette::Window, Qt::white);
    palette.setColor(palette.Light, Qt::darkGreen /*QColor(255, 0, 0));*/);
    ui->lcdNumber->setPalette(palette);
    ui->lcdNumber->setStyleSheet("border: 1px solid transparent");


    palette = ui->label->palette();
    palette.setColor(ui->label->foregroundRole(), Qt::darkGreen);
    ui->label->setPalette(palette);
    QFont font = ui->label->font();
    font.setPointSize(15);
    ui->label->setFont(font);
    ui->label->setText("Total Seating Capacity: ");

    this->setWindowTitle("NFL Information");

    this->on_BothCheckBox_clicked();


    //trip creator table view
    tripTableViewRowNumber = 0;
    BFStableWidgitRowNumber = 0;
    table = new QStandardItemModel(this);
    //istantiate the QVector of stadiums
    stadiumTrip.clear();
    //reset the stadiumStartingIndex
    currentStadiumIndex = 0;
    //populate the dropdown box initially
    this->populateTripSelectionDropDownBox();\
    this->populateDFSandBFSdropDownBox();
    //hide secondary inputs
    this->hideSecondaryTripInputs();
    ui->nextCollegeButton->setText("Next Stadium");


    stadiumMap.createGraph();
    //stadiumMap.printGraph();

    ui->dijkstrasTableWidget->insertColumn(0);
    ui->dijkstrasTableWidget->insertColumn(1);
    ui->dijkstrasTableWidget->insertColumn(2);

    ui->dijkstrasTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Starting Stadium"));
    ui->dijkstrasTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Ending Stadium"));
    ui->dijkstrasTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Distance"));
    ui->dijkstrasTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->cartTable->setColumnCount(4);
    ui->cartTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Item"));
    ui->cartTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Quantity"));
    ui->cartTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Team Name"));
    ui->cartTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Total Price"));

    ui->totalDistanceLabel->hide();
}

/*!
 * \fn brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \fn MainWindow::populateConferenceDropDownBox
 * \param box
 */
void MainWindow::populateConferenceDropDownBox(QString box)
{
    QSqlQuery query;

    ui->TeamsComboBox->clear();
    ui->TeamsComboBox->addItem("Select A Team");

    /*! \brief If option is AFL */
    if(box == "AFL")
    {
        query.prepare("SELECT distinct TeamName FROM Teams WHERE Conference = \"American Football Conference\" ORDER BY TeamName ASC");

        query.exec();

        while(query.next())
        {
            ui->TeamsComboBox->addItem(query.value(0).toString());
        }
    }
    /*! \brief If option is NFL */
    else if(box  == "NFL")
    {
        query.prepare("SELECT distinct TeamName FROM Teams WHERE Conference = \"National Football Conference\" ORDER BY TeamName ASC");

        query.exec();

        while(query.next())
        {
            ui->TeamsComboBox->addItem(query.value(0).toString());
        }
    }
    /*! \brief If option is both AFL and NFL */
    else if(box == "Both")
    {
        query.prepare("SELECT distinct TeamName FROM Teams ORDER BY TeamName ASC");

        query.exec();

        while(query.next())
        {
            ui->TeamsComboBox->addItem(query.value(0).toString());
        }
    }
}

void MainWindow::populateTripSelectionDropDownBox()
{
    QSqlQuery query;

    ui->tripCreationComboBox->clear();
    ui->tripCreationComboBox->addItem("Select a Stadium");

    query.prepare("SELECT distinct stadiumName FROM Teams");

    query.exec();

    while(query.next())
    {
        ui->tripCreationComboBox->addItem(query.value(0).toString());
    }
}
void MainWindow::showStartingTripInputs()
{
    ui->label_3->show();
    ui->tripCreationComboBox->show();
    ui->addToTripButton->show();
    ui->finishAddingButton->show();
}

void MainWindow::visitAllStadiumsEfficiently(QString startingCity)
{
    QVector<QString> visitedStadiums;
    QVector<Vertex> tempGraph = this->stadiumMap.getGraph();
    int numOfStadiumsVisited = 0;
    int totalDistance = 0;

    QString currentStadium = startingCity;
    QString nextStadium;

    visitedStadiums.push_back(currentStadium);

    // Automatically traverse through entire graph
    // its size - 1 because we dont need to visit the starting city
    while(numOfStadiumsVisited != this->stadiumMap.getGraphSize() - 1)
    {
        // make smallest weight a large number so the first comparison overwrites it
        int smallestWeight = 99999;

        // Traverse through the entire graph to compare the dijkstra weights between
        // curent stadium and all the other stadiums
        for(int i = 0; i < tempGraph.size(); i++)
        {
            bool visited = false;
            int visitIndex = 0;
            while(!visited && visitIndex < visitedStadiums.size())
            {
                if(tempGraph[i].label == visitedStadiums[visitIndex])
                {
                    visited = true;
                }
                else
                {
                    visitIndex++;
                }
            }
            if(currentStadium != tempGraph[i].label && !visited)
            {
                this->stadiumMap.resetShortestPath();
                this->stadiumMap.shortestPathAtVertex(currentStadium, tempGraph[i].label);

                if(this->stadiumMap.getShortestPathWeight() < smallestWeight)
                {
                    smallestWeight = this->stadiumMap.getShortestPathWeight();
                    nextStadium = tempGraph[i].label;
                }
            }
        }

        visitedStadiums.push_back(nextStadium);
        this->stadiumMap.shortestPathAtVertex(currentStadium, nextStadium);
        qDebug() << currentStadium << " to " << nextStadium << " is " << this->stadiumMap.getShortestPathWeight();
        totalDistance += this->stadiumMap.getShortestPathWeight();

        currentStadium = nextStadium;
        numOfStadiumsVisited++;
    }

    for(int i = 0 ; i < visitedStadiums.size(); i++)
    {
        this->stadiumMap.visitStadium(visitedStadiums[i]);
    }
    qDebug() << totalDistance;
}
void MainWindow::hideSecondaryTripInputs()
{
    ui->nextCollegeButton->hide();
    ui->currentCollegeLabel->hide();
    ui->currentStadiumLabel->hide();
    ui->startTripButton->hide();
}

/*!
 * \fn MainWindow::on_AFLCheckBox_clicked
 */
void MainWindow::on_AFLCheckBox_clicked()
{
    if(ui->AFLCheckBox->isChecked())
    {
        ui->NFLCheckBox->setChecked(false);
        ui->BothCheckBox->setChecked(false);
        ui->roofComboBox->setCurrentIndex(0);

        this->populateConferenceDropDownBox("AFL");

        QSqlQueryModel* model = new QSqlQueryModel;

        QSqlQuery query;

        query.prepare("SELECT * FROM Teams WHERE Conference = ? ORDER BY TeamName ASC");
        query.addBindValue("American Football Conference");

        query.exec();

        int total = 0;
        QString temp;

        while(query.next())
        {
            temp = query.value(2).toString();
            temp.remove(",");
            total += temp.toInt();
        }

        ui->lcdNumber->setDigitCount(7);
        ui->lcdNumber->display(QString::number(total));

        model->setQuery(query);

        model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Team Name") );
        model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Stadium Name") );
        model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Seating Capacity") );
        model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Location") );
        model->setHeaderData( 4, Qt::Horizontal, QObject::tr("Conference") );
        model->setHeaderData( 5, Qt::Horizontal, QObject::tr("Surface Type") );
        model->setHeaderData( 6, Qt::Horizontal, QObject::tr("Stadium Roof Type") );
        model->setHeaderData( 7, Qt::Horizontal, QObject::tr("Star Player") );

        /*! \brief allows the table to be sorted by clicking on the header*/
        QSortFilterProxyModel *m = new QSortFilterProxyModel(this);
        m->setDynamicSortFilter(true);
        m->setSourceModel(model);

        ui->ConferenceTableView->setModel(m);
        ui->ConferenceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->ConferenceTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->ConferenceTableView->verticalHeader()->setHidden(true);

        ui->AFLCheckBox->setChecked(true);
    }
    else
    {
        ui->NFLCheckBox->setChecked(false);
        ui->BothCheckBox->setChecked(false);
        ui->AFLCheckBox->setChecked(false);
    }
}

/*!
 * \fn MainWindow::on_NFLCheckBox_clicked
 */
void MainWindow::on_NFLCheckBox_clicked()
{
    ui->AFLCheckBox->setChecked(false);
    ui->BothCheckBox->setChecked(false);
    ui->roofComboBox->setCurrentIndex(0);

    this->populateConferenceDropDownBox("NFL");

    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query;

    query.prepare("SELECT * FROM Teams WHERE Conference = ? ORDER BY TeamName ASC");
    query.addBindValue("National Football Conference");

    query.exec();

    int total = 0;
    QString temp;

    while(query.next())
    {
        temp = query.value(2).toString();
        temp.remove(",");
        total += temp.toInt();
    }

    ui->lcdNumber->setDigitCount(7);
    ui->lcdNumber->display(QString::number(total));


    model->setQuery(query);

    model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Team Name") );
    model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Stadium Name") );
    model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Seating Capacity") );
    model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Location") );
    model->setHeaderData( 4, Qt::Horizontal, QObject::tr("Conference") );
    model->setHeaderData( 5, Qt::Horizontal, QObject::tr("Surface Type") );
    model->setHeaderData( 6, Qt::Horizontal, QObject::tr("Stadium Roof Type") );
    model->setHeaderData( 7, Qt::Horizontal, QObject::tr("Star Player") );

    /*! \brief allows the table to be sorted by clicking on the header*/
    QSortFilterProxyModel *m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->ConferenceTableView->setModel(m);
    ui->ConferenceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ConferenceTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ConferenceTableView->verticalHeader()->setHidden(true);

    ui->NFLCheckBox->setChecked(true);
}

/*!
 * \fn MainWindow::on_BothCheckBox_clicked
 */
void MainWindow::on_BothCheckBox_clicked()
{
    ui->NFLCheckBox->setChecked(false);
    ui->AFLCheckBox->setChecked(false);
    ui->roofComboBox->setCurrentIndex(0);

    this->populateConferenceDropDownBox("Both");

    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query;

    query.prepare("SELECT * FROM Teams ORDER BY TeamName ASC");

    query.exec();

    int total = 0;
    QString temp;

    while(query.next())
    {
        temp = query.value(2).toString();
        temp.remove(",");
        total += temp.toInt();
    }

    total -= 82500;

    ui->lcdNumber->setDigitCount(7);
    ui->lcdNumber->display(QString::number(total));


    model->setQuery(query);

    model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Team Name") );
    model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Stadium Name") );
    model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Seating Capacity") );
    model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Location") );
    model->setHeaderData( 4, Qt::Horizontal, QObject::tr("Conference") );
    model->setHeaderData( 5, Qt::Horizontal, QObject::tr("Surface Type") );
    model->setHeaderData( 6, Qt::Horizontal, QObject::tr("Stadium Roof Type") );
    model->setHeaderData( 7, Qt::Horizontal, QObject::tr("Star Player") );

    /*! \brief allows the table to be sorted by clicking on the header*/
    QSortFilterProxyModel *m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->ConferenceTableView->setModel(m);
    ui->ConferenceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ConferenceTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ConferenceTableView->verticalHeader()->setHidden(true);

    ui->BothCheckBox->setChecked(true);
}

/*!
 * \fn MainWindow::on_TeamsComboBox_currentIndexChanged
 * \param arg1
 */
void MainWindow::on_TeamsComboBox_currentIndexChanged(const QString &arg1)
{
    ui->AFLCheckBox->setChecked(false);
    ui->NFLCheckBox->setChecked(false);
    ui->BothCheckBox->setChecked(false);

    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query;

    query.prepare("SELECT * FROM Teams WHERE TeamName = ?");
    query.addBindValue(arg1);

    query.exec();
    query.next();

    QString temp = query.value(2).toString().remove(",");

    ui->lcdNumber->display(temp);

    model->setQuery(query);

    model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Team Name") );
    model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Stadium Name") );
    model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Seating Capacity") );
    model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Location") );
    model->setHeaderData( 4, Qt::Horizontal, QObject::tr("Conference") );
    model->setHeaderData( 5, Qt::Horizontal, QObject::tr("Surface Type") );
    model->setHeaderData( 6, Qt::Horizontal, QObject::tr("Stadium Roof Type") );
    model->setHeaderData( 7, Qt::Horizontal, QObject::tr("Star Player") );

    /*! \brief allows the table to be sorted by clicking on the header*/
    QSortFilterProxyModel *m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->ConferenceTableView->setModel(m);
    ui->ConferenceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ConferenceTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->ConferenceTableView->verticalHeader()->setHidden(true);
}

void MainWindow::on_roofComboBox_currentIndexChanged(const QString &arg1)
{
    ui->AFLCheckBox->setChecked(false);
    ui->NFLCheckBox->setChecked(false);
    ui->BothCheckBox->setChecked(false);

    QString roofLabelMessage = "Total number of roof type: ";
    int numberOfRoofType = 0;
    //set values to change the lcd seating number
    int total = 0;
    QString temp;

    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query;

    if(ui->roofComboBox->currentText() == "Open")
    {
        query.prepare("SELECT * FROM Teams WHERE StadiumRoofType = ? ORDER BY TeamName ASC");
        query.addBindValue("Open");


        query.exec();

        //geting the size of the query after exec
        while(query.next())
        {
            //incrementing the rooftype
            numberOfRoofType++;
            //adding to the seating number
            temp = query.value(2).toString();
            temp.remove(",");
            total += temp.toInt();

        }
        //subtract the total seating number that the GIANTS and the JETS
        //share the same stadium
        total -= 82500;
        ui->lcdNumber->setDigitCount(7);
        ui->lcdNumber->display(QString::number(total));

        //sets the label to show the number of stadiums with the specific rooftype
        ui->roofNumberLabel->setText(roofLabelMessage + QString::number(numberOfRoofType-1));
        model->setQuery(query);
    }
    else if(ui->roofComboBox->currentText() == "Retractable")
    {
        query.prepare("SELECT * FROM Teams WHERE StadiumRoofType = ? ORDER BY TeamName ASC");
        query.addBindValue("Retractable");
        query.exec();

        //geting the size of the query after exec
        while(query.next())
        {
            numberOfRoofType++;
            //adding to the seating number
            temp = query.value(2).toString();
            temp.remove(",");
            total += temp.toInt();
        }
        ui->lcdNumber->setDigitCount(7);
        ui->lcdNumber->display(QString::number(total));
        //sets the label to show the number of stadiums with the specific rooftype
        ui->roofNumberLabel->setText(roofLabelMessage + QString::number(numberOfRoofType));
        model->setQuery(query);
    }
    else if(ui->roofComboBox->currentText() == "Fixed")
    {
        query.prepare("SELECT * FROM Teams WHERE StadiumRoofType = ? ORDER BY TeamName ASC");
        query.addBindValue("Fixed");
        query.exec();
        //geting the size of the query after exec
        while(query.next())
        {
            numberOfRoofType++;
            //adding to the seating number
            temp = query.value(2).toString();
            temp.remove(",");
            total += temp.toInt();
        }
        ui->lcdNumber->setDigitCount(7);
        ui->lcdNumber->display(QString::number(total));
        //sets the label to show the number of stadiums with the specific rooftype
        ui->roofNumberLabel->setText(roofLabelMessage + QString::number(numberOfRoofType));
        model->setQuery(query);
    }
    else
    {
        query.prepare("SELECT * FROM Teams ORDER BY TeamName ASC");
        query.exec();
        //sets the label to show the number of stadiums with the specific rooftype
        ui->roofNumberLabel->setText(roofLabelMessage);
        model->setQuery(query);
    }

    model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Team Name") );
    model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Stadium Name") );
    model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Seating Capacity") );
    model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Location") );
    model->setHeaderData( 4, Qt::Horizontal, QObject::tr("Conference") );
    model->setHeaderData( 5, Qt::Horizontal, QObject::tr("Surface Type") );
    model->setHeaderData( 6, Qt::Horizontal, QObject::tr("Stadium Roof Type") );
    model->setHeaderData( 7, Qt::Horizontal, QObject::tr("Star Player") );

    /*! \brief allows the table to be sorted by clicking on the header*/
    QSortFilterProxyModel *m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->ConferenceTableView->setModel(m);
    ui->ConferenceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ConferenceTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->ConferenceTableView->verticalHeader()->setHidden(true);

}

/*!
 * \fn MainWindow::on_adminButton_clicked
 */
void MainWindow::on_adminButton_clicked()
{
    // Open Admin window/tab/whatever we are using for the admin page

    Graph *graphPoint = &stadiumMap;
    this->admin->setPointerToGraph(graphPoint);
    this->admin->newShow();
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{   
    //ui->nextCollegeButton->hide();
    //ui->currentCollegeLabel->hide();
    //ui->currentStadiumLabel->hide();
    //ui->startTripButton->hide();


    tripTableViewRowNumber = 0;
    table->setHorizontalHeaderItem(0, new QStandardItem(QString("Stadium Name")) );

    ui->tripTableView->setModel(table);
    ui->tripTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tripTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tripTableView->verticalHeader()->setHidden(true);

    //ui->visitAllStadiumsButton->show();
}

void MainWindow::on_addToTripButton_clicked()
{

    //ui->visitAllStadiumsButton->hide();
    //setting the current index of the combo to this variable
    int choosenStadiumIndex = ui->tripCreationComboBox->currentIndex();

    //grabbing the associative college name
    QSqlQuery query;
    query.prepare("SELECT teamName, stadiumName FROM Teams");
    query.exec();

    collegeStadiumPair addCollege;

    if(choosenStadiumIndex > 0) {
        //set the current text of the choosen box to the stadium as long as the stadium is
        //a valid selection
        QString choosenStadium = ui->tripCreationComboBox->currentText();
        //sets the item in the standarndItemModel class (adds to the table)
        table->setItem(tripTableViewRowNumber ,new QStandardItem(choosenStadium));
        //update the row number
        tripTableViewRowNumber++;
        //remove the stadium from the choosable combobox list
        ui->tripCreationComboBox->removeItem(choosenStadiumIndex);

        //this will associate the college stadium with its college name
        while(query.next()) {
            if(choosenStadium == query.value(1).toString())
            {
                addCollege.stadium = choosenStadium;
                addCollege.college = query.value(0).toString();
            }
        }

        //add the stadium to the vector of stadiums to visit
        stadiumTrip.push_back(addCollege);

    }


    //qDebug() << choosenStadium;
    qDebug() << choosenStadiumIndex;

}

void MainWindow::on_tripCreationComboBox_currentIndexChanged(int index)
{
    if(index > 0)
    {
        ui->tripCreateTeamNameLabel->show();
        QString currentStadium = ui->tripCreationComboBox->currentText();

        //get the team name from the query, that is associated with the index of the
        //combo box
        QSqlQuery query;
        query.prepare("SELECT TeamName FROM Teams WHERE StadiumName = ?");
        query.addBindValue(currentStadium);

        query.exec();

        //for some reason the query wants you to go to the next query
        //to be able to pull the team name info, makes total sense
        while(query.next()) {
            ui->tripCreateTeamNameLabel->setText(query.value(0).toString());
            if(query.value(0).toString() == "New York Jets") {
                ui->tripCreateTeamNameLabel->setText(ui->tripCreateTeamNameLabel->text()+" and Giants");
            }
        }
    }
    else
    {
        ui->tripCreateTeamNameLabel->hide();
    }
}


void MainWindow::on_finishAddingButton_clicked()
{
    int totalDistanceTraveled = 0;
    //ui->nextCollegeButton->show();
    //ui->currentCollegeLabel->show();
    //ui->currentStadiumLabel->show();
    if(stadiumTrip.size() > 0) {
        ui->finishAddingButton->hide();
        ui->dreamVacationButton->hide();
        ui->visitAllStadiumsButton->hide();
        ui->addToTripButton->hide();
        ui->tripCreateTeamNameLabel->hide();
        ui->tripCreationComboBox->hide();
        ui->label_3->hide();
        ui->startTripButton->show();

        //VARIABLES
        QString currentCollege = stadiumTrip[0].college;
        QString currentStadium = stadiumTrip[0].stadium;

        //debugging purpuses
        for(int index = 0; index < stadiumTrip.size(); index++)
        {
            qDebug() << stadiumTrip[index].stadium << " " << stadiumTrip[index].college;
            if(index+1 < stadiumTrip.size()) {
                this->stadiumMap.shortestPathAtVertex(stadiumTrip[index].stadium, stadiumTrip[index+1].stadium);
                totalDistanceTraveled += this->stadiumMap.getShortestPathWeight();
            }
        }

        this->stadiumMap.printVector();
        qDebug() << totalDistanceTraveled;

        table->setItem(tripTableViewRowNumber, new QStandardItem("Total Distance Traveled: " + QString::number(totalDistanceTraveled)));
    }
}

void MainWindow::on_startTripButton_clicked()
{

    ui->nextCollegeButton->show();
    ui->currentCollegeLabel->show();
    ui->currentStadiumLabel->show();
    ui->startTripButton->hide();
    ui->souvenirTable->show();
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT Souvenir, Price FROM Souvenirs WHERE NFLTeam = :NFLTeam");
    query.bindValue(":NFLTeam", stadiumTrip[currentStadiumIndex].college);
    query.exec();
    model->setQuery(query);
    ui->souvenirTable->setModel(model);
    ui->souvenirTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->souvenirTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->souvenirTable->verticalHeader()->setHidden(true);

//    ui->cartTable->show();
//    ui->cartTable->setRowCount(1);
//    ui->cartTable->setColumnCount(1);
//    ui->cartTable->setItem(0, 0, new QTableWidgetItem("Hello"));


    //VARIABLES
    QString currentCollege = stadiumTrip[currentStadiumIndex].college;
    QString currentStadium = stadiumTrip[currentStadiumIndex].stadium;

    ui->currentCollegeLabel->setText(currentCollege);
    ui->currentStadiumLabel->setText(currentStadium);
}

void MainWindow::on_nextCollegeButton_clicked()
{
    qDebug() << "index:: " << currentStadiumIndex;
    qDebug() << "stadiumTripSize:: " << stadiumTrip.size();

    if(currentStadiumIndex < stadiumTrip.size()-1)
    {

        currentStadiumIndex++;
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT Souvenir, Price FROM Souvenirs WHERE NFLTeam = :NFLTeam");
        query.bindValue(":NFLTeam", stadiumTrip[currentStadiumIndex].college);
        query.exec();

        model->setQuery(query);
        ui->souvenirTable->setModel(model);

        ui->souvenirTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
      //  ui->souvenirTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->souvenirTable->verticalHeader()->setHidden(true);
        ui->currentCollegeLabel->setText(stadiumTrip[currentStadiumIndex].college);
        ui->currentStadiumLabel->setText(stadiumTrip[currentStadiumIndex].stadium);

        if(currentStadiumIndex == stadiumTrip.size()-1)
        {
            ui->nextCollegeButton->setText("Finish Trip");
        }
    }
    else
    {
        ui->purchaseButton->hide();
        ui->quantityLabel->hide();
        ui->spinBox->hide();
        ui->nextCollegeButton->hide();
        ui->currentCollegeLabel->setText("You Have Completed Your Trip");
        ui->currentStadiumLabel->setText("Review Your Cart");
        ui->cartTable->show();
        ui->cartTable->setRowCount(purchases.size());
        ui->cartTable->setColumnCount(4);
        for(int i = 0; i < purchases.size(); ++i) {

            ui->cartTable->setItem(i,0, new QTableWidgetItem(purchases.getSouvenirName(i)));
            ui->cartTable->setItem(i,1, new QTableWidgetItem(QString::number(purchases.getQuantity(i))));
            ui->cartTable->setItem(i,2, new QTableWidgetItem((purchases.getTeamName(i))));
            ui->cartTable->setItem(i,3, new QTableWidgetItem(QString::number(purchases.getTotalPrice(i))));
            ui->cartTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
           // ui->cartTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->cartTable->verticalHeader()->setHidden(true);
            ui->souvenirTable->hide();

        }
    }

}

void MainWindow::on_resetTripButton_clicked()
{
    this->showStartingTripInputs();
    this->hideSecondaryTripInputs();

    table->clear();
    ui->dreamVacationButton->show();
    ui->visitAllStadiumsButton->show();
    currentStadiumIndex = 0;
    stadiumTrip.clear();
    this->populateTripSelectionDropDownBox();
    tripTableViewRowNumber = 0;
    ui->nextCollegeButton->setText("Next Stadium");
    this->stadiumMap.resetShortestPath();

    table->setHorizontalHeaderItem(0, new QStandardItem(QString("Stadium Name")) );

    ui->tripTableView->setModel(table);
    ui->tripTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tripTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tripTableView->verticalHeader()->setHidden(true);
}

void MainWindow::populateDijkstrasDropDownBox()
{
    ui->endingStadiumComboBoxDijkstras->hide();
    ui->startingStadiumComboBoxDijkstras->clear();
    ui->startingStadiumComboBoxDijkstras->addItem("Select A Stadium");

    QSqlQuery query;

    query.prepare("SELECT distinct StadiumName From Teams Order By StadiumName ASC");

    query.exec();

    while(query.next())
    {
        ui->startingStadiumComboBoxDijkstras->addItem(query.value(0).toString());
    }

}

void MainWindow::populateDFSandBFSdropDownBox()
{
    QSqlQuery query;

    ui->BFScomboBox->clear();
    ui->DFScomboBox->clear();
    ui->BFScomboBox->addItem("Select a Stadium");
    ui->DFScomboBox->addItem("Select a Stadium");

    query.prepare("SELECT distinct stadiumName FROM Teams");
    query.exec();

    while(query.next())
    {
        ui->DFScomboBox->addItem(query.value(0).toString());
        ui->BFScomboBox->addItem(query.value(0).toString());
    }

}

void MainWindow::on_startingStadiumComboBoxDijkstras_currentIndexChanged(const QString &arg1)
{
    ui->endingStadiumComboBoxDijkstras->setCurrentIndex(0);
    ui->dijkstrasTableWidget->setRowCount(0);
    ui->totalDistanceLabel->hide();

    if(arg1 != "Select A Stadium")
    {
        QSqlQuery query;

        query.prepare("SELECT distinct StadiumName From Teams Order By StadiumName ASC");

        query.exec();

        ui->endingStadiumComboBoxDijkstras->clear();
        ui->endingStadiumComboBoxDijkstras->addItem("Select A Stadium");

        while(query.next())
        {
            if(arg1 != query.value(0).toString())
                ui->endingStadiumComboBoxDijkstras->addItem(query.value(0).toString());
        }

        ui->endingStadiumComboBoxDijkstras->show();
    }
    else
    {
        ui->endingStadiumComboBoxDijkstras->hide();
    }
}

void MainWindow::on_endingStadiumComboBoxDijkstras_currentIndexChanged(const QString &arg1)
{
    ui->dijkstrasTableWidget->setRowCount(0);
    this->stadiumMap.resetShortestPath();
    ui->totalDistanceLabel->hide();

    if(ui->endingStadiumComboBoxDijkstras->currentText() != "Select A Stadium")
    {
        this->stadiumMap.shortestPathAtVertex(ui->startingStadiumComboBoxDijkstras->currentText(),
                                              ui->endingStadiumComboBoxDijkstras->currentText());

        QVector<QString> tempTrip = this->stadiumMap.getShortestPathTraversal();
        QVector<std::pair<QString,QString>> shortestPaths;
        QVector<int> shortestPathsDistances;

        int total = 0;

        QSqlQuery query;


        for(int i = 0; i < tempTrip.size() - 1; i++)
        {
            shortestPaths.push_back(std::pair<QString,QString>(tempTrip[i], tempTrip[i + 1]));

            query.prepare("Select Distance From Distances Where BeginningStadium = :begin AND EndingStadium = :end");
            query.bindValue(":begin", tempTrip[i]);
            query.bindValue(":end",   tempTrip[i+1]);

            query.exec();

            query.next();

            shortestPathsDistances.push_back(query.value(0).toInt());
        }

        for(int i = 0; i < shortestPaths.size(); i++)
        {
            int insertRow = ui->dijkstrasTableWidget->rowCount();

            //insert the row at the bottom of the table widget - using.
            ui->dijkstrasTableWidget->insertRow(insertRow);

            //After a new row is inserted we can add the table widget items as required.
            ui->dijkstrasTableWidget->setItem(insertRow,0,new QTableWidgetItem(shortestPaths[i].first));
            ui->dijkstrasTableWidget->setItem(insertRow,1,new QTableWidgetItem(shortestPaths[i].second));
            ui->dijkstrasTableWidget->setItem(insertRow,2,new QTableWidgetItem(QString::number(shortestPathsDistances[i])));


            total += shortestPathsDistances[i];

        }
        ui->totalDistanceLabel->setText("Total Distance: " + QString::number(total) + " miles");
        ui->totalDistanceLabel->show();
    }
}

void MainWindow::on_visitAllStadiumsButton_clicked()
{
    this->table->clear();
    this->table->setHorizontalHeaderItem(0, new QStandardItem(QString("Stadium Name")) );
    this->table->setRowCount(0);
    this->stadiumMap.resetShortestPath();

    QSqlQuery query;

    query.prepare("SELECT TeamName From Teams where :stadium = StadiumName");

    if(ui->tripCreationComboBox->currentText() != "Select a Stadium")
    {
        this->visitAllStadiumsEfficiently(ui->tripCreationComboBox->currentText());
        QVector<QString> visits = this->stadiumMap.getVisited();
        for(int i = 0; i < visits.size(); i++)
        {
            QString chosenStadium = visits[i];
            collegeStadiumPair temp;
            query.bindValue(":stadium", chosenStadium);
            query.exec();
            query.next();

            //sets the item in the standarndItemModel class (adds to the table)
            table->setItem(tripTableViewRowNumber ,new QStandardItem(chosenStadium));
            //update the row number
            tripTableViewRowNumber++;
            temp.stadium = chosenStadium;
            temp.college = query.value(0).toString();
            this->stadiumTrip.push_back(temp);
        }
        this->stadiumMap.resetVisitedVector();
    }

    this->on_finishAddingButton_clicked();
}

void MainWindow::on_BFSstartButton_clicked()
{
    QString retarded = "BFS traversal";
    if(ui->BFScomboBox->currentIndex() > 0) {
        QString stadiumEntry = ui->BFScomboBox->currentText();
        this->stadiumMap.bfsAtVertex(stadiumEntry);

        ui->BFStable->insertColumn(0);
        ui->BFStable->setHorizontalHeaderItem(0, new QTableWidgetItem(retarded));
        ui->BFStable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        for(int index = 0; index < this->stadiumMap.getTraversalInfoTraversal().size(); index++)
        {
            //ui->BFStable->set
        }


   }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
//    if(index != 1) {
//        ui->hide-
//    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if(arg1 > 0) {
        ui->purchaseButton->show();
    }
    else {
        ui->purchaseButton->hide();
    }
}

void MainWindow::on_souvenirTable_clicked(const QModelIndex &index)
{
    ui->quantityLabel->show();
    ui->spinBox->show();
    ui->spinBox->setMinimum(1);
    souvenirIndex = index;
}

void MainWindow::on_purchaseButton_clicked()
{
 //   ui->cartTable->show();
   // ui->cartTable->show();
    ui->totalSpentWidget->show();
    ui->totalSpentWidget->setColumnCount(2);
    QSqlQuery query;
    query.prepare("Select StadiumName FROM Teams WHERE TeamName = :teamName");
    query.bindValue(":teamName", stadiumTrip[currentStadiumIndex].college);
    query.exec();
    ui->totalSpentWidget->setItem(0,0, new QTableWidgetItem(query.value(0).toString()));
    QModelIndex souvenirNameIndex = souvenirIndex.sibling(souvenirIndex.row(), 0);
    QModelIndex priceIndex = souvenirIndex.sibling(souvenirIndex.row(),1);
    int quantity = ui->spinBox->value();
    QString souvenirName = ui->souvenirTable->model()->data(souvenirNameIndex).toString();
    QString price = ui->souvenirTable->model()->data(priceIndex).toString();

    purchases.addPurchase(price.toDouble(), quantity, souvenirName, stadiumTrip[currentStadiumIndex].college);
    qDebug() << "price: " << price;
    qDebug() << "quantity: " << quantity;
    qDebug() << "souvenir: " << souvenirName;
    qDebug() << "team: " << stadiumTrip[currentStadiumIndex].college;
//    ui->cartTable->setColumnCount(2);
//    ui->cartTable->setRowCount(1);
//    qDebug() << "TEST TOTAL SPENT " << purchases.getTotalSpentAt(stadiumTrip[currentStadiumIndex].college);
//  //  ui->cartTable->setRowCount(currentStadiumIndex+1);
//    ui->cartTable->setItem(0, 0, new QTableWidgetItem("Total Spent"));
//    ui->cartTable->setItem(0, 1, new QTableWidgetItem(QString::number(purchases.getTotalSpentAt(stadiumTrip[currentStadiumIndex].college))));
}
