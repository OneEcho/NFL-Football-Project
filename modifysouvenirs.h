#ifndef MODIFYSOUVENIRS_H
#define MODIFYSOUVENIRS_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDebug>
#include "graph.h"

class adminWindow;
namespace Ui {
class modifysouvenirs;
}

class modifysouvenirs : public QDialog
{
    Q_OBJECT
private:
    Ui::modifysouvenirs *ui;
    Graph *graph;
    typedef void (adminWindow::*function)();
    function updateStadiumTableFunc;
    adminWindow &parentWindow;
public:
    explicit modifysouvenirs(adminWindow& parent, function func);
    ~modifysouvenirs();
    void setGraphPointer(Graph *p);

private slots:

    void on_addSouvenir_clicked();

    void on_deleteSouvenir_clicked();

    void on_modifySouvenir_clicked();

    void on_doneButton_clicked();

    void on_addSandiegoSailorsButton_clicked();

    void on_teamComboBox_currentIndexChanged(const QString &arg1);

    void updateSouvenirView();

};

#endif // MODIFYSOUVENIRS_H
