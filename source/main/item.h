#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>
#include <vector>

class Page;

/**
*This class can be a part of a page. 
*It has a name and a pointer to the page it belongs to.
*/
class Item
{
private:
    std::string name;
    Page* link;
public:
    Item(std::string _name, Page* _link) : name(_name), link(_link) {}
    std::string getName() { return name; }
    Page* getLink() { return link; }
};


/**
*This class is to store values that can be changed throught the interface.
*It has an integer value and a unit text.
*/
class ValueItem : public Item
{
private:
    short value;
    std::string unit;
public:
    ValueItem(std::string _name, Page* _link, short _value, std::string _unit = "") :
        Item(_name, _link), value(_value), unit(_unit) {}
    short getValue() { return value; }
    void setValue(short _value) { value = _value; }
    std::string getUnit() { return unit; }
};

#endif
