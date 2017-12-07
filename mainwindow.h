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
    void visitAllStadiumsEfficiently(QString startingCity);
    void hideSecondaryTripInputs();

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

    void on_spinBox_valueChanged(int arg1);

    void on_souvenirTable_clicked(const QModelIndex &index);

    void on_purchaseButton_clicked();

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
    Cart purchases;
    Map Souvenirs;
    Graph stadiumMap;
    QModelIndex souvenirIndex;
    int souvenirQuantity;
};

#endif // MAINWINDOW_H
