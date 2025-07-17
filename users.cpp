#include "users.h"
bool Users::hasUser(int id) const {
    return userIDs.count(id) > 0;
}

void Users::addUser(int id) {
    auto [_, inserted] = userIDs.insert(id);
    if (!inserted) {
        std::cout << "[!] User ID " << id << " already exists.\n";
    }
}

void Users::removeUser(int id) {
    size_t erased = userIDs.erase(id);
    if (erased == 0) {
        std::cout << "[!] User ID " << id << " not found.\n";
    }
}

void Users::printAllUsers() const {
    std::cout << "Current User IDs:\n";
    if (userIDs.empty()) {
        std::cout << "[No Users in memory]\n";
        return;
    }

    for (int id : userIDs) {
        std::cout << " - " << id << "\n";
    }
}

const std::unordered_set<int>& Users::getAllUsers() const {
    return userIDs;
}

bool Users::isEmpty() const {
    return userIDs.empty();
}
