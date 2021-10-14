#ifndef __PAGE_H__
#define __PAGE_H__

#include <string>
#include <vector>
#include "item.h"

class Page {
private:
    std::string title;
public:
    Page(std::string _title) : title(_title) {}
    std::string getTitle() { return title; }
};

class MenuPage : public Page
{
private:
    std::vector<Item*> items;
    short arrowPos = 1;
public:
    MenuPage(std::string _title) : Page(_title) {}
    Item* getItem(short pos) { return items.at(pos); }
    void addItem(Item* item) { items.push_back(item); }
    short itemCount() { return items.size(); }
    short getArrowPos() { return arrowPos; }
    void setArrowPos(short _arrowPos) { arrowPos = _arrowPos; }
};

struct Spectrum
{
    short min, max, step;
    Spectrum(short _min, short _max, short _step) : min(_min), max(_max), step(_step) {}
};

class SliderPage : public Page
{
private:
    ValueItem* valueItem = nullptr;
public:
    SliderPage(std::string _title) : Page(_title) {}
    ValueItem* getValueItem() { return valueItem; }
    void setValueItem(ValueItem* _valueItem) { valueItem = _valueItem; }
};

#endif
