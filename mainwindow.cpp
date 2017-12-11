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
    ui->labelClickSouvenir->hide();

    ui->totalSpentWidget->hide();
    ui->quantityLabel->hide();
    ui->spinBox->hide();
    ui->cartTable->hide();
    ui->purchaseButton->hide();

    purchases = NULL;

    // get the palette
    QPalette palette = ui->lcdNumber->palette();
    ui->lcdNumber->setAutoFillBackground(true);
    totalAmountRowIndex = 0;

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
    //DFS and BFS table settings
    ui->bfsClearButton->hide();
    ui->DFSclearButton->hide();
    ui->BFStable->insertColumn(0);
    ui->BFStable->setHorizontalHeaderItem(0, new QTableWidgetItem("BFS traversal"));
    ui->BFStable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->DFStable->insertColumn(0);
    ui->DFStable->setHorizontalHeaderItem(0, new QTableWidgetItem("DFS traversal"));
    ui->DFStable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



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
    ui->totalSpentWidget->setColumnCount(2);
    ui->totalSpentWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Stadium"));
    ui->totalSpentWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Total Spent"));
    //ui->totalSpentWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Total Stadiums Visited"));

    ui->totalSpentWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // ui->cartTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->totalSpentWidget->verticalHeader()->setHidden(true);
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

void MainWindow::visitAllStadiumsEfficiently(QString startingStadium, QStringList stadiumsToVisit, QVector<QString> &visitedStadiums)
{
    if(stadiumsToVisit.size() == visitedStadiums.size())
    {
        return;
    }

    QVector<Vertex> tempGraph = this->stadiumMap.getGraph();
    bool found = false;
    int index = 0;
    QString nextStadium;
    int smallestWeight = 99999;

    while(!found && index < tempGraph.size())
    {
        if(startingStadium == tempGraph[index].label)
        {
            found = true;
        }
        else
        {
            index++;
        }
    }

    for(int i = 0; i < stadiumsToVisit.size(); i++)
    {
        bool visited = false;
        int visitIndex = 0;

        while(!visited && visitIndex < visitedStadiums.size())
        {
            if(stadiumsToVisit[i] == visitedStadiums[visitIndex])
            {
                visited = true;
            }
            else
            {
                visitIndex++;
            }
        }
        if(startingStadium != stadiumsToVisit[i] && !visited)
        {
            this->stadiumMap.resetShortestPath();
            this->stadiumMap.shortestPathAtVertex(startingStadium, stadiumsToVisit[i]);

            if(this->stadiumMap.getShortestPathWeight() < smallestWeight)
            {
                smallestWeight = this->stadiumMap.getShortestPathWeight();
                nextStadium = stadiumsToVisit[i];
            }
        }
    }
    visitedStadiums.push_back(nextStadium);
    this->stadiumMap.shortestPathAtVertex(startingStadium, nextStadium);
    qDebug() << startingStadium << " to " << nextStadium << " is " << this->stadiumMap.getShortestPathWeight();
    this->totalDistance += this->stadiumMap.getShortestPathWeight();

    visitAllStadiumsEfficiently(nextStadium, stadiumsToVisit, visitedStadiums);

}
void MainWindow::hideSecondaryTripInputs()
{
    ui->nextCollegeButton->hide();
    ui->currentCollegeLabel->hide();
    ui->currentStadiumLabel->hide();
    ui->startTripButton->hide();
}

void MainWindow::dreamVacation(QString startStadium, QStringList tripList, QStringList & visitedStadiums)
{
    if(tripList.size() == visitedStadiums.size())
    {
        return;
    }

    QVector<Vertex> tempGraph = this->stadiumMap.getGraph();
    bool found = false;
    int index = 0;
    QString nextStadium;
    int smallestWeight = 99999;

    while(!found && index < tempGraph.size())
    {
        if(startStadium == tempGraph[index].label)
        {
            found = true;
        }
        else
        {
            index++;
        }
    }

    for(int i = 0; i < tripList.size(); i++)
    {
        bool visited = false;
        int visitIndex = 0;

        while(!visited && visitIndex < visitedStadiums.size())
        {
            if(tripList[i] == visitedStadiums[visitIndex])
            {
                visited = true;
            }
            else
            {
                visitIndex++;
            }
        }
        if(startStadium != tripList[i] && !visited)
        {
            this->stadiumMap.resetShortestPath();
            this->stadiumMap.shortestPathAtVertex(startStadium, tripList[i]);

            if(this->stadiumMap.getShortestPathWeight() < smallestWeight)
            {
                smallestWeight = this->stadiumMap.getShortestPathWeight();
                nextStadium = tripList[i];
            }
        }
    }
    visitedStadiums.push_back(nextStadium);
    this->stadiumMap.shortestPathAtVertex(startStadium, nextStadium);
    qDebug() << startStadium << " to " << nextStadium << " is " << this->stadiumMap.getShortestPathWeight();
    this->totalDistance += this->stadiumMap.getShortestPathWeight();

    dreamVacation(nextStadium, tripList, visitedStadiums);
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

    this->listTrip.push_back(ui->tripCreationComboBox->currentText());

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
    ui->totalSpentWidget->setRowCount(0);
    ui->cartTable->setColumnCount(4);
    ui->cartTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Item"));
    ui->cartTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Quantity"));
    ui->cartTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Team Name"));
    ui->cartTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Total Price"));

    ui->totalDistanceLabel->hide();
    ui->totalSpentWidget->setColumnCount(2);
    ui->totalSpentWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Stadium"));
    ui->totalSpentWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Total Spent"));
    if(purchases != NULL) {
        delete purchases;

    }
    purchases = new Cart();
    souvenirSelected = false;
    totalAmountRowIndex = 1;
    ui->nextCollegeButton->show();
    ui->currentCollegeLabel->show();
    ui->currentStadiumLabel->show();
    ui->startTripButton->hide();
    ui->souvenirTable->show();
    ui->totalSpentWidget->show();
    ui->labelClickSouvenir->show();

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
        ui->purchaseButton->hide();
        ui->quantityLabel->hide();
        ui->spinBox->hide();
        currentStadiumIndex++;
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT Souvenir, Price FROM Souvenirs WHERE NFLTeam = :NFLTeam");
        query.bindValue(":NFLTeam", stadiumTrip[currentStadiumIndex].college);
        query.exec();
        if(souvenirSelected) {
            totalAmountRowIndex++;
            souvenirSelected = false;
        }
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
        qDebug() << "ENTERED: ERROR CHECK";
        qDebug() << purchases->size();

        ui->totalSpentWidget->hide();
        ui->purchaseButton->hide();
        ui->quantityLabel->hide();
        ui->souvenirTable->hide();
        ui->labelClickSouvenir->hide();

        ui->spinBox->hide();
        ui->nextCollegeButton->hide();
        ui->currentCollegeLabel->setText("You Have Completed Your Trip");
        ui->currentStadiumLabel->setText("Review Your Cart");
        ui->cartTable->show();
        ui->cartTable->setRowCount(purchases->size()+1);
        ui->cartTable->setColumnCount(4);
        for(int i = 0; i < purchases->size(); ++i) {

            ui->cartTable->setItem(i,0, new QTableWidgetItem(purchases->getSouvenirName(i)));
            ui->cartTable->setItem(i,1, new QTableWidgetItem(QString::number(purchases->getQuantity(i))));
            ui->cartTable->setItem(i,2, new QTableWidgetItem((purchases->getTeamName(i))));
            ui->cartTable->setItem(i,3, new QTableWidgetItem(QString::number(purchases->getTotalPrice(i))));
            ui->cartTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            // ui->cartTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->cartTable->verticalHeader()->setHidden(true);


        }
        ui->cartTable->setItem(ui->cartTable->rowCount()-1, 3, new QTableWidgetItem(QString::number(purchases->getTotalSpent())));
    }

}

void MainWindow::on_resetTripButton_clicked()
{
    totalAmountRowIndex = 0;
    this->showStartingTripInputs();
    this->hideSecondaryTripInputs();

    ui->cartTable->clear();
    ui->totalSpentWidget->clear();
    //ui->souvenirTable->
    ui->cartTable->hide();
    ui->totalSpentWidget->hide();
    ui->souvenirTable->hide();
    ui->labelClickSouvenir->hide();


    ui->purchaseButton->hide();
    ui->quantityLabel->hide();
    ui->spinBox->hide();
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
    this->listTrip.clear();
}


void MainWindow::populateDijkstrasDropDownBox()
{
    int index = 0;

    ui->endingStadiumComboBoxDijkstras->hide();
   //if(ui->startingStadiumComboBoxDijkstras->count() > 1) {
        ui->startingStadiumComboBoxDijkstras->setMaxCount(++index);

   // }
    ui->startingStadiumComboBoxDijkstras->addItem("Select A Stadium");

    QSqlQuery query;

    query.prepare("SELECT distinct StadiumName From Teams Order By StadiumName ASC");

    query.exec();
    while(query.next())
    {
        ui->startingStadiumComboBoxDijkstras->setMaxCount(++index);

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
    qDebug() << this->stadiumTrip.size();
    if(stadiumTrip.size() == 0)
    {
        this->table->clear();
        this->table->setHorizontalHeaderItem(0, new QStandardItem(QString("Stadium Name")) );
        this->table->setRowCount(0);
        this->stadiumMap.resetShortestPath();

        QSqlQuery query;

        query.prepare("SELECT TeamName From Teams where :stadium = StadiumName");

        if(ui->tripCreationComboBox->currentText() != "Select a Stadium")
        {
            this->listTrip.clear();

            for(int i = 0; i < this->stadiumMap.getGraphSize(); i++)
            {
                if(ui->tripCreationComboBox->currentText() !=
                        this->stadiumMap.getGraph()[i].label)
                    this->listTrip.push_back(this->stadiumMap.getGraph()[i].label);
            }


            QVector<QString> visits;
            visits.push_back(ui->tripCreationComboBox->currentText());
            this->visitAllStadiumsEfficiently(ui->tripCreationComboBox->currentText(), this->listTrip, visits);
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
}

void MainWindow::on_BFSstartButton_clicked()
{
    QString totalMessage = "Total Distance Traveled: ";
    int traversalDistance;
    int rowCount = 0;
    QStringList traversal;


    if(ui->BFScomboBox->currentIndex() > 0) {
        QString stadiumEntry = ui->BFScomboBox->currentText();
        this->stadiumMap.bfsAtVertex(stadiumEntry);

        traversal = this->stadiumMap.getTraversalInfoTraversal();
        traversalDistance = this->stadiumMap.getTraversalInfoDistance();

        ui->bfsClearButton->show();


        for(rowCount; rowCount < traversal.size(); rowCount++)
        {
            ui->BFStable->insertRow(rowCount);
            ui->BFStable->setItem(rowCount, 0, new QTableWidgetItem(traversal[rowCount]));
        }

        ui->BFStable->insertRow(rowCount);
        ui->BFStable->setItem(rowCount, 0, new QTableWidgetItem(totalMessage + QString::number(traversalDistance)));

    ui->BFSstartButton->setEnabled(false);
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
    ui->purchaseButton->show();
    souvenirIndex = index;
}

void MainWindow::on_purchaseButton_clicked()
{
    //   ui->cartTable->show();
    // ui->cartTable->show();
    ui->totalSpentWidget->show();
    ui->totalSpentWidget->setRowCount(totalAmountRowIndex);

    souvenirSelected = true;
    // get data from ui
    QModelIndex souvenirNameIndex = souvenirIndex.sibling(souvenirIndex.row(), 0);
    QModelIndex priceIndex = souvenirIndex.sibling(souvenirIndex.row(),1);
    int quantity = ui->spinBox->value();
    QString souvenirName = ui->souvenirTable->model()->data(souvenirNameIndex).toString();
    QString price = ui->souvenirTable->model()->data(priceIndex).toString();

    // add purchase
    purchases->addPurchase(price.toDouble(), quantity, souvenirName, stadiumTrip[currentStadiumIndex].college);
    qDebug() << "price: " << price;
    qDebug() << "quantity: " << quantity;
    qDebug() << "souvenir: " << souvenirName;
    qDebug() << "team: " << stadiumTrip[currentStadiumIndex].college;

    QSqlQuery query;

    // display stadium and total spent on widget
    query.prepare("SELECT StadiumName FROM Teams WHERE TeamName = :teams");
    // qDebug() << "TEST FOR PURCHASE" << purchases.getTeamName(i);

    query.bindValue(":teams", stadiumTrip[currentStadiumIndex].college);
    query.exec();

    while(query.next())
    {

        QString stadiumName = query.value(0).toString();
        qDebug() << "TEST FOR PURCHASE" << stadiumName;
        ui->totalSpentWidget->setItem(totalAmountRowIndex-1, 0, new QTableWidgetItem(stadiumName));
        ui->totalSpentWidget->setItem(totalAmountRowIndex-1, 1, new QTableWidgetItem(QString::number(purchases->getTotalSpentAt(stadiumTrip[currentStadiumIndex].college))));
        //ui->totalSpentWidget->setItem(i, 2, new QTableWidgetItem(QString::number(stadiumTrip.size())));
    }
    ui->spinBox->setValue(1);

    //    ui->cartTable->setColumnCount(2);
    //    ui->cartTable->setRowCount(1);
    //    qDebug() << "TEST TOTAL SPENT " << purchases.getTotalSpentAt(stadiumTrip[currentStadiumIndex].college);
    //  //  ui->cartTable->setRowCount(currentStadiumIndex+1);
    //    ui->cartTable->setItem(0, 0, new QTableWidgetItem("Total Spent"));
    //    ui->cartTable->setItem(0, 1, new QTableWidgetItem(QString::number(purchases.getTotalSpentAt(stadiumTrip[currentStadiumIndex].college))));
}
void MainWindow::on_dreamVacationButton_clicked()
{
    QStringList temp;
    if(ui->tripCreationComboBox->currentText() != "Select a Stadium")
    {
        if(tripTableViewRowNumber == 0)
            return;

        QSqlQuery query;

        query.prepare("SELECT TeamName From Teams where :stadium = StadiumName");

        temp.push_back(this->listTrip[0]);
        dreamVacation(this->listTrip[0], this->listTrip, temp);

        this->stadiumTrip.clear();


        this->table->clear();
        this->table->setHorizontalHeaderItem(0, new QStandardItem(QString("Stadium Name")) );
        this->table->setRowCount(0);
        qDebug() << temp.size();
        this->tripTableViewRowNumber = this->table->rowCount();
        for(int i = 0; i < temp.size(); i++)
        {
            collegeStadiumPair pair;

            query.bindValue(":stadium", temp[i]);
            query.exec();
            query.next();

            qDebug() << temp[i];
            table->setItem(tripTableViewRowNumber ,new QStandardItem(temp[i]));
            tripTableViewRowNumber++;
            pair.stadium = temp[i];
            pair.college = query.value(0).toString();
            this->stadiumTrip.push_back(pair);

        }
        this->on_finishAddingButton_clicked();

    }
}

void MainWindow::on_MSTButton_clicked()
{
    ui->startingStadiumComboBoxDijkstras->setCurrentIndex(0);
    QVector<Vertex> tempList = this->stadiumMap.getGraph();

    QVector<QString> visitedCities;
    QVector<std::pair<QString,QString>> visitedEdges;
    QVector<int> visitedCitiesWeight;
    int totalMileage = 0;
    int numberOfCitiesFound = 0;
    QString closestCity;

    QString firstCity = tempList[0].label;

    bool found = false;
    int currentCityIndex = 0;
    while(!found && currentCityIndex < tempList.size())
    {
        if(tempList[currentCityIndex].label == firstCity)
        {
            found = true;
        }
        else
        {
            currentCityIndex++;
        }
    }


    closestCity = tempList[currentCityIndex].label;

    // pushing back the city found
    visitedCities.push_back(closestCity);
    numberOfCitiesFound++;


    QString startingCity;
    QString endingCity;
    QVector<Edge> closestEdges;


    // This while loop keeps the program running until all cities have been visited
    while(numberOfCitiesFound != tempList.size())
    {
        int smallestWeight = 9999;


        //this for-loop iterates through all visited cities gathering their
        // edges and finding the smallest one of all the possible edges
        for(int i = 0; i < visitedCities.size(); i++)
        {
            bool searching = false;
            int cityIndex= 0;


            //this while loop finds the existing city to pull its adjacency list
            while(!searching && cityIndex < tempList.size())
            {
                if(visitedCities[i] == tempList[cityIndex].label)
                {
                    searching = true;
                    closestEdges = tempList[cityIndex].incidentEdges;
                    currentCityIndex = cityIndex;
                }
                else
                {
                    cityIndex++;
                }
            }

            // this for-loop finds the smallest edge in that adjacency list and
            // compares it to the smallestWeight, and stores the appropriate data
            for(int k = 0; k < closestEdges.size(); k++)
            {
                if(closestEdges[k].weight < smallestWeight)
                {
                    bool exists = false;
                    for(int j = 0; j < visitedCities.size(); j++)
                    {

                        if(closestEdges.size() > 0 &&
                                closestEdges[0].connectedVertex->label == visitedCities[j])
                        {
                            closestEdges.erase(closestEdges.begin());
                            exists = true;
                        }
                    }
                    if(!exists)
                    {
                        startingCity = tempList[cityIndex].label;
                        endingCity = closestEdges[0].connectedVertex->label;
                        smallestWeight = closestEdges[0].weight;
                    }

                }
            }
        }
        if(smallestWeight != 9999)
        {
            visitedEdges.push_back(std::pair<QString,QString>(startingCity, endingCity));
            visitedCities.push_back(endingCity);
            visitedCitiesWeight.push_back(smallestWeight);
            numberOfCitiesFound++;
        }
    }

    ui->dijkstrasTableWidget->setRowCount(0);
    int rowNumber = ui->dijkstrasTableWidget->rowCount();

    for(int i = 0; i < visitedEdges.size(); i++)
    {
        ui->dijkstrasTableWidget->insertRow(rowNumber);
        ui->dijkstrasTableWidget->setItem(rowNumber, 0, new QTableWidgetItem(visitedEdges[i].first));
        ui->dijkstrasTableWidget->setItem(rowNumber, 1, new QTableWidgetItem(visitedEdges[i].second));
        ui->dijkstrasTableWidget->setItem(rowNumber, 2, new QTableWidgetItem(QString::number(visitedCitiesWeight[i])));
        rowNumber++;
        totalMileage += visitedCitiesWeight[i];
    }

    ui->dijkstrasTableWidget->insertRow(rowNumber);
    ui->dijkstrasTableWidget->setItem(rowNumber, 0, new QTableWidgetItem("TotalDistance: "));
    ui->dijkstrasTableWidget->setItem(rowNumber, 1, new QTableWidgetItem(QString::number(totalMileage)));

}


void MainWindow::on_updateButton_clicked()
{
   // populateConferenceDropDownBox("Both");
   // populateDFSandBFSdropDownBox();
    populateDijkstrasDropDownBox();
    populateTripSelectionDropDownBox();
    on_BothCheckBox_clicked();
}


void MainWindow::on_bfsClearButton_clicked()
{
    ui->BFSstartButton->setEnabled(true);
    ui->BFStable->setRowCount(0);
    ui->bfsClearButton->hide();
}

void MainWindow::on_DFSstartButton_clicked()
{
    QStringList traversalInfo;          //PROC - dfs trip
    int traversalDistance;              //PROC - dfs distance traveled
    int rowCount = 0;                   //for dfsTable inserts

    if(ui->DFScomboBox->currentIndex() > 0)
    {
        //set the current stadium choice
        QString currentStadiumChoice = ui->DFScomboBox->currentText();
        //call the dfs graph class function
        this->stadiumMap.dfsAtVertex(currentStadiumChoice);
        //set the traversal vector and distance traveled for the DFS
        traversalInfo = this->stadiumMap.getTraversalInfoTraversal();
        traversalDistance = this->stadiumMap.getTraversalInfoDistance();

        //ITER over the traveral info and set the vector into the table
        for(rowCount; rowCount < traversalInfo.size(); rowCount++)
        {
            ui->DFStable->insertRow(rowCount);
            ui->DFStable->setItem(rowCount, 0, new QTableWidgetItem(traversalInfo[rowCount]));
        }
        //now insert the total distance traveled into the table
        ui->DFStable->insertRow(rowCount);
        ui->DFStable->setItem(rowCount, 0, new QTableWidgetItem("Total Distance Traveled: " + QString::number(traversalDistance)) );

        ui->DFSstartButton->setEnabled(false);
        ui->DFSclearButton->show();

    }

}

void MainWindow::on_DFSclearButton_clicked()
{
    ui->DFSstartButton->setEnabled(true);
    ui->DFStable->setRowCount(0);
    ui->DFSclearButton->hide();
}
