#include "buttons.h"

//0: OK - 1: UP - 2: DOWN
std::vector<short>inputSequence = { 0, 2, 0, 1, 1, 1, 1};

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
    //creating pages

    MenuPage mainMenuPage = MenuPage("MAIN MENU");
    MenuPage settingsPage = MenuPage("SETTINGS", &mainMenuPage);
    MenuPage monitoringPage = MenuPage("MONITORING", &mainMenuPage);
    MenuPage logsPage = MenuPage("LOGS", &mainMenuPage);

    TextOptsPage lightSensitivityPage = TextOptsPage("LIGHT SENSITIVITY", &settingsPage);
    lightSensitivityPage.addOpt("Low", 10);
    lightSensitivityPage.addOpt("Medium", 20);
    lightSensitivityPage.addOpt("High", 30);

    SliderPage timerPage = SliderPage("TIMER", &settingsPage, Spectrum(0, 100, 10));

    //creating items

    Item settingsItem = Item("Settings", &settingsPage);
    Item monitoringItem = Item("Monitoring", &monitoringPage);
    Item logsItem = Item("Logs", &logsPage);
    
    ValueItem lightSensitivityItem = ValueItem("Light Sensitivity", &lightSensitivityPage, lightSensitivityPage.getFirstOptionValue(), "");
    ValueItem timerItem = ValueItem("Timer", &timerPage, 60, "sec");

    //filling up pages

    mainMenuPage.addItem(&settingsItem);
    mainMenuPage.addItem(&monitoringItem);
    mainMenuPage.addItem(&logsItem);

    settingsPage.addItem(&lightSensitivityItem);
    settingsPage.addItem(&timerItem);

    lightSensitivityPage.setValueItem(&lightSensitivityItem);

    timerPage.setValueItem(&timerItem);

    //creaing the screen and running the simulation on it

    Screen screen;
    
    runSimulation(inputSequence, &mainMenuPage, &screen);

    return 0;
}
