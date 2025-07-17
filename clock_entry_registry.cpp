#include "clock_entry_registry.h"

//PRIVATE=================================================================================

ClockEntry* ClockEntryRegistry::findActiveClockEntryByID(int userID) {
    for (ClockEntry& entry : data) {
        if (entry.getUserID() == userID && !entry.isCompleted()) {
            return &entry;
        }
    }
    return nullptr; // Not found
}

//PUBLIC==================================================================================

bool ClockEntryRegistry::clockUser (int userID){

    ClockEntry* clockOutEntry = findActiveClockEntryByID(userID);

    if(clockOutEntry != nullptr ){
        clockOutEntry->clockOutUser();
        return true;
        //The User had an active entry and clocked out
    }
    else{
        ClockEntry newEntry;
        newEntry.setUserID(userID);
        newEntry.clockInUser();
        data.insert(data.begin(), newEntry);
        return false;
        //The user had no active entry and clocked in
    }
    
}

std::vector<ClockEntry>* ClockEntryRegistry::getData(){

    std::vector<ClockEntry>* ptrToVector = &data;

    return ptrToVector;
}

void ClockEntryRegistry::print() {
    if (data.empty()) {
        std::cout << "No clock entries available.\n";
        return;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        data[i].print();  // Example usage
    }



}
