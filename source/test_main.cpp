#include <iostream>
#include "page.h"

int main()
{
    Page* testPage = new Page("page name");
    Item* testItem = new Item("item name", testPage);
    std::cout << testItem->getLink()->getTitle() << std::endl;
    std::cout << testItem->getName() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    ValueItem* testValueItem = new ValueItem("value item name", nullptr, 20, "cm");
    std::cout << "value: " << testValueItem->getValue() << std::endl << "unit: " << testValueItem->getUnit() << std::endl;
    testValueItem->setValue(30);
    std::cout << "new value: " << testValueItem->getValue() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    MenuPage* testMenuPage = new MenuPage("menu page name");
    testMenuPage->addItem(testItem);
    testMenuPage->addItem(testValueItem);
    for (short i = 0; i < testMenuPage->itemCount(); i++)
    {
        std::cout << testMenuPage->getItem(i)->getName() << std::endl;
    }
    std::cout << "item count: " << testMenuPage->itemCount() << std::endl;
    std::cout << "arrow position: " << testMenuPage->getArrowPos() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    SliderPage* testSliderPage = new SliderPage("slider page name");
    testSliderPage->setValueItem(testValueItem);
    std::cout << testSliderPage->getValueItem()->getName() << std::endl;
    return 0;
}
