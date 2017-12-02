#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adminwindow.h"
#include <QString>
#include <QStandardItemModel>
#include <graph.h>
#include "cart.h"

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
    void showStartingTripInputs();
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

    void on_resetTripButton_clicked();

private:
    Ui::MainWindow *ui;
    adminWindow *admin;

    //index to track row additions
    int tripTableViewRowNumber;
    QStandardItemModel *table;
    //vector full of the pushed stadiums
    QVector<collegeStadiumPair> stadiumTrip;
    //integer of current stadium
    int currentStadiumIndex;
    Cart items;

    Graph stadiumMap;
};

#endif // MAINWINDOW_H
