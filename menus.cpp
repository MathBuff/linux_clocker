#include "menus.h"


void mainMenu(){
    ConsoleUI mainMenu;

    bool program_running = true;
    
    mainMenu.sfx.clickSwish();

    std::string mainMenuToolTip = "MAIN_MENU:\n"
                                  " [ESC] twice to exit\n"
                                  " [SPACE] To Clock In/Out\n"
                                  " [INSERT] (not done) To manually enter a Clock Entry\n"
                                  " [DELETE] To delete a Clock Entry\n"
                                  " [RIGHT ARROW] To check user menu\n" 
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
                mainMenu.errorMessege = "*The previously entered ID is not on user list, no changes made.*\n";
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
            //mainMenu.addMessageFromInput();
            manualClockEntry();
            mainMenu.errorMessege = "";
            mainMenu.populateMessagesFromClockRegistry();
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
//          mainMenu.removeMessageAtCursor();
            mainMenu.sfx.lowClick();
            mainMenu.clockData.deleteAtPosition(mainMenu.getCursorPosition());
            mainMenu.populateMessagesFromClockRegistry();
            mainMenu.errorMessege = "*ClockEntry was deleted*\n";
        }

        // Buffered drawing
        mainMenu.drawUI(input);
    }

}

//---------------------------------------------------------------------------------------------------

void userMenu(){

std::string userMenuToolTip = "USER_MENU:\n"
                              "[LEFT ARROW] to go back to main menu\n"
                              "[SPACE] to add a user\n"
                              "[RIGHT ARROW] to inspect a user\n\n"
                              "[Delete]\n" 
                              "->[temperarily pauses user clocking.]\n" 
                              "->[To pernamently delete a user]:\n"
                              "->[Manually delete all of the users clock entries.]\n"
                              "->[Then exit the program.]\n\n"
                              "[UP/DOWN ARROW] To move the cursor around\n";

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

    if (input == "ArrowRight") {
        userMenu.sfx.lightSwitch();

        if(userMenu.idData.isEmpty()){
            userMenu.errorMessege = "*There are no Users to inspect*\n";
        }
        else{
            int userID = userMenu.idData.getUserAtPosition(userMenu.getCursorPosition());//got to get user at this position
            inspectUserPanel(userID);
        }

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

    if (input == "Delete") {
        int userID = userMenu.idData.getUserAtPosition(userMenu.getCursorPosition());
        userMenu.idData.removeUser(userID);
        userMenu.populateMessagesFromUsers();
        userMenu.sfx.lowClick();

    }

    // Buffered drawing
    userMenu.drawUI(input);
}

}

//-------------------------------------------------------------------------------------------------------

void inspectUserPanel(int userID){
    ConsoleUI data;
    std::string input = "None";
    data.toolTip = "USER_INSPECTION_PANEL:\n"
                   "[LEFT ARROW] goes back to user menu\n";

    inspectUserPanelRenderer(data, userID, input);

    while (true) {
        std::string input = data.getButton();

        if (input == "ArrowLeft"){
                data.sfx.smolClick();
                return;
        }

        inspectUserPanelRenderer(data, userID, input); 
    }
}

void inspectUserPanelRenderer(ConsoleUI& data, int userID,std::string input){
     data.buffer.str("");   
    data.buffer.clear();

    data.printToolTips();//Writes into buffer Memory not console
    float lifeHours = data.clockData.getUserLifeTimeHours(userID);

    data.buffer << "\n";

    data.buffer << "   " << "Displaying DATA for user# ["<< userID <<"]\n\n";
    
    data.buffer << "    - "<< "Total Lifetime Hours: "<< lifeHours << " HRS\n\n";

    std::string buttonMessege = "last Button activated on this menu: ";
    buttonMessege.append(input);
    data.printTextBox(buttonMessege, 70);  // Writes into buffer

    data.clearScreen();                         // Clear screen before redraw
    std::cout << data.buffer.str() << std::flush;  // Output everything at once
}

//-------------------------------------------------------------------------------------------------------


void manualClockEntry(){


ConsoleUI panel;
ClockEntry newEntry;

panel.clearScreen();

std::chrono::system_clock::time_point clockInTime;

std::chrono::system_clock::time_point clockOutTime;

bool successConditionReturn = true;

//^Declarations^

//Enter User:

panel.sfx.cameraShutter();

std::cout << "Please Enter an existing Users ID: ";

int userID;

std::cin >> userID; 
std::cout << "\n";

std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // <-- Add this

if(panel.idData.hasUser(userID) == false){
        panel.sfx.tpLowHealth();
        std::cout << "No user in memory matches the entry, manual clock entry cancelled.\n\n"

                     "Please press any button to continue...\n";

        panel.getButton();
        panel.sfx.reload1();
        return;
}

panel.sfx.cameraShutter();

//Setting the user to the clock Entry

newEntry.setUserID(userID);

//Clock In sequence:

std::cout << "Please Enter the clock IN time:\n";

clockInTime = promptTimePoint(successConditionReturn);

    if(successConditionReturn != true){
        panel.sfx.tpLowHealth();
        std::cout <<"The Clock IN value entered failed, manual clock entry cancelled.\n\n"

                    "Please press any button to continue...\n";

        panel.getButton();
        panel.sfx.reload1();
        return;
    }

panel.sfx.cameraShutter();

newEntry.setClockIn(clockInTime);

//Clock Out Sequence:

std::cout << "Please Enter the clock out time:\n";

clockOutTime = promptTimePoint(successConditionReturn);

    if(successConditionReturn != true){
        panel.sfx.tpLowHealth();
        std::cout << "The Clock OUT value entered failed, manual clock entry cancelled.\n\n"

                     "Please press any button to continue...\n";

        panel.getButton();
        panel.sfx.reload1();
        return;
    }

newEntry.setClockOut(clockOutTime);

//Set Entry to completed:

newEntry.setEntryComplete();

//Calculate Shift Time

newEntry.calculateShiftTimeSeconds();

//Enter New entry into registry:

panel.clockData.add(newEntry);

//Done

panel.sfx.typewriterKey();

std::cout << "Manual Entry for " << userID << " was successful.\n\n"
             "Please Press any button to return to the main menu.\n";

panel.getButton();
panel.sfx.reload1();

}

//-----------------------------------------------
std::chrono::system_clock::time_point promptTimePoint(bool& successConditionReturn){
    Sound sfx;
    std::tm tm = {};
    std::string input;

    std::cout << "(Format: YYYY-MM-DD HH:MM:SS)\n"
                 "       ->";
    std::cout.flush();
    std::getline(std::cin, input);

    sfx.reload1();

    std::cout << "\n";

    std::istringstream ss(input);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) {
        std::cerr << "*Failed to parse time.*\n";
        successConditionReturn = false; //to indicate failure.
        return std::chrono::system_clock::now();
    }

    std::time_t time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}


/*
void recordMenu(){

std::string userMenuToolTip = "RECORD_MENU:\n"
                              " [LEFT ARROW] to go back to USER MENU\n"
                              " [SPACE] to add a user\n"
                              " [UP/DOWN ARROW] To move the cursor around\n";

std::string input = "None";

ConsoleUI recordMenu;

recordMenu.toolTip = userMenuToolTip;

recordMenu.populateMessagesFromUsers();

recordMenu.drawUI(input);

while (true) {
    std::string input = recordMenu.getButton();
    // Handle Controls

    if (input == "ArrowUp") {
        recordMenu.moveCursorUp();
    }

    if (input == "ArrowDown") {
        recordMenu.moveCursorDown(); 
    }

    if (input == "ArrowLeft") {
        recordMenu.sfx.smolClick();
        return;
    }


    // Buffered drawing
    recordMenu.drawUI(input);
}

}
*/


