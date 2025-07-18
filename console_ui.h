#pragma once
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <cctype>
#include <algorithm>
#include <variant>
#include <iomanip>
#include <fcntl.h>

#include "sound.h"
#include "users.h"
#include "clock_entry_registry.h"

class ConsoleUI {
private:
 
    int cursorPosition = 0; 

public:
    Sound sfx;
    static Users idData;
    static ClockEntryRegistry clockData;

    std::vector<std::string> messages;
    std::ostringstream buffer;   

    std::string errorMessege = "";
    std::string toolTip = "";

    void printMessages();
    int getCursorPosition();
    void moveCursorUp();
    void moveCursorDown();
    void addMessageFromInput();
    void removeMessageAtCursor();

    std::string getButton();
    void clearScreen();

    void printToolTips();
    void printTextBox(const std::string& content, int width);


    int clockInPrompt();

    void drawUI(const std::string& lastInput);

    void populateMessagesFromUsers();
    void populateMessagesFromClockRegistry();

    int addUserPrompt();
};



