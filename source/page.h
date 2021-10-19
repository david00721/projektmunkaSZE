#ifndef __PAGE_H__
#define __PAGE_H__

#include <string>
#include <vector>
#include <utility>
#include "item.h"
#include "screen.h"

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

short calculateSliderArrowPos(short value, Spectrum spectrum, short range)
{
    return range * value / (spectrum.getMax() - spectrum.getMin());
}

class Page {
private:
    std::string title;
public:
    Page(std::string _title) : title(_title) {}
    std::string getTitle() { return title; }

    virtual void move(bool) {}
    virtual Page* ok() { return nullptr; }

    virtual void updateArrow(Screen*) {}
    virtual void loadPage(Screen*) {}
};

class MenuPage : public Page
{
private:
    std::vector<Item*> items;
    short arrowPos = 1;
public:
    MenuPage(std::string _title) : Page(_title) {}
    void addItem(Item* item) { items.push_back(item); }

    void move(bool upDirection)
    {
        if (upDirection && arrowPos == 1) arrowPos = items.size();
        else if (!upDirection && arrowPos == items.size()) arrowPos = 1;
        else upDirection ? arrowPos-- : arrowPos++;
    }
    Page* ok() { return items[arrowPos - 1]->getLink(); }

    void updateArrow(Screen* screen)
    {
        for (short lineNum = 1; lineNum < HEIGHT; lineNum++)
        {
            if (screen->getScreenMatrix()[lineNum][1] == SELECTIONARROWCHAR) screen->getScreenMatrix()[lineNum][1] = ' ';
            if (lineNum == arrowPos) screen->getScreenMatrix()[lineNum][1] = SELECTIONARROWCHAR;
        }
    }
    void loadPage(Screen* screen)
    {
        arrowPos = 1;
        screen->writeLine(0, this->getTitle());
        short lineNum = 1;
        for (; lineNum <= items.size(); lineNum++) screen->writeLine(lineNum, "   " + items[lineNum - 1]->getName());
        for (; lineNum < HEIGHT; lineNum++) screen->writeLine(lineNum, "");
        screen->getScreenMatrix()[arrowPos][1] = SELECTIONARROWCHAR;
    }
};

class ValueSetterPage : public Page
{
private:
    ValueItem* valueItem = nullptr;
public:
    ValueSetterPage(std::string _title) : Page(_title) {}
    ValueItem* getValueItem() { return valueItem; }
    void setValueItem(ValueItem* _valueItem) { valueItem = _valueItem; }

    virtual void foo() = 0;
};

class SliderPage : public ValueSetterPage
{
private:
    Spectrum spectrum;
public:
    SliderPage(std::string _title, Spectrum _spectrum) : ValueSetterPage(_title), spectrum(_spectrum) {}
    
    void move(bool upDirection)
    {
        if (upDirection && getValueItem()->getValue() + spectrum.getStep() > spectrum.getMax()) getValueItem()->setValue(spectrum.getMax());
        else if (!upDirection && getValueItem()->getValue() - spectrum.getStep() < spectrum.getMin()) getValueItem()->setValue(spectrum.getMin());
        else upDirection ? getValueItem()->setValue(getValueItem()->getValue() + spectrum.getStep()) : getValueItem()->setValue(getValueItem()->getValue() - spectrum.getStep());
    }
    Page* ok() { return getValueItem()->getLink(); }

    void updateArrow(Screen* screen)
    {
        screen->writeLine(1, "Value: " + std::to_string(getValueItem()->getValue()));
        screen->writeLine(4, std::string(WIDTH - 1, ' '));
        screen->getScreenMatrix()[4][calculateSliderArrowPos(getValueItem()->getValue(), spectrum, WIDTH)] = SLIDERARROWCHAR;
    }
    void loadPage(Screen* screen)
    {
        updateArrow(screen);
        screen->writeLine(0, getTitle());
        screen->writeLine(2, "Min: " + std::to_string(spectrum.getMin()) + ", Max: " + std::to_string(spectrum.getMax()));
        screen->writeLine(3, "");
        screen->writeLine(5, std::string(WIDTH - 1, HORIZONTALLINECHAR));
        for (short lineNum = 6; lineNum < HEIGHT; lineNum++) screen->writeLine(lineNum, "");
    }
};

class TextOptsPage : public ValueSetterPage
{
private:
    std::vector<std::pair<std::string, short>> opts;
    short arrowPos = 1;
public:
    TextOptsPage(std::string _title) : ValueSetterPage(_title) {}
    void addOpt(std::string _text, short _value) { opts.push_back(std::pair<std::string, short>(_text, _value)); }

    void move(bool upDirection)
    {
        if (upDirection && arrowPos == 1) arrowPos = opts.size();
        else if (!upDirection && arrowPos == opts.size()) arrowPos = 1;
        else upDirection ? arrowPos-- : arrowPos++;
        getValueItem()->setValue(opts[arrowPos - 1].second);
    }
    Page* ok() { return getValueItem()->getLink(); }

    void updateArrow(Screen* screen)
    {
        for (short lineNum = 1; lineNum < HEIGHT; lineNum++)
        {
            if (screen->getScreenMatrix()[lineNum][1] == SELECTIONARROWCHAR) screen->getScreenMatrix()[lineNum][1] = ' ';
            if (lineNum == arrowPos) screen->getScreenMatrix()[lineNum][1] = SELECTIONARROWCHAR;
        }
    }
    void loadPage(Screen* screen)
    {
        arrowPos = 1;
        screen->writeLine(0, this->getTitle());
        short lineNum = 1;
        for (; lineNum <= opts.size(); lineNum++) screen->writeLine(lineNum, "   " + opts[lineNum - 1].first);
        for (; lineNum < HEIGHT; lineNum++) screen->writeLine(lineNum, "");
        screen->getScreenMatrix()[arrowPos][1] = SELECTIONARROWCHAR;
    }
};

#endif
