#ifndef TIMER_H
#define TIMER_H

#include <QObject>

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

#endif
