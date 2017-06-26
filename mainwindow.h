#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_clicked();

    void on_newinput_clicked();

    void on_bgreedy_clicked();

    void on_bbandb_clicked();

    void on_bbrute_clicked();

    void on_bdp_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
