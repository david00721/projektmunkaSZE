#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>

class Page;

class Item
{
private:
    std::string name;
    Page* link;
public:
    Item(std::string _name, Page* _link = nullptr) : name(_name), link(_link) {}
    std::string getName() { return name; }
    Page* getLink() { return link; }
};

#endif
