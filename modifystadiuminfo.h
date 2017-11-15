#ifndef MODIFYSTADIUMINFO_H
#define MODIFYSTADIUMINFO_H

#include <QDialog>
#include "adminwindow.h"

namespace Ui {
class modifyStadiumInfo;
}

class modifyStadiumInfo : public QDialog
{
    Q_OBJECT

public:
    ~modifyStadiumInfo();
    // create a more readable type name for function pointer from mainwindow
    typedef void (adminWindow::*functionName)();
    // name of function
    functionName functor;
    // reference to mainwindow
    adminWindow &parentWindow;
    explicit modifyStadiumInfo(adminWindow& pw, functionName funcF);
    void setData(QString oldData, QString team, QString stadium, QString column, QString header);
    void updateData(QString);

private slots:
    void on_PB_Update_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_PB_Conference_clicked();

    void on_cancel_conference_clicked();

    void on_PB_Conference_pressed();

private:
    Ui::modifyStadiumInfo *ui;
    QString columnName;
    QString oldData;
    QString teamName;
    QString stadiumName;
    QString headerName;
};

#endif // MODIFYSTADIUMINFO_H
