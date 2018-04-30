#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamecontroller.h"
#include "timer.h"
#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void red();
    void blue();
    void green();
    void yellow();

    ~MainWindow();

public slots:
    void recieve_red();
    void recieve_blue();
    void recieve_green();
    void recieve_yellow();

    void stop_red();
    void stop_blue();
    void stop_green();
    void stop_yellow();

    void playSuccess();
    void playFailure();

private:
    Ui::MainWindow *ui;
    QSharedPointer<QThread> thread;
    QSharedPointer<GameController> worker;

    QSharedPointer<QThread> redThread;
    QSharedPointer<QThread> blueThread;
    QSharedPointer<QThread> greenThread;
    QSharedPointer<QThread> yellowThread;

    QSharedPointer<Timer> redTimer;
    QSharedPointer<Timer> blueTimer;
    QSharedPointer<Timer> greenTimer;
    QSharedPointer<Timer> yellowTimer;

    QSharedPointer<QMediaPlayer> redPlayer;
    QSharedPointer<QMediaPlayer> bluePlayer;
    QSharedPointer<QMediaPlayer> greenPlayer;
    QSharedPointer<QMediaPlayer> yellowPlayer;
    QSharedPointer<QMediaPlayer> successPlayer;
    QSharedPointer<QMediaPlayer> failurePlayer;

private slots:
    void on_redButton_pressed();
    void on_blueButton_pressed();
    void on_greenButton_pressed();
    void on_yellowButton_pressed();

signals:
    void signal_red();
    void signal_blue();
    void signal_green();
    void signal_yellow();

    void start_red_timer();
    void start_blue_timer();
    void start_green_timer();
    void start_yellow_timer();

};

#endif // CONTROLLER_H
