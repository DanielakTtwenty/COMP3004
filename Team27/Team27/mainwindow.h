#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <iostream>
#include "menu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool power;
    Menu* masterMenu;
    Menu* mainMenuOG;
    int pacer;
    QTimer* session;
    float coherence = 0;
    int count;
    float achievement = 0;
    QTimer *timer;
    int currentTimerCount = 0;
    QString timeString;
    QVector<double> x_axis,y_axis;
    bool session_in_progress = false;
    QStringList records;
    int filelength;


private slots:
    void updateMenu( const QStringList); //  populates the menu screen with the given list
    void power_button(); //  handles the power state of the device, connected to power button
    void moveUpTheMenu(); // moves up the menu
    void moveDownTheMenu(); // moves down  the menu
    void mainMenu(); // goes back to the main menu
    void initializeMainMenu(Menu*); // populates the menu at the beggining of program
    void navigateSubMenu(); //connected to the ok button, handles selection of a menu option
    void navigateBack(); // navigates back to the parent menu
    int readData(int l);  //  reads file up to line l
    void updateTimer(); //
    void startSession(); // starts a session on tthe device
    void addPoint(double x, double y); // plots for x and y on the graph
    void updateValues();  // updates the coherence and achievement values during an ongoing session
    void chargeBattery();  //  charges the device battery to 100%
    void endSession(); //  ends an ongoing session
    void updateHistory();  // updates the device's sesssion history
    void clearHistory(); //clears the device session history
    void contactHr(); // contacts the HR dept.

};
#endif // MAINWINDOW_H
