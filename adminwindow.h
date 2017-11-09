#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::adminWindow *ui;
};

#endif // ADMINWINDOW_H
