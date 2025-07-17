#include <iostream>
#include <string>
#include "console_ui.h"
#include <thread>
#include <chrono>

/*
    int newUser = 6531;
    ConsoleUI mainMenu;
    mainMenu.idData.addUser(newUser);
    mainMenu.clockData.clockUser(newUser);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mainMenu.clockData.clockUser(newUser);
    mainMenu.clockData.print();
*/

void userMenu();

void mainMenu();

int main() {
    mainMenu();



    return 0;
}
//--------------------------------------------------------------------------------

void mainMenu(){
    ConsoleUI mainMenu;

    bool program_running = true;
    
    mainMenu.sfx.clickSwish();

    std::string mainMenuToolTip = "MAIN_MENU:\n"
                                  " [ESC] twice to exit\n"
                                  " [SPACE] To Clock In/Out\n"
                                  " [INSERT] To throw some shit in the menu\n"
                                  " [DELETE] To delete shit in the menu\n"
                                  " [TAB] To check user menu\n" 
                                  " [UP/DOWN ARROW] To move the cursor around\n";

    mainMenu.toolTip = mainMenuToolTip;

    std::string input = "None";

    mainMenu.populateMessagesFromClockRegistry();

    mainMenu.drawUI(input);

    while (program_running) {
        std::string input = mainMenu.getButton();


        // Handle Controls
        if (input == "Escape") {
            mainMenu.clearScreen();
            std::cout << "Exited program\n";
            mainMenu.sfx.tpLettersRead();
            break;
        }

        if (input == "Space") {
            int check = mainMenu.clockInPrompt(); //check is both error or id return.

            if (check == -1){
                mainMenu.errorMessege = "*The previously entered ID was not a viable integer, no changes made.*\n";

            } 

            else if(check == -2){
                mainMenu.errorMessege = "*The previously entered ID does not exist, no changes made.*\n";
            }
            else{
                bool doubleCheck = mainMenu.clockData.clockUser (check);
                mainMenu.populateMessagesFromClockRegistry();
                
                if(doubleCheck == false){
                    mainMenu.errorMessege = "*" + std::to_string(check) + " Clocked In*\n";
                }else{
                    mainMenu.errorMessege = "*" + std::to_string(check) + " Clocked Out*\n";
                }
            }


        }

        if(input == "Insert"){
            mainMenu.addMessageFromInput();
        }

        if(input == "ArrowRight"){
            mainMenu.sfx.lightSwitch();
            userMenu();
        }

        if (input == "ArrowUp") {
            mainMenu.moveCursorUp();
        }

        if (input == "ArrowDown") {
            mainMenu.moveCursorDown(); 
        }

        if (input == "Delete") {
            mainMenu.removeMessageAtCursor();
        }

        // Buffered drawing
        mainMenu.drawUI(input);
    }

}

void userMenu(){

std::string userMenuToolTip = "USER_MENU:\n"
                              " [A] to go back to main menu\n"
                              " [SPACE] to add a user\n"
                              " [BACKSPACE] to remove user at cursor from user registry\n"
                              " [UP/DOWN ARROW] To move the cursor around\n";

std::string input = "None";

ConsoleUI userMenu;

userMenu.toolTip = userMenuToolTip;

userMenu.populateMessagesFromUsers();

userMenu.drawUI(input);

while (true) {
    std::string input = userMenu.getButton();
    // Handle Controls

    if (input == "ArrowUp") {
        userMenu.moveCursorUp();
    }

    if (input == "ArrowDown") {
        userMenu.moveCursorDown(); 
    }

    if (input == "ArrowLeft") {
        userMenu.sfx.smolClick();
        return;
    }


    if (input == "Space") {
        int check = userMenu.addUserPrompt();
        if(check ==-1){
            userMenu.errorMessege = "*Ealier Add user command failed because the new id was not valid*\n";
        }

        if(check == -2){
            userMenu.errorMessege = "*Ealier Add user command failed because the new id already exists*\n";
        }
        userMenu.populateMessagesFromUsers();

    }

    if (input == "Backspace") {

    }

    // Buffered drawing
    userMenu.drawUI(input);
}



userMenu.populateMessagesFromUsers();
userMenu.idData.printAllUsers();
}
