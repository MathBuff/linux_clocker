#include "console_ui.h"

Users ConsoleUI::idData;
ClockEntryRegistry ConsoleUI::clockData;
Tmt88v ConsoleUI::tmt88v;

void ConsoleUI::printMessages() {
    for (size_t i = 0; i < messages.size(); ++i) {
        // Cursor indicator
        if (static_cast<int>(i) == cursorPosition)
            buffer << " > ";
        else
            buffer << "   ";

        buffer << messages[i] << "\n";
    }
}

int ConsoleUI::getCursorPosition(){
    return cursorPosition;
}




void ConsoleUI::moveCursorUp() {
    if (cursorPosition > 0) {
        sfx.highClick();
        --cursorPosition;
    }

}

void ConsoleUI::moveCursorDown() {
    if (cursorPosition + 1 < static_cast<int>(messages.size())) {
        sfx.highClick();
        ++cursorPosition;
    }

}

void ConsoleUI::addMessageFromInput() {
    clearScreen();
    sfx.tpLowHealth();
    std::cout << "Enter a message: ";
    std::string input;
    std::getline(std::cin, input);
    messages.push_back(input);
    clearScreen();
}

std::string ConsoleUI::getButton() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::string result;
    int ch = getchar();

    if (ch == 27) { // ESC
        int next1 = getchar();
        if (next1 == '[') {
            int next2 = getchar();

            if (next2 >= '0' && next2 <= '6') {
                int next3 = getchar(); // Should be ~
                switch (next2) {
                    case '1': result = "Home"; break;      // May not trigger
                    case '2': result = "Insert"; break;
                    case '3': result = "Delete"; break;
                    case '4': result = "End"; break;        // May not trigger
                    case '5': result = "PageUp"; break;
                    case '6': result = "PageDown"; break;
                    default:  result = "UnknownEscapeSeq"; break;
                }
            } else {
                // Common alternative Home/End codes
                switch (next2) {
                    case 'A': result = "ArrowUp"; break;
                    case 'B': result = "ArrowDown"; break;
                    case 'C': result = "ArrowRight"; break;
                    case 'D': result = "ArrowLeft"; break;
                    case 'H': result = "Home"; break;  // <- this works in most terminals
                    case 'F': result = "End"; break;   // <- this too
                    default:  result = "UnknownEscapeSeq"; break;
                }
            }
        } else {
            result = "Escape";  // ESC alone
        }
    } else if (ch == 10 || ch == 13) {
        result = "Enter";
    } else if (ch == 127 || ch == 8) {
        result = "Backspace";
    } else if (ch == 9) {
        result = "Tab";
    } else if (ch == 32) {
        result = "Space";
    } else {
        result = std::string(1, static_cast<char>(ch));
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return result;
}

void ConsoleUI::clearScreen() {
    std::cout << "\033c\033[2J\033[H" << std::flush;
}

void ConsoleUI::printToolTips(){
    printTextBox(toolTip, 80);
}

void ConsoleUI::printTextBox(const std::string& content, int width) {
    if (width < 10) width = 10;

    const std::string top    = "+" + std::string(width - 2, '-') + "+";
    const std::string bottom = "+" + std::string(width - 2, '-') + "+";

    // Helper: wrap a single line of text
    auto wrapLine = [](const std::string& text, int maxWidth) {
        std::istringstream words(text);
        std::string word;
        std::vector<std::string> lines;
        std::string current;

        while (words >> word) {
            if (current.length() + word.length() + 1 > static_cast<size_t>(maxWidth)) {
                lines.push_back(current);
                current = word;
            } else {
                if (!current.empty()) current += " ";
                current += word;
            }
        }

        if (!current.empty()) lines.push_back(current);
        return lines;
    };

    // Split on '\n' first
    std::istringstream full(content);
    std::string line;
    std::vector<std::string> wrapped;

    while (std::getline(full, line)) {
        std::vector<std::string> partials = wrapLine(line, width - 2);
        wrapped.insert(wrapped.end(), partials.begin(), partials.end());
    }

    buffer << top << "\n";

    for (const auto& line : wrapped) {
        std::string lineOut = "|" + line;
        lineOut.resize(width - 1, ' ');
        lineOut += "|";
        buffer << lineOut << "\n";
    }

    buffer << bottom << "\n";
}


void ConsoleUI::removeMessageAtCursor() {
    if (!messages.empty() && cursorPosition >= 0 && cursorPosition < static_cast<int>(messages.size())) {
        sfx.lowClick();
        messages.erase(messages.begin() + cursorPosition);
        // Adjust cursor if it now points past the last element
        if (cursorPosition >= static_cast<int>(messages.size())) {
            cursorPosition = static_cast<int>(messages.size()) - 1;
        }
        // If vector is now empty, reset cursorPosition to 0
        if (messages.empty()) {
            cursorPosition = 0;
        }
    }
}


int ConsoleUI::clockInPrompt() {
    std::string input;
    std::string message = "Enter ID for clock In: ";
    sfx.clickSwish();
    
    clearScreen();
    std::cout << message;

    std::cin >> input;

    // Check if input is made entirely of digits (positive integer)
    bool isPositiveInteger = !input.empty() && 
        std::all_of(input.begin(), input.end(), ::isdigit);

    if (!isPositiveInteger) {
        sfx.tpLowHealth(); 
        std::cin.clear(); 
        return -1; // Not a valid integer
    }

    int id = std::stoi(input); // Convert to int

    if (!idData.hasUser(id)) {
        sfx.tpLowHealth(); 
        return -2; // ID doesn't exist
    }
    
    sfx.typewriterKey();
    clearScreen();
    return id; // Success
}

void ConsoleUI::drawUI(const std::string& lastInput) {
    buffer.str("");   // Clear previous contents
    buffer.clear();   // Reset any error flags

    printToolTips();                       // Writes into buffer
    printMessages();                       // Writes into buffer

    std::string buttonMessege = "last Button activated on this menu: ";
    buttonMessege.append(lastInput);
    printTextBox(buttonMessege, 80);  // Writes into buffer

    buffer<< errorMessege;

    clearScreen();                         // Clear screen before redraw
    std::cout << buffer.str() << std::flush;  // Output everything at once
}

void ConsoleUI::populateMessagesFromUsers() {
    messages.clear();

    for (int id : idData.getAllUsers()) {
        messages.push_back(std::to_string(id));  // Convert int to string
    }

    cursorPosition = 0;
}


void ConsoleUI::populateMessagesFromClockRegistry() {
    messages.clear();

    for (const auto& entry : *clockData.getData()) {
        std::string clockEntry = entry.toString();
        messages.push_back(clockEntry);
    }
}





int ConsoleUI::addUserPrompt() {
    std::string input;
    std::string message = "Enter new user ID: ";
    sfx.clickSwish();
    
    clearScreen();
    std::cout << message;

    std::cin >> input;

    // Check if input is made entirely of digits (positive integer)
    bool isPositiveInteger = !input.empty() && 
        std::all_of(input.begin(), input.end(), ::isdigit);

    if (!isPositiveInteger) {
        sfx.tpLowHealth(); 
        std::cin.clear(); 
        return -1; // Not a valid integer
    }

    int id = std::stoi(input); // Convert to int

    if (idData.hasUser(id)) {
        sfx.tpLowHealth(); 
        return -2; // ID doesn't exist
    }
    
    sfx.tpFanfareGoldenBug();

    clearScreen();
    
    idData.addUser(id);

    return id; // Success
}

void ConsoleUI::printClocker(bool clockingOut, int userID){

    std::string heading = "";

    //PRINTING OUT THE HEADING
    if(!clockingOut){
        tmt88v.setFontStyleIndex(1); //Sets to larger font
        tmt88v.setFontScaleIndex(3); //Largest possible letters
        tmt88v.setCutMode(0); //No cutting 
        tmt88v.setCutPadding(false);//no padding since this is a heading.
        heading = "Clock IN\n";
        tmt88v.setText(heading);
        tmt88v.print();


    }
    else{
        tmt88v.setFontStyleIndex(1); //Sets to larger font
        tmt88v.setFontScaleIndex(3); //Largest possible letters
        tmt88v.setCutMode(0); //No cutting 
        tmt88v.setCutPadding(false);//no padding since this is a heading.
        heading = "Clock OUT\n";
        tmt88v.setText(heading);
        tmt88v.print();
    }
    //PRINTING OUT CURRENT TIME AND DATE

    ClockEntry readEntry;

    readEntry = clockData.getLatestClockEntry(userID);

    std::string time = "";

    if(!clockingOut){
        std::chrono::system_clock::time_point clockIn = readEntry.getClockIn();

        time = readEntry.timePointToString(clockIn);

    }
    else{
        std::chrono::system_clock::time_point clockOut = readEntry.getClockOut();
        time = readEntry.timePointToString(clockOut);
    }

    tmt88v.setFontStyleIndex(1); 
    tmt88v.setFontScaleIndex(1); 
    tmt88v.setCutMode(0); //No cutting 
    tmt88v.setCutPadding(false);//no padding since this is mid document.
    tmt88v.setText(time);
    tmt88v.print();

    //PRINTING OUT INFORMATION
    std::ostringstream printBuffer;

    printBuffer << "\n";

    float lifeSec = clockData.getUserLifeTimeSeconds(userID);
    float curWeekSec = clockData.getUserCalendarWeekSeconds(userID);

    printBuffer << "Displaying DATA for user# ["<< userID <<"]\n\n";
    
    printBuffer << "    - "<< "Total Lifetime Hours:\n"
                << "      "<< curWeekSec/3600 << " Hours = ("<< curWeekSec << " seconds" <<")\n\n";
    printBuffer << "    - "<< "Hours this week:\n"
                << "      "<< curWeekSec/3600 << " Hours = ("<< curWeekSec << " seconds" <<")\n\n";

    tmt88v.setFontStyleIndex(0); 
    tmt88v.setFontScaleIndex(0); 
    tmt88v.setCutMode(2); //No cutting 
    tmt88v.setCutPadding(true);//no padding since this is a heading.
    tmt88v.setText(printBuffer.str());
    tmt88v.print();

}

void ConsoleUI::quickSave(){
    clockData.saveToFile();
}

void ConsoleUI::ripUsersFromLoadedRegistry() {
    // Clear the current Users set to avoid duplicates from old data
    idData = Users();

    // Get all clock entries from the registry
    std::vector<ClockEntry>* entries = clockData.getData();
    if (!entries) return;  // Safety check

    // Iterate through all ClockEntry objects
    for (const auto& entry : *entries) {
        int userId = entry.getUserID();
        idData.addUser(userId);  // Users class manages uniqueness internally
    }
}



