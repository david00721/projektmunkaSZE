#include <iostream>
#include "page.h"

int main()
{
    Page* testPage = new Page("page name");
    Item* testItem = new Item("item name", testPage);
    std::cout << testItem->getLink()->getTitle() << std::endl;
    std::cout << testItem->getName() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    ValueItem* testValueItem = new ValueItem("value item name", nullptr, 20, { 0, 100, 10 }, "cm");
    std::cout << "value: " << testValueItem->getValue() << std::endl
        << "min: " << testValueItem->getSpectrum().min << std::endl
        << "max: " << testValueItem->getSpectrum().max << std::endl
        << "step: " << testValueItem->getSpectrum().step << std::endl
        << "unit: " << testValueItem->getUnit() << std::endl;
    testValueItem->setValue(30);
    std::cout << "new value: " << testValueItem->getValue() << std::endl;
    return 0;
}
