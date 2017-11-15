#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "modifysouvenirs.h"

namespace Ui {
class adminWindow;
}

class adminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminWindow(QWidget *parent = 0);
    ~adminWindow();
    void newShow();

private slots:
    void on_passwordLineEdit_returnPressed();

    void on_cancelLoginButton_clicked();

    void on_loginButton_clicked();

    void on_usernameLineEdit_returnPressed();

    void on_pushButtonAddStadium_clicked();


    void on_stadiumTableView_doubleClicked(const QModelIndex &index);
    void openStadiumModifyPage();

    void on_modifySouvenirsButton_clicked();

private:
    Ui::adminWindow *ui;
    modifysouvenirs *modifySouvenirsWindow;
};

#endif // ADMINWINDOW_H
