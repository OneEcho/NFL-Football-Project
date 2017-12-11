#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adminwindow.h"
#include <QString>
#include <QStandardItemModel>
#include <graph.h>
#include "cart.h"
#include "map.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QModelIndex>

/**************************************************************************************************************************************************************
 * PROGRAM DESCRIPTION
 * --------------------------------------------------------------
 * Let's Play NFL Football Project 2
 * --------------------------------------------------------------
 * This Program is set for the Football Fan actor, who’s main goal is planning a vacation by visiting a starting
 * stadium and ending stadium, along with the option to visit stadiums in between. The fan also gets options to
 * view all team data and information including but not limited to: stadium name, stadium roof type, and stadium turf type.
 * The football fan is also able to determine the BFS and DFS along with the MST of the graph of all the football stadiums in the US.
 * Lastly the football fan is able to buy souvenirs at each stadium they visit and add to their football vacation.
 * The program will give them a final total after visiting all the stadiums so the football fan can see exactly how much they spent while on their vacation.
 * The Admin actor is allowed to access the admin window via a login screen. Upon entering the correct credentials, the admin is faced
 * with a window and given the credentials to change team names, stadium names, souvenirs, souvenirs prices, and all data associated with each team.
 ***************************************************************************************************************************************************************/

struct collegeStadiumPair
{
    QString college;
    QString stadium;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void populateConferenceDropDownBox(QString box);
    void populateTripSelectionDropDownBox();
    void populateDijkstrasDropDownBox();
    void populateDFSandBFSdropDownBox();
    void showStartingTripInputs();
    void visitAllStadiumsEfficiently(QString startingStadium, QStringList stadiumsToVisit, QVector<QString> &visitedStadiums);
    void hideSecondaryTripInputs();

    void dreamVacation(QString startStadium, QStringList tripList, QStringList &visitedStadiums);

private slots:
    /*! \fn AFLCheckBox_clicked */
    void on_AFLCheckBox_clicked();

    /*! \fn NFLCheckBox_clicked */
    void on_NFLCheckBox_clicked();

    /*! \fn BothCheckBox_clicked */
    void on_BothCheckBox_clicked();

    /*! \fn TeamsComboBox_currentIndexChanged */
    void on_TeamsComboBox_currentIndexChanged(const QString &arg1);

    /*! \fn adminButton_clicked */
    void on_adminButton_clicked();
    
    /*!
    * \brief on_comboBox_currentIndexChanged*/
    void on_roofComboBox_currentIndexChanged(const QString &arg1);

    void on_tabWidget_tabBarClicked(int index);

    void on_addToTripButton_clicked();

    void on_tripCreationComboBox_currentIndexChanged(int index);

    void on_finishAddingButton_clicked();

    void on_startTripButton_clicked();

    void on_nextCollegeButton_clicked();

    void on_startingStadiumComboBoxDijkstras_currentIndexChanged(const QString &arg1);

    void on_endingStadiumComboBoxDijkstras_currentIndexChanged(const QString &arg1);

    void on_visitAllStadiumsButton_clicked();
    void on_resetTripButton_clicked();

    void on_BFSstartButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_bfsClearButton_clicked();

    void on_DFSstartButton_clicked();

    void on_DFSclearButton_clicked();
    void on_spinBox_valueChanged(int arg1);

    void on_souvenirTable_clicked(const QModelIndex &index);

    void on_purchaseButton_clicked();
    void on_dreamVacationButton_clicked();

    void on_MSTButton_clicked();

    void on_updateButton_clicked();

private:
    Ui::MainWindow *ui;
    adminWindow *admin;

    //index to track row additions
    int tripTableViewRowNumber;
    int BFStableWidgitRowNumber;
    QStandardItemModel *table;
    QStandardItemModel *BFSwidgit;
    //vector full of the pushed stadiums
    QVector<collegeStadiumPair> stadiumTrip;
    //integer of current stadium
    int currentStadiumIndex;
    Cart *purchases;
    Map Souvenirs;
    int totalDistance;
    QStringList listTrip;
    Graph stadiumMap;
    QModelIndex souvenirIndex;
    int souvenirQuantity;
    int totalAmountRowIndex;
    bool souvenirSelected;
};

#endif // MAINWINDOW_H
