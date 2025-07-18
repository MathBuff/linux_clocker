#pragma once
#include <unordered_set>
#include <iostream>

class Users {
private:
    std::unordered_set<int> userIDs;

public:
    bool hasUser(int id) const;

    void addUser(int id);

    void removeUser(int id);

    void printAllUsers() const;

    const std::unordered_set<int>& getAllUsers() const; 

    bool isEmpty() const;

    int getUserAtPosition(size_t index) const;

};
