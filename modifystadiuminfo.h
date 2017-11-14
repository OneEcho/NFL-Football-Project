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
    explicit modifyStadiumInfo(adminWindow& pw, functionName func);
    void setDataToOverwrite(QString d);
    void setColumn(QString);
    void setTeamName(QString);
    void setStadiumName(QString);

private slots:
    void on_PB_Update_clicked();

private:
    Ui::modifyStadiumInfo *ui;
    QString columnName;
    QString oldData;
    QString teamName;
    QString stadiumName;
};

#endif // MODIFYSTADIUMINFO_H
