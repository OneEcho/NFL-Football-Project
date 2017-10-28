#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_AFLCheckBox_clicked();

    void on_NFLCheckBox_clicked();

    void on_BothCheckBox_clicked();

    void on_TeamsComboBox_currentIndexChanged(const QString &arg1);

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
