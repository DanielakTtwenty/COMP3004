#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QColor>
#include <QWidget>
#include <QFrame>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    session = new QTimer(this);
    timer = new QTimer(this);

    count = 0;

    connect(session, SIGNAL(timeout()), this, SLOT(updateValues()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    power = false;
    pacer = 10; //default is set to 10
    ui->setupUi(this);
    connect(ui->power, SIGNAL(released()), this, SLOT(power_button()));
    connect(ui->navigateUp, SIGNAL(released()), this, SLOT(moveUpTheMenu()));
    connect(ui->navigateDown, SIGNAL(released()), this, SLOT(moveDownTheMenu()));
    connect(ui->menu, SIGNAL(released()), this, SLOT(mainMenu()));
    connect(ui->control, SIGNAL(released()), this, SLOT(navigateSubMenu()));
    connect(ui->back, SIGNAL(released()), this, SLOT(navigateBack()));
    connect(ui->charge, SIGNAL(released()), this, SLOT(chargeBattery()));
    connect(ui->hr, SIGNAL(released()), this, SLOT(contactHr()));

    ui->battery->setValue(100);

    masterMenu = new Menu("MAIN MENU", {"START SESSION","SETTINGS","HISTORY"}, nullptr);
    mainMenuOG = masterMenu;
    initializeMainMenu(masterMenu);

    ui->menudisplay->addItems(masterMenu->getMenuItems());
    ui->menudisplay->setCurrentRow(0);
    power_button();

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x_axis, y_axis);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Heart Rate");
    ui->customPlot->xAxis->setRange(0, 150);
    ui->customPlot->yAxis->setRange(0.5, 3);

}

void MainWindow::updateValues(){
    addPoint(currentTimerCount,coherence);
    count+=1;
    if(ui->battery->value() == 20){
        qDebug("Warning! Low battery");
    }
    readData(count);
}

void MainWindow::chargeBattery(){
    ui->battery->setValue(100);
}

MainWindow::~MainWindow()
{
    delete mainMenuOG;
    delete ui;
    delete timer;
    delete session;
}

void MainWindow::power_button(){

    ui->menudisplay->setVisible(power);
    ui->navigateUp->setEnabled(power);
    ui->navigateDown->setEnabled(power);
    ui->menu->setEnabled(power);
    ui->back->setEnabled(power);
    ui->battery->setEnabled(power);
    ui->control->setEnabled(power);
    ui->left->setEnabled(power);
    ui->right->setEnabled(power);

    ui->coherence->setEnabled(power);
    ui->achievement->setEnabled(power);
    ui->widget->setVisible(power);
    ui->hr->setEnabled(power);
    ui->customPlot->setVisible(power);

    ui->length->setEnabled(power);
    if(session_in_progress){
        endSession();
    }

    //initialize breath pacer to a default of 10
    QFrame *pacer;
    if(power == true){
        for (int i = 0; i < 10; ++i) {
           QString p_strip = "line_" + QString::number(i);
           pacer = MainWindow::findChild<QFrame *>(p_strip);
           pacer->setStyleSheet("background-color: red;");

        }
    }else{
        for (int i = 0; i < 10; ++i) {
           QString p_strip = "line_" + QString::number(i);
           pacer = MainWindow::findChild<QFrame *>(p_strip);
           pacer->setStyleSheet("");

        }
    }

    power = !power;
}
int MainWindow::readData(int l){
    QFile file("/home/student/Desktop/Team27/Cohenrence_InputData");

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for reading: " << file.errorString();
            return 1;
        }
        QTextStream in(&file);
        QString line;

        for (int var = 0; var < l; ++var) {
            if(in.atEnd()){
                qDebug() << "Patient might be critical condition..";
                return 0;
            }else{
                 line = in.readLine();
            }
        }
        QStringList li = line.split(",");
        float sum = 0;
        int count= 0;
        for (int var = 0; var < li.size(); ++var) {
            QString n = li.at(var);
            sum += n.toFloat();
            count++;
        }
        coherence = QString::number((sum/count *12.5),'f',2).toFloat();

        qDebug() << "Beep";
        achievement += coherence;
        int battery_level = ui->battery->value();
        battery_level -= 2;
        if(battery_level == 0){
            ui->battery->setValue(battery_level);
            power = false;
            power_button();
        }else{
             ui->battery->setValue(battery_level);
        }

        if(coherence <= 1.33 && coherence >= 0.5){

            ui->widget->setStyleSheet("background-color: red;");
            ui->coherence->setText(QString::number(coherence));
            ui->achievement->setText(QString::number(achievement));
        }
        else if(coherence >= 1.34 && coherence <= 2.17){

             ui->widget->setStyleSheet("background-color: blue;");
             ui->coherence->setText(QString::number(coherence));
             ui->achievement->setText(QString::number(achievement));
        }
        else if(coherence >= 2.18 && coherence <= 3){
             ui->widget->setStyleSheet("background-color: green;");
             ui->coherence->setText(QString::number(coherence));
             ui->achievement->setText(QString::number(achievement));
        }
        else{
            qDebug() << "Invalid Breath Level";
            ui->widget->setStyleSheet("background-color: white;");
        }

        return 0;

}

void MainWindow::mainMenu(){
    ui->menudisplay->clear();
    while (masterMenu->getName()!="MAIN MENU"){
         masterMenu = masterMenu->getParent();
    }
    updateMenu(masterMenu->getMenuItems());
}

void MainWindow::navigateBack(){
    ui->menudisplay->clear();
    if( masterMenu->getParent()!= nullptr){
         masterMenu = masterMenu->getParent();
    }
    updateMenu(masterMenu->getMenuItems());
}

void MainWindow::moveUpTheMenu() {

    int nextIndex = ui->menudisplay->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex =  ui->menudisplay->count() - 1;
    }

     ui->menudisplay->setCurrentRow(nextIndex);
}

void MainWindow::moveDownTheMenu() {

    int nextIndex = ui->menudisplay->currentRow() + 1;

    if (nextIndex >  ui->menudisplay->count() - 1) {
        nextIndex = 0;
    }

     ui->menudisplay->setCurrentRow(nextIndex);
}

void MainWindow::initializeMainMenu(Menu* m) {

    QFile file("/home/student/Desktop/Team27/history.txt");

// open the file in read-only mode

    // create a QTextStream object to read from the file
    QTextStream in(&file);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!in.atEnd()) {
            records.append(in.readLine());
        }
        file.close();

    }
    QStringList programsList;
    //QStringList settingsList;
    int numlevels = 30;
    QStringList arr;
    for (int var = 1; var < numlevels+1; ++var) {
        arr.append(QString::number(var));
    }

    Menu* sessions = new Menu("SESSIONS", programsList, m);
    Menu* settings = new Menu("SETTINGS", {"CHALLENGE LEVEL","BREATH PACER LEVEL"}, m);
    Menu* history = new Menu("HISTORY", {"VIEW","CLEAR"}, m);

    m->addChildMenu(sessions);
    m->addChildMenu(settings);
    m->addChildMenu(history);

    Menu* viewHistory = new Menu("VIEW",records, history);
    Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);

    Menu* challengeLevel = new Menu("CHALLENGE LEVEL",{"BEGINNER","AMATEUR","INTERMEDIATE", "ADVANCED"}, settings);
    Menu* bpLevel = new Menu("BREATH PACER LEVEL",arr, settings);

    settings->addChildMenu(challengeLevel);
    settings->addChildMenu(bpLevel);

    history->addChildMenu(viewHistory);
    history->addChildMenu(clearHistory);
}

void MainWindow::updateMenu(const QStringList menuItems) {

    ui->menudisplay->clear();
    ui->menudisplay->addItems(menuItems);
    ui->menudisplay->setCurrentRow(0);
    if(masterMenu->getName() == "MAIN MENU" && session_in_progress){
        ui->menudisplay->currentItem()->setText("END SESSION");
    }
}

void MainWindow::navigateSubMenu() {

    int index = ui->menudisplay->currentRow();
    if (index < 0) return;

    if (ui->menudisplay->currentItem()->text() == "VIEW" && records.length() == 0) {
        masterMenu = masterMenu->get(index);

         ui->menudisplay->clear();

        return;
    }

    if (masterMenu->getName() == "VIEW") {
        return;
    }

    if(ui->menudisplay->currentItem()->text() == "YES"){
        clearHistory();
        return;
    }
    if(ui->menudisplay->currentItem()->text() == "NO"){
        navigateBack();
        return;
    }
    if(masterMenu->getName() == "BREATH PACER LEVEL"){
        int index = ui->menudisplay->currentRow()+1;

        QFrame *pacer;
        for (int i = 0; i < 13; ++i) {
           QString p_strip = "line_" + QString::number(i);
           pacer = MainWindow::findChild<QFrame *>(p_strip);
           pacer->setStyleSheet("");

        }
        if (index>=13){
            for (int i = 0; i < 13; ++i) {
               QString p_strip = "line_" + QString::number(i);
               pacer = MainWindow::findChild<QFrame *>(p_strip);
               pacer->setStyleSheet("background-color: red;");
             }

        }else{
            for (int i = 0; i < index; ++i) {
               QString p_strip = "line_" + QString::number(i);
               pacer = MainWindow::findChild<QFrame *>(p_strip);
               pacer->setStyleSheet("background-color: red;");

            }
        }
        return;

    }
    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu(masterMenu->getMenuItems());

    }


    else if(masterMenu->getName() == "MAIN MENU" && index == 0 && session_in_progress == false){
        startSession();

    }
    else if(masterMenu->getName() == "MAIN MENU" && index == 0 && session_in_progress == true){
       endSession();

    }
    else{
        return;
    }
}

void MainWindow:: startSession(){
    ui->menudisplay->currentItem()->setText("END SESSION");
    session_in_progress = true;
    session->start(5000);
    timer->start(1000);
}

void MainWindow::updateTimer() {
    currentTimerCount += 1;
    timeString = QString::number(currentTimerCount/60) + ((currentTimerCount%60 < 10) ? + ":0" + QString::number(currentTimerCount%60) : + ":" + QString::number(currentTimerCount%60));

    ui->length->clear();
    ui->length->setText(timeString);

}

void MainWindow::addPoint(double x, double y){
    x_axis.append(x);
    y_axis.append(y);
    ui->customPlot->graph(0)->setData(x_axis, y_axis);
    //give the axes some labels:
    ui->customPlot->replot();
    ui->customPlot->update();
}

void MainWindow::endSession(){
    ui->menudisplay->currentItem()->setText("START SESSION");
    timer->stop();
    session->stop();
    ui->achievement->setText("0.0");
    ui->coherence->setText("0.0");
    ui->length->setText("0.0");
    session_in_progress = false;
    count = 0;
    ui->widget->setStyleSheet("");
    x_axis.clear();
    y_axis.clear();
    ui->customPlot->graph(0)->setData(x_axis, y_axis);
    ui->customPlot->replot();
    ui->customPlot->update();
    updateHistory();

}

void MainWindow::updateHistory(){
    QFile file("/home/student/Desktop/Team27/history.txt");

// open the file in read-only mode
    if(!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading: " << file.errorString();

   }
    QString rec;
    QDate cdate = QDate::currentDate();
    QTime ctime =  QTime::currentTime();
    ctime.toString();

    QString t = ctime.toString();
    t.chop(3);
    QString d = cdate.toString();
    if(ctime.hour()>=12){
       rec = "Date: "+ d+", "+t + " pm, ";

    }else{
        rec = "Date: "+ d+", "+t + " am, ";
    }

    // create a QTextStream object to read from the file
    QTextStream in(&file);
    QString time ="Duration : "+ QString::number(currentTimerCount) + ", ";
    QString chrence =" Coherence: "+ QString::number(coherence) + ", ";
    QString ach =" Achievement: "+ QString::number(achievement);
    rec += time + chrence + ach;
    records.append(rec);
    rec = rec+"\n";

    currentTimerCount = 0;
    coherence = 0;
    achievement = 0;
    ui->menudisplay->clear();
    masterMenu = new Menu("MAIN MENU", {"START SESSION","SETTINGS","HISTORY"}, nullptr);
    QStringList programsList;
    //QStringList settingsList;
    int numlevels = 30;
    QStringList arr;
    for (int var = 1; var < numlevels+1; ++var) {
        arr.append(QString::number(var));
    }

    Menu* sessions = new Menu("SESSIONS", programsList, masterMenu);
    Menu* settings = new Menu("SETTINGS", {"CHALLENGE LEVEL","BREATH PACER LEVEL"}, masterMenu);
    Menu* history = new Menu("HISTORY", {"VIEW","CLEAR"}, masterMenu);

    masterMenu->addChildMenu(sessions);
    masterMenu->addChildMenu(settings);
    masterMenu->addChildMenu(history);

    Menu* viewHistory = new Menu("VIEW",records, history);
    Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);

    Menu* challengeLevel = new Menu("CHALLENGE LEVEL",{"BEGINNER","AMATEUR","INTERMEDIATE", "ADVANCED"}, settings);
    Menu* bpLevel = new Menu("BREATH PACER LEVEL",arr, settings);

    settings->addChildMenu(challengeLevel);
    settings->addChildMenu(bpLevel);

    history->addChildMenu(viewHistory);
    history->addChildMenu(clearHistory);

    ui->menudisplay->addItems(masterMenu->getMenuItems());
    ui->menudisplay->setCurrentRow(0);
    file.close();
}

void MainWindow::clearHistory(){
    QFile file("/home/student/Desktop/Team27/history.txt");
    QTextStream in(&file);

    if(records.length()>0){
        records.clear();
    }

// open the file in read-only mode
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
       in << "";
       file.close();

   }
    ui->menudisplay->clear();
    if (session_in_progress == true){
         masterMenu = new Menu("MAIN MENU", {"END SESSION","SETTINGS","HISTORY"}, nullptr);
    }
    else{
        masterMenu = new Menu("MAIN MENU", {"START SESSION","SETTINGS","HISTORY"}, nullptr);
    }


    QStringList programsList;
    //QStringList settingsList;
    int numlevels = 30;
    QStringList arr;
    for (int var = 1; var < numlevels+1; ++var) {
        arr.append(QString::number(var));
    }

    Menu* sessions = new Menu("SESSIONS", programsList, masterMenu);
    Menu* settings = new Menu("SETTINGS", {"CHALLENGE LEVEL","BREATH PACER LEVEL"}, masterMenu);
    Menu* history = new Menu("HISTORY", {"VIEW","CLEAR"}, masterMenu);

    masterMenu->addChildMenu(sessions);
    masterMenu->addChildMenu(settings);
    masterMenu->addChildMenu(history);

    Menu* viewHistory = new Menu("VIEW",records, history);
    Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);

    Menu* challengeLevel = new Menu("CHALLENGE LEVEL",{"BEGINNER","AMATEUR","INTERMEDIATE", "ADVANCED"}, settings);
    Menu* bpLevel = new Menu("BREATH PACER LEVEL",arr, settings);

    settings->addChildMenu(challengeLevel);
    settings->addChildMenu(bpLevel);

    history->addChildMenu(viewHistory);
    history->addChildMenu(clearHistory);

    ui->menudisplay->addItems(masterMenu->getMenuItems());
    ui->menudisplay->setCurrentRow(0);

}

void MainWindow::contactHr()
{
    qDebug()<< "Calling HR...";

}



