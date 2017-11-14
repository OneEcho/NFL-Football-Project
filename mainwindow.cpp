#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QPalette>
#include <QMessageBox>

/*!
 * \fn MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    this->admin = new adminWindow;

    ui->setupUi(this);

    Database* DB = Database::getInstance();

    this->populateConferenceDropDownBox("Both");
    ui->tabWidget->setCurrentIndex(0);


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
        ui->roofNumberLabel->setText(roofLabelMessage + QString::number(numberOfRoofType));
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
    this->admin->newShow();
}

