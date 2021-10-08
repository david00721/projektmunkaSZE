#include <iostream>
#include "page.h"

int main()
{
    Page* testPage = new Page("page name");
    Item* testItem = new Item("item name", testPage);
    std::cout << testItem->getLink()->getTitle() << std::endl;
    std::cout << testItem->getName() << std::endl;

    return 0;
}
