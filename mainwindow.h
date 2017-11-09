#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adminwindow.h"
#include <QString>

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

private:
    Ui::MainWindow *ui;
    adminWindow *admin;
};

#endif // MAINWINDOW_H