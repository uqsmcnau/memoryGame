#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamecontroller.h"
#include "timer.h"
#include <QThread>
#include <QFile>
#include <QTime>
#include <QtMultimedia/QMediaPlayer>
#include <thread>
#include <chrono>

QThread* workerThread;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup Game Controller Object and start
    thread = QSharedPointer<QThread>(new QThread);
    worker = QSharedPointer<GameController>(new GameController());
    worker.data()->moveToThread(thread.data());

    connect(thread.data(), SIGNAL(started()), worker.data(), SLOT(start()));
    connect(worker.data(), SIGNAL(finished()), thread.data(), SLOT(quit()));
    connect(worker.data(), SIGNAL(finished()), worker.data(), SLOT(deleteLater()));
    connect(thread.data(), SIGNAL(finished()), thread.data(), SLOT(deleteLater()));

    connect(worker.data(), SIGNAL(signal_red()), this, SLOT(recieve_red()));
    connect(worker.data(), SIGNAL(signal_blue()), this, SLOT(recieve_blue()));
    connect(worker.data(), SIGNAL(signal_green()), this, SLOT(recieve_green()));
    connect(worker.data(), SIGNAL(signal_yellow()), this, SLOT(recieve_yellow()));

    connect(this, SIGNAL(signal_red()), worker.data(), SLOT(hear_red()));
    connect(this, SIGNAL(signal_blue()), worker.data(), SLOT(hear_blue()));
    connect(this, SIGNAL(signal_green()), worker.data(), SLOT(hear_green()));
    connect(this, SIGNAL(signal_yellow()), worker.data(), SLOT(hear_yellow()));

    connect(worker.data(), SIGNAL(playSuccess()), this, SLOT(playSuccess()));
    connect(worker.data(), SIGNAL(playFailure()), this, SLOT(playFailure()));

    thread.data()->start();

    // Setup Red Button
    redThread = QSharedPointer<QThread>(new QThread);
    redTimer = QSharedPointer<Timer>(new Timer());
    redTimer.data()->moveToThread(redThread.data());
    connect(redThread.data(), SIGNAL(started()), redTimer.data(), SLOT(process()));
    connect(redTimer.data(), SIGNAL(finished()), redThread.data(), SLOT(quit()));
    connect(redTimer.data(), SIGNAL(finished()), redTimer.data(), SLOT(deleteLater()));
    connect(redThread.data(), SIGNAL(finished()), redThread.data(), SLOT(deleteLater()));
    connect(this, SIGNAL(start_red_timer()), redTimer.data(), SLOT(process()));
    connect(redTimer.data(), SIGNAL(done()), this, SLOT(stop_red()));
    redThread.data()->start();

    // Setup Blue Button
    blueThread = QSharedPointer<QThread>(new QThread);
    blueTimer = QSharedPointer<Timer>(new Timer());
    blueTimer.data()->moveToThread(blueThread.data());
    connect(blueThread.data(), SIGNAL(started()), blueTimer.data(), SLOT(process()));
    connect(blueTimer.data(), SIGNAL(finished()), blueThread.data(), SLOT(quit()));
    connect(blueTimer.data(), SIGNAL(finished()), blueTimer.data(), SLOT(deleteLater()));
    connect(blueThread.data(), SIGNAL(finished()), blueThread.data(), SLOT(deleteLater()));
    connect(this, SIGNAL(start_blue_timer()), blueTimer.data(), SLOT(process()));
    connect(blueTimer.data(), SIGNAL(done()), this, SLOT(stop_blue()));
    blueThread.data()->start();

    // Setup Green Button
    greenThread = QSharedPointer<QThread>(new QThread);
    greenTimer = QSharedPointer<Timer>(new Timer());
    greenTimer.data()->moveToThread(greenThread.data());
    connect(greenThread.data(), SIGNAL(started()), greenTimer.data(), SLOT(process()));
    connect(greenTimer.data(), SIGNAL(finished()), greenThread.data(), SLOT(quit()));
    connect(greenTimer.data(), SIGNAL(finished()), greenTimer.data(), SLOT(deleteLater()));
    connect(greenThread.data(), SIGNAL(finished()), greenThread.data(), SLOT(deleteLater()));
    connect(this, SIGNAL(start_green_timer()), greenTimer.data(), SLOT(process()));
    connect(greenTimer.data(), SIGNAL(done()), this, SLOT(stop_green()));
    greenThread.data()->start();

    // Setup Yellow Button
    yellowThread = QSharedPointer<QThread>(new QThread);
    yellowTimer = QSharedPointer<Timer>(new Timer());
    yellowTimer.data()->moveToThread(yellowThread.data());
    connect(yellowThread.data(), SIGNAL(started()), yellowTimer.data(), SLOT(process()));
    connect(yellowTimer.data(), SIGNAL(finished()), yellowThread.data(), SLOT(quit()));
    connect(yellowTimer.data(), SIGNAL(finished()), yellowTimer.data(), SLOT(deleteLater()));
    connect(yellowThread.data(), SIGNAL(finished()), yellowThread.data(), SLOT(deleteLater()));
    connect(this, SIGNAL(start_yellow_timer()), yellowTimer.data(), SLOT(process()));
    connect(yellowTimer.data(), SIGNAL(done()), this, SLOT(stop_yellow()));
    yellowThread.data()->start();
}

MainWindow::~MainWindow()
{
    delete ui;

    workerThread->quit();
    workerThread->wait();
}

// Change background of red button to non-pressed default
void MainWindow::stop_red()
{
    ui->redButton->setStyleSheet("background-color: rgb(192, 0, 0);");
}

// Change background of blue button to non-pressed default
void MainWindow::stop_blue()
{
    ui->blueButton->setStyleSheet("background-color: rgb(0, 0, 192);");
}

// Change background of green button to non-pressed default
void MainWindow::stop_green()
{
    ui->greenButton->setStyleSheet("background-color: rgb(0, 192, 0);");
}

// Change background of yellow button to non-pressed default
void MainWindow::stop_yellow()
{
    ui->yellowButton->setStyleSheet("background-color: rgb(192, 192, 0);");
}

// Slot from controller to press red button
void MainWindow::recieve_red()
{
    red();
}

// Slot from controller to press blue button
void MainWindow::recieve_blue()
{
    blue();
}

// Slot from controller to press green button
void MainWindow::recieve_green()
{
    green();
}

// Slot from controller to press yellow button
void MainWindow::recieve_yellow()
{
    yellow();
}

// Signal to controller for red button pressed
void MainWindow::on_redButton_pressed()
{
    red();
    emit signal_red();
}

// Signal to controller for blue button pressed
void MainWindow::on_blueButton_pressed()
{
    blue();
    emit signal_blue();
}

// Signal to controller for green button pressed
void MainWindow::on_greenButton_pressed()
{
    green();
    emit signal_green();
}

// Signal to controller for yellow button pressed
void MainWindow::on_yellowButton_pressed()
{
    yellow();
    emit signal_yellow();
}

// Play a note, change the color of the button to indicate it has been pressed
// and start a timer to change it back
void MainWindow::red(){
    // Change color of button
    ui->redButton->setStyleSheet("background-color: rgb(255, 63, 63);");

    // Play Note
    redPlayer = QSharedPointer<QMediaPlayer>(new QMediaPlayer);
    redPlayer.data()->setMedia(QUrl("qrc:/audio/45.mp3"));
    redPlayer.data()->setVolume(50);
    redPlayer.data()->play();

    // Start timer
    emit start_red_timer();
}

// Play a note, change the color of the button to indicate it has been pressed
// and start a timer to change it back
void MainWindow::blue(){
    // Change color of button
    ui->blueButton->setStyleSheet("background-color: rgb(63, 63, 255);");

    // Play Note
    bluePlayer = QSharedPointer<QMediaPlayer>(new QMediaPlayer);
    bluePlayer.data()->setMedia(QUrl("qrc:/audio/46.mp3"));
    bluePlayer.data()->setVolume(50);
    bluePlayer.data()->play();

    // Start timer
    emit start_blue_timer();
}

// Play a note, change the color of the button to indicate it has been pressed
// and start a timer to change it back
void MainWindow::green(){
    // Change color of button
    ui->greenButton->setStyleSheet("background-color: rgb(63, 255, 63);");

    // Play Note
    greenPlayer = QSharedPointer<QMediaPlayer>(new QMediaPlayer);
    greenPlayer.data()->setMedia(QUrl("qrc:/audio/47.mp3"));
    greenPlayer.data()->setVolume(50);
    greenPlayer.data()->play();

    // Start timer
    emit start_green_timer();
}


// Play a note, change the color of the button to indicate it has been pressed
// and start a timer to change it back
void MainWindow::yellow(){
    // Change color of button
    ui->yellowButton->setStyleSheet("background-color: rgb(255, 255, 63);");

    // Play Note
    yellowPlayer = QSharedPointer<QMediaPlayer>(new QMediaPlayer);
    yellowPlayer.data()->setMedia(QUrl("qrc:/audio/48.mp3"));
    yellowPlayer.data()->setVolume(50);
    yellowPlayer.data()->play();

    // Start timer
    emit start_yellow_timer();
}

// Play a high note to indicate a correct sequence has been pressed
void MainWindow::playSuccess() {
    successPlayer = QSharedPointer<QMediaPlayer>(new QMediaPlayer);
    successPlayer.data()->setMedia(QUrl("qrc:/audio/64.mp3"));
    successPlayer.data()->setVolume(50);
    successPlayer.data()->play();
}

// Play a deep note to indicate a wrong key has been pressed
void MainWindow::playFailure() {
    failurePlayer = QSharedPointer<QMediaPlayer>(new QMediaPlayer);
    failurePlayer.data()->setMedia(QUrl("qrc:/audio/08.mp3"));
    failurePlayer.data()->setVolume(50);
    failurePlayer.data()->play();
}
