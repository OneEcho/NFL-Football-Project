#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adminwindow.h"
#include <QString>
#include <QStandardItemModel>

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

private:
    Ui::MainWindow *ui;
    adminWindow *admin;

    //index to track row additions
    int tripTableViewRowNumber;
    QStandardItemModel *table;
};

#endif // MAINWINDOW_H
