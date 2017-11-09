#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <QMessageBox>

adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Admin Window");
}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::newShow()
{
    ui->Pages->setCurrentIndex(0);
    this->show();
}

void adminWindow::on_passwordLineEdit_returnPressed()
{
    if(ui->usernameLineEdit->text() == "admin" &&
       ui->passwordLineEdit->text() == "password")
    {
        ui->Pages->setCurrentIndex(1);
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
    else
    {
        QMessageBox::critical(this, "Incorrect Login", "Incorrect login information. Try again.");
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
}

void adminWindow::on_cancelLoginButton_clicked()
{
    this->hide();
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
}

void adminWindow::on_loginButton_clicked()
{
    if(ui->usernameLineEdit->text() == "admin" &&
       ui->passwordLineEdit->text() == "password")
    {
        ui->Pages->setCurrentIndex(1);
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
    else
    {
        QMessageBox::critical(this, "Incorrect Login", "Incorrect login information. Try again.");
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }

}

void adminWindow::on_usernameLineEdit_returnPressed()
{
    if(ui->usernameLineEdit->text() == "admin" &&
       ui->passwordLineEdit->text() == "password")
    {
        ui->Pages->setCurrentIndex(1);
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
    else
    {
        QMessageBox::critical(this, "Incorrect Login", "Incorrect login information. Try again.");
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }

}
