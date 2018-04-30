#include "gamecontroller.h"

#include <QTime>
#include <QCoreApplication>

int INITAL_SEQUENCE_LENGTH = 3;
int NUMBER_OF_BUTTONS = 4;

GameController::GameController() {
    // Constructor
}

GameController::~GameController() {
    // free resources
}


// Slot for when the player has pressed the red button
void GameController::hear_red() {
    hear('0',"0");
}

// Slot for when the player has pressed the blue button
void GameController::hear_blue() {
    hear('1',"1");
}

// Slot for when the player has pressed the green button
void GameController::hear_green() {
    hear('2',"2");
}

// Slot for when the player has pressed the yellow button
void GameController::hear_yellow() {
    hear('3',"3");
}

// The player has pressed a button and the game responses based
// on what was pressed
void GameController::hear(char c, std::string s) {
    // Set the playing flag to false to interrupt if
    // the controller is playing a sequence
    playing = false;

    // If the button pressed was the correct one in the sequence
    if (targetset[inputset.length()] == c) {
        // Update the target
        inputset.append(s);
        // If the player has completed the sequence
        if (targetset.length() == inputset.length()) {
            // Reset their input
            inputset = "";

            // Play the success note to alert the player
            emit playSuccess();

            // Add a random button onto the end of the sequence
            targetset.append(std::to_string(rand() % 4));

            // Wait 1 second
            QTime dieTime= QTime::currentTime().addMSecs(1000);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

            // Play the sequence for the player to have a chance
            // to learn the new note
            play();
        }
    // If the button pressed was incorrect
    } else {
        // Play the failure note to aleart the player
        emit playFailure();

        // Reset the target sequence
        initialize_notes();

        // Wait 1 second
        QTime dieTime= QTime::currentTime().addMSecs(1000);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        // Play the sequence for the player to have a chance
        // to learn the sequence
        play();
    }
}

// Start the game
void GameController::start(){
    // Set initial sequence
    initialize_notes();

    // Wait 1 second
    QTime dieTime= QTime::currentTime().addMSecs(1000);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    // Play the first sequence for the player
    play();
}

// Light up the buttons in the sequence for the player to remember
void GameController::play() {
    playing = true;
    // For each button in the current sequence
    for( unsigned int i = 0; i < targetset.length(); i++ )
    {
        // Wait 1 second between buttons
        QTime dieTime= QTime::currentTime().addMSecs(1000);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        // If the player has interrupted stop
        if (playing == false) {
            break;
        }

        // Emit a signal to the main class to light up the button
        // for the current item in the sequence
        switch(targetset[i])
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

// Create a new sequence of buttons of minimium length
void GameController::initialize_notes(){
    targetset = "";
    srand ( time(0));
    for( int i = 0; i < INITAL_SEQUENCE_LENGTH; i++ )
    {
        targetset.append(std::to_string(rand() % NUMBER_OF_BUTTONS));
    }
    inputset = "";
}
