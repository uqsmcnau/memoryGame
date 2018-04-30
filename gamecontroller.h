#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

class GameController : public QObject {
    Q_OBJECT

public:
    GameController();
    ~GameController();
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
    void initialize_notes();
};
#endif // GAMECONTROLLER_H
