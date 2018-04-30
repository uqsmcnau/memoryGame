#include "timer.h"
#include <thread>
#include <QTime>
#include <QCoreApplication>

Timer::Timer() {
    // Constructor
}

Timer::~Timer() {
    // free resources
}

// Recieve a signal that a button has been pressed and a timer is to be
// started until it can be return to default background color
// This process can be interrupted if that timer needs to be extended
void Timer::process() {
    using namespace std::this_thread;
    using namespace std::chrono;

    // Wait half a second and return signal to return
    // background color of button to default
    QTime dieTime= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    emit done();
}
