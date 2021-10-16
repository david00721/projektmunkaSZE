#ifndef __PAGE_H__
#define __PAGE_H__

#include <string>
#include <vector>
#include <utility>
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
    short getArrowPos() { return arrowPos; }
    void setArrowPos(short _arrowPos) { arrowPos = _arrowPos; }

    std::string getSelectionText(short pos) { return items[pos]->getName(); }
    short selectionCount() { return items.size(); }
};

class ValueSetterPage : public Page
{
private:
    ValueItem* valueItem = nullptr;
public:
    ValueSetterPage(std::string _title) : Page(_title) {}
    ValueItem* getValueItem() { return valueItem; }
    void setValueItem(ValueItem* _valueItem) { valueItem = _valueItem; }
};

struct Spectrum
{
private:
    short min, max, step;
public:
    Spectrum(short _min, short _max, short _step) : min(_min), max(_max), step(_step) {}
    short getMin() { return min; }
    short getMax() { return max; }
    short getStep() { return step; }
};

class SliderPage : public ValueSetterPage
{
private:
    Spectrum spectrum;
public:
    SliderPage(std::string _title, Spectrum _spectrum) : ValueSetterPage(_title), spectrum(_spectrum) {}
    Spectrum getSpectrum() { return spectrum; }
};

class TextOptsPage : public ValueSetterPage
{
private:
    std::vector<std::pair<std::string, short>> opts;
    short arrowPos = 1;
public:
    TextOptsPage(std::string _title) : ValueSetterPage(_title) {}
    void addOpt(std::string _text, short _value) { opts.push_back(std::pair<std::string, short>(_text, _value)); }
    short getSelectionValue(short pos) { return opts[pos].second; }
    short getArrowPos() { return arrowPos; }
    void setArrowPos(short _arrowPos) { arrowPos = _arrowPos; }

    std::string getSelectionText(short pos) { return opts[pos].first; }
    short selectionCount() { return opts.size(); }
};

#endif
