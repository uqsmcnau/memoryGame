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

class Worker : public QObject {
    Q_OBJECT

public:
    Worker();
    ~Worker();
    void hear (char c, std::string s);
    void play();

public slots:
    void start();

    void hear_red();
    void hear_blue();
    void hear_green();
    void hear_yellow();

signals:
    void signal_red();
    void signal_blue();
    void signal_green();
    void signal_yellow();

    void playSuccess();
    void playFailure();

    void finished();
    void error(QString err);

private:
    std::string targetset;
    std::string inputset;
    bool playing;
};

class Timer : public QObject {
    Q_OBJECT

public:
    Timer();
    ~Timer();

public slots:
    void process();

signals:
    void done();
    void finished();
    void error(QString err);
};

#endif // MAINWINDOW_H
