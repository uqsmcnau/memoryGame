#include "mainwindow.h"
#include "ui_mainwindow.h"
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

    QThread* thread = new QThread;
    Worker* worker = new Worker();
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(start()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(worker, SIGNAL(signal_red()), this, SLOT(recieve_red()));
    connect(worker, SIGNAL(signal_blue()), this, SLOT(recieve_blue()));
    connect(worker, SIGNAL(signal_green()), this, SLOT(recieve_green()));
    connect(worker, SIGNAL(signal_yellow()), this, SLOT(recieve_yellow()));

    connect(this, SIGNAL(signal_red()), worker, SLOT(hear_red()));
    connect(this, SIGNAL(signal_blue()), worker, SLOT(hear_blue()));
    connect(this, SIGNAL(signal_green()), worker, SLOT(hear_green()));
    connect(this, SIGNAL(signal_yellow()), worker, SLOT(hear_yellow()));

    connect(worker, SIGNAL(playSuccess()), this, SLOT(playSuccess()));
    connect(worker, SIGNAL(playFailure()), this, SLOT(playFailure()));

    thread->start();

    QThread* redThread = new QThread;
    Timer* redTimer = new Timer();
    redTimer->moveToThread(redThread);
    connect(redThread, SIGNAL(started()), redTimer, SLOT(process()));
    connect(redTimer, SIGNAL(finished()), redThread, SLOT(quit()));
    connect(redTimer, SIGNAL(finished()), redTimer, SLOT(deleteLater()));
    connect(redThread, SIGNAL(finished()), redThread, SLOT(deleteLater()));
    connect(this, SIGNAL(start_red_timer()), redTimer, SLOT(process()));
    connect(redTimer, SIGNAL(done()), this, SLOT(stop_red()));
    redThread->start();

    QThread* blueThread = new QThread;
    Timer* blueTimer = new Timer();
    blueTimer->moveToThread(blueThread);
    connect(blueThread, SIGNAL(started()), blueTimer, SLOT(process()));
    connect(blueTimer, SIGNAL(finished()), blueThread, SLOT(quit()));
    connect(blueTimer, SIGNAL(finished()), blueTimer, SLOT(deleteLater()));
    connect(blueThread, SIGNAL(finished()), blueThread, SLOT(deleteLater()));
    connect(this, SIGNAL(start_blue_timer()), blueTimer, SLOT(process()));
    connect(blueTimer, SIGNAL(done()), this, SLOT(stop_blue()));
    blueThread->start();

    QThread* greenThread = new QThread;
    Timer* greenTimer = new Timer();
    greenTimer->moveToThread(greenThread);
    connect(greenThread, SIGNAL(started()), greenTimer, SLOT(process()));
    connect(greenTimer, SIGNAL(finished()), greenThread, SLOT(quit()));
    connect(greenTimer, SIGNAL(finished()), greenTimer, SLOT(deleteLater()));
    connect(greenThread, SIGNAL(finished()), greenThread, SLOT(deleteLater()));
    connect(this, SIGNAL(start_green_timer()), greenTimer, SLOT(process()));
    connect(greenTimer, SIGNAL(done()), this, SLOT(stop_green()));
    greenThread->start();

    QThread* yellowThread = new QThread;
    Timer* yellowTimer = new Timer();
    yellowTimer->moveToThread(yellowThread);
    connect(yellowThread, SIGNAL(started()), yellowTimer, SLOT(process()));
    connect(yellowTimer, SIGNAL(finished()), yellowThread, SLOT(quit()));
    connect(yellowTimer, SIGNAL(finished()), yellowTimer, SLOT(deleteLater()));
    connect(yellowThread, SIGNAL(finished()), yellowThread, SLOT(deleteLater()));
    connect(this, SIGNAL(start_yellow_timer()), yellowTimer, SLOT(process()));
    connect(yellowTimer, SIGNAL(done()), this, SLOT(stop_yellow()));
    yellowThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;

    workerThread->quit();
    workerThread->wait();
}

void MainWindow::stop_red()
{
    ui->redButton->setStyleSheet("background-color: rgb(192, 0, 0);");
}

void MainWindow::stop_blue()
{
    ui->blueButton->setStyleSheet("background-color: rgb(0, 0, 192);");
}

void MainWindow::stop_green()
{
    ui->greenButton->setStyleSheet("background-color: rgb(0, 192, 0);");
}

void MainWindow::stop_yellow()
{
    ui->yellowButton->setStyleSheet("background-color: rgb(192, 192, 0);");
}

void MainWindow::recieve_red()
{
    red();
}

void MainWindow::recieve_blue()
{
    blue();
}

void MainWindow::recieve_green()
{
    green();
}

void MainWindow::recieve_yellow()
{
    yellow();
}

void MainWindow::on_redButton_pressed()
{
    red();
    emit signal_red();
}

void MainWindow::on_blueButton_pressed()
{
    blue();
    emit signal_blue();
}

void MainWindow::on_greenButton_pressed()
{
    green();
    emit signal_green();
}

void MainWindow::on_yellowButton_pressed()
{
    yellow();
    emit signal_yellow();
}

void MainWindow::red(){
    ui->redButton->setStyleSheet("background-color: rgb(255, 63, 63);");
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/audio/45.mp3"));
    player->setVolume(50);
    player->play();

    emit start_red_timer();
}

void MainWindow::blue(){
    ui->blueButton->setStyleSheet("background-color: rgb(63, 63, 255);");
    QMediaPlayer *player = new QMediaPlayer;
    //player->setMedia(QUrl::fromLocalFile("D:/Workshop/workspace3/Round2/sounds/46.mp3"));
    player->setMedia(QUrl("qrc:/audio/46.mp3"));
    player->setVolume(50);
    player->play();

    emit start_blue_timer();
}

void MainWindow::green(){
    ui->greenButton->setStyleSheet("background-color: rgb(63, 255, 63);");
    QMediaPlayer *player = new QMediaPlayer;
    //player->setMedia(QUrl::fromLocalFile("D:/Workshop/workspace3/Round2/sounds/45.mp3"));
    player->setMedia(QUrl("qrc:/audio/47.mp3"));
    player->setVolume(50);
    player->play();

    emit start_green_timer();
}

void MainWindow::yellow(){
    ui->yellowButton->setStyleSheet("background-color: rgb(255, 255, 63);");
    QMediaPlayer *player = new QMediaPlayer;
    //player->setMedia(QUrl::fromLocalFile("D:/Workshop/workspace3/Round2/sounds/45.mp3"));
    player->setMedia(QUrl("qrc:/audio/48.mp3"));
    player->setVolume(50);
    player->play();

    emit start_yellow_timer();
}

void MainWindow::playSuccess() {
    QMediaPlayer *player = new QMediaPlayer;
    //player->setMedia(QUrl::fromLocalFile("D:/Workshop/workspace3/Round2/sounds/64.mp3"));
    player->setMedia(QUrl("qrc:/audio/64.mp3"));
    player->setVolume(50);
    player->play();
}

void MainWindow::playFailure() {
    QMediaPlayer *player = new QMediaPlayer;
    //player->setMedia(QUrl::fromLocalFile("D:/Workshop/workspace3/Round2/sounds/08.mp3"));
    player->setMedia(QUrl("qrc:/audio/08.mp3"));
    player->setVolume(50);
    player->play();
}

Worker::Worker() {
    targetset = "";
    srand ( time(0));
    for( int a = 0; a < 3; a = a + 1 )
    {
        targetset.append(std::to_string(rand() % 4));
    }
    inputset = "";
}

Worker::~Worker() {
    // free resources
}


void Worker::hear_red() {
    hear('0',"0");
}

void Worker::hear_blue() {
    hear('1',"1");
}

void Worker::hear_green() {
    hear('2',"2");
}

void Worker::hear_yellow() {
    hear('3',"3");
}

void Worker::hear(char c, std::string s) {
    playing = false;

    if (targetset[inputset.length()] == c) {
        inputset.append(s);
        if (targetset.length() == inputset.length()) {
            inputset = "";

            emit playSuccess();

            targetset.append(std::to_string(rand() % 4));

            QTime dieTime= QTime::currentTime().addMSecs(1000);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            play();
        }
    } else {
        inputset = "";

        emit playFailure();

        targetset = "";
        for( int a = 0; a < 3; a = a + 1 )
        {
            targetset.append(std::to_string(rand() % 4));
        }

        QTime dieTime= QTime::currentTime().addMSecs(1000);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        play();
    }
}

void Worker::start(){
    targetset = "";
    playing = false;

    for( int a = 0; a < 3; a = a + 1 )
    {
        targetset.append(std::to_string(rand() % 4));
    }

    QTime dieTime= QTime::currentTime().addMSecs(3000);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    play();
}

void Worker::play() {
    playing = true;
    for( unsigned int a = 0; a < targetset.length(); a = a + 1 )
    {
        QTime dieTime= QTime::currentTime().addMSecs(1000);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        if (playing == false) {
            break;
        }


        switch(targetset[a])
        {
            case '0':
                emit signal_red();
                break;
            case '1':
                emit signal_blue();
                break;
            case '2':
                emit signal_green();
                break;
            case '3':
                emit signal_yellow();
                break;
        }
    }
    playing = false;
}

Timer::Timer() {
    // you could copy data from constructor arguments to internal variables here.
}

Timer::~Timer() {
    // free resources
}

void Timer::process() {
    using namespace std::this_thread;
    using namespace std::chrono;

    QTime dieTime= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    emit done();
}
