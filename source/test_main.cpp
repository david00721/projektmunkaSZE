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
    MenuPage mainMenu = MenuPage("MAIN MENU");
    MenuPage settingsMenu = MenuPage("SETTINGS");
    MenuPage monitoringMenu = MenuPage("MONITORING");
    MenuPage logsMenu = MenuPage("LOGS");

    Item backToMainMenuItem = Item("Back", &mainMenu);
    Item settingsItem = Item("Settings", &settingsMenu);
    Item monitoringItem = Item("Monitoring", &monitoringMenu);
    Item logsItem = Item("Logs", &logsMenu);
    
    ValueItem lightSensitivity = ValueItem("Light Sensitivity", &settingsMenu, 10, "");
    ValueItem timer = ValueItem("Timer", &settingsMenu, 60, "sec");

    mainMenu.addItem(&settingsItem);
    mainMenu.addItem(&monitoringItem);
    mainMenu.addItem(&logsItem);

    settingsMenu.addItem(&lightSensitivity);
    settingsMenu.addItem(&timer);
    settingsMenu.addItem(&backToMainMenuItem);

    monitoringMenu.addItem(&backToMainMenuItem);

    Screen screen;
    
    runSimulation(inputSequence, &mainMenu, &screen);

    return 0;
}
