#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "modifysouvenirs.h"
#include "graph.h"
class MainWindow;
namespace Ui {
class adminWindow;
}

class adminWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::adminWindow *ui;
    modifysouvenirs *modifySouvenirsWindow;
    Graph *graphPointer;
    typedef void (MainWindow::*functionName)();
    functionName func;
    MainWindow &parentWindow;

public:
    explicit adminWindow(MainWindow& parent, functionName funcT);
    ~adminWindow();
    void newShow();
    void setPointerToGraph(Graph *p);


private slots:
    void on_passwordLineEdit_returnPressed();

    void on_cancelLoginButton_clicked();

    void on_loginButton_clicked();

    void on_usernameLineEdit_returnPressed();

    void on_pushButtonAddStadium_clicked();


    void on_stadiumTableView_doubleClicked(const QModelIndex &index);
    void openStadiumModifyPage();

    void on_modifySouvenirsButton_clicked();
    void updateTable();
    void updateModifyTable();
};


#endif // ADMINWINDOW_H
