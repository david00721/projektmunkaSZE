#include "buttons.h"

//1: OK - 2: UP - 3: DOWN
std::vector<short>inputSequence = { 0, 1, 0 };

void runSimulation(std::vector<short> inputSequence, Page* page, Screen* screen)
{
    std::cout << "Opening the menu...\n\n"; openAction(page, screen);
    for (short currentInput : inputSequence)
    {
        if (currentInput == 0)
        {
            std::cout << "\nPressing the OK button...\n\n";
            page = okButtonAction(page, screen);
        }
        else if (currentInput == 1)
        {
            std::cout << "\nPressing the up button...\n\n";
            upButtonAction(page, screen);
        }
        else if (currentInput == 2)
        {
            std::cout << "\nPressing the down button...\n\n";
            downButtonAction(page, screen);
        }
    }
}

int main()
{
    MenuPage mainMenuPage = MenuPage("MAIN MENU");
    MenuPage settingsPage = MenuPage("SETTINGS");
    MenuPage monitoringPage = MenuPage("MONITORING");
    MenuPage logsPage = MenuPage("LOGS");

    Item backToMainMenuPageItem = Item("Back", &mainMenuPage);
    Item settingsItem = Item("Settings", &settingsPage);
    Item monitoringItem = Item("Monitoring", &monitoringPage);
    Item logsItem = Item("Logs", &logsPage);
    
    ValueItem lightSensitivityItem = ValueItem("Light Sensitivity", &settingsPage, 10, "");
    ValueItem timerItem = ValueItem("Timer", &settingsPage, 60, "sec");

    mainMenuPage.addItem(&settingsItem);
    mainMenuPage.addItem(&monitoringItem);
    mainMenuPage.addItem(&logsItem);

    settingsPage.addItem(&lightSensitivityItem);
    settingsPage.addItem(&timerItem);
    settingsPage.addItem(&backToMainMenuPageItem);

    monitoringPage.addItem(&backToMainMenuPageItem);

    Screen screen;
    
    runSimulation(inputSequence, &mainMenuPage, &screen);

    return 0;
}
