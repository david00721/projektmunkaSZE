#ifndef __PAGE_H__
#define __PAGE_H__

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
    Page* back;
public:
    Page(std::string _title, Page* _back) : title(_title), back(_back) {}
    std::string getTitle() { return title; }
    Page* getBack() { return back; }

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
    MenuPage(std::string _title, Page* _back = nullptr) : Page(_title, _back) {}
    void addItem(Item* item) { items.push_back(item); }

    void move(bool upDirection)
    {
        if (upDirection && arrowPos == 1) arrowPos = items.size() + 1;
        else if (!upDirection && arrowPos == items.size() + 1) arrowPos = 1;
        else upDirection ? arrowPos-- : arrowPos++;
    }
    Page* ok()
    {
        if (arrowPos != items.size() + 1) return items[arrowPos - 1]->getLink();
        else return getBack();
    }

    void updateArrow(Screen* screen)
    {
        for (short lineNum = 1; lineNum < HEIGHT; lineNum++)
        {
            screen->writeChar(lineNum, 1, ' ');
        }
        screen->writeChar(arrowPos, 1, SELECTIONARROWCHAR);
    }
    void loadPage(Screen* screen)
    {
        arrowPos = 1;
        screen->writeLine(0, getTitle());
        short lineNum = 1;
        for (; lineNum <= items.size(); lineNum++) screen->writeLine(lineNum, "   " + items[lineNum - 1]->getName());
        getBack() == nullptr ? screen->writeLine(lineNum, "   Exit") : screen->writeLine(lineNum, "   Back"); lineNum++;
        for (; lineNum < HEIGHT; lineNum++) screen->writeLine(lineNum, "");
        updateArrow(screen);
    }
};

class ValueSetterPage : public Page
{
private:
    ValueItem* valueItem = nullptr;

    virtual void foo() = 0;
protected:
    ValueItem* getValueItem() { return valueItem; }
public:
    ValueSetterPage(std::string _title, Page* _back) : Page(_title, _back) {}
    void setValueItem(ValueItem* _valueItem) { valueItem = _valueItem; }
};

class SliderPage : public ValueSetterPage
{
private:
    Spectrum spectrum;

    void foo() {}
public:
    SliderPage(std::string _title, Page* _back, Spectrum _spectrum) : ValueSetterPage(_title, _back), spectrum(_spectrum) {}
    
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

    void foo() {}
public:
    TextOptsPage(std::string _title, Page* _back) : ValueSetterPage(_title, _back) {}
    void addOpt(std::string _text, short _value) { opts.push_back(std::pair<std::string, short>(_text, _value)); }
    short getFirstOptionValue() { return opts[0].second; }

    void move(bool upDirection)
    {
        if (upDirection && arrowPos == 1) arrowPos = opts.size();
        else if (!upDirection && arrowPos == opts.size()) arrowPos = 1;
        else upDirection ? arrowPos-- : arrowPos++;
        getValueItem()->setValue(opts[arrowPos - 1].second);
    }
    Page* ok() { return getBack(); }

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
        screen->writeLine(0, getTitle());
        short lineNum = 1;
        for (; lineNum <= opts.size(); lineNum++) screen->writeLine(lineNum, "   " + opts[lineNum - 1].first);
        for (; lineNum < HEIGHT; lineNum++) screen->writeLine(lineNum, "");
        screen->getScreenMatrix()[arrowPos][1] = SELECTIONARROWCHAR;
    }
};

#endif
