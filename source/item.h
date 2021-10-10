#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>
#include <vector>
#include <utility>

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

struct Spectrum
{
    short min, max, step;
    Spectrum(short _min, short _max, short _step) : min(_min), max(_max), step(_step) {}
};

class ValueItem : public Item
{
private:
    short value;
    Spectrum spectrum;
    std::string unit;
public:
    ValueItem(std::string _name, Page* _link, short _value, Spectrum _spectrum, std::string _unit) :
        Item(_name, _link), value(_value), spectrum(_spectrum), unit(_unit) {}
    short getValue() { return value; }
    void setValue(short _value) { value = _value; }
    Spectrum getSpectrum() { return spectrum; }
    std::string getUnit() { return unit; }
};

class TextOptsItem : public Item
{
private:
    short value;
    std::vector<std::pair<std::string, short>> opts;
public:
    TextOptsItem(std::string _name, Page* _link, short _value) :
        Item(_name, _link), value(_value) {}
    short getValue() { return value; }
    void setValue(short _value) { value =  _value; }
    std::string getOptText(short pos) { return opts[pos].first; }
    short getOptValue(short pos) { return opts[pos].second; }
    void addOpt(std::string optText, short optValue) { opts.push_back(std::pair<std::string, short>(optText, optValue)); }
};

#endif
