#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "console_ui.h"
#include "sound.h"

void recordMenu();

void userMenu();

void mainMenu();

void inspectUserPanel(int userID);

void inspectUserPanelRenderer(ConsoleUI& data, int userID, std::string input);

void manualClockEntry();

std::chrono::system_clock::time_point promptTimePoint(bool& successConditionReturn);

