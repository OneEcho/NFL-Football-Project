#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

/*!
 * \fn MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Database* DB = Database::getInstance();

    this->populateConferenceDropDownBox("Both");
    ui->tabWidget->setCurrentIndex(0);
//    ui->ConferenceTableView->setSortingEnabled(false);
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
    ui->NFLCheckBox->setChecked(false);
    ui->BothCheckBox->setChecked(false);

    this->populateConferenceDropDownBox("AFL");

    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query;

    query.prepare("SELECT * FROM Teams WHERE Conference = ? ORDER BY TeamName ASC");
    query.addBindValue("American Football Conference");

    query.exec();

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

/*!
 * \fn MainWindow::on_NFLCheckBox_clicked
 */
void MainWindow::on_NFLCheckBox_clicked()
{
    ui->AFLCheckBox->setChecked(false);
    ui->BothCheckBox->setChecked(false);

    this->populateConferenceDropDownBox("NFL");

    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query;

    query.prepare("SELECT * FROM Teams WHERE Conference = ? ORDER BY TeamName ASC");
    query.addBindValue("National Football Conference");

    query.exec();

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

    this->populateConferenceDropDownBox("Both");

    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query;

    query.prepare("SELECT * FROM Teams ORDER BY TeamName ASC");

    query.exec();

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

void MainWindow::on_ConferenceTableView_clicked(const QModelIndex &index)
{

    qDebug() << index.row();
}
