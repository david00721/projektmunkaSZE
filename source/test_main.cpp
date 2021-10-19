#include "buttons.h"

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

    Screen* screen = new Screen;
    Page* currentPage = &mainMenu;
    
    std::cout << std::endl << "Opening the menu... " << std::endl << std::endl;
    openAction(currentPage, screen);

    std::cout << std::endl << "Pressing the OK button... " << std::endl << std::endl;
    currentPage = okButtonAction(currentPage, screen);

    std::cout << std::endl << "Pressing the UP button... " << std::endl << std::endl;
    upButtonAction(currentPage, screen);

    std::cout << std::endl << "Pressing the OK button... " << std::endl << std::endl;
    currentPage = okButtonAction(currentPage, screen);

    return 0;
}
