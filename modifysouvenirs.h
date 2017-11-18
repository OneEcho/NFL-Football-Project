#ifndef MODIFYSOUVENIRS_H
#define MODIFYSOUVENIRS_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class modifysouvenirs;
}

class modifysouvenirs : public QDialog
{
    Q_OBJECT

public:
    explicit modifysouvenirs(QWidget *parent = 0);
    ~modifysouvenirs();

private slots:

    void on_addSouvenir_clicked();

    void on_deleteSouvenir_clicked();

    void on_modifySouvenir_clicked();

    void on_doneButton_clicked();

    void on_addSandiegoSailorsButton_clicked();

    void on_teamComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::modifysouvenirs *ui;
};

#endif // MODIFYSOUVENIRS_H
