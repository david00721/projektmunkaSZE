#ifndef __PAGE_H__
#define __PAGE_H__

#include <utility>
#include "item.h"
#include "screen.h"

/**
*This struct defines a range and a step size for an integer value.
*/
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

/**
*This function proportionates a 'Spectrum' range to a screen range to determine the slider arrow position.
*@param Value to be positioned.
*@param Value range.
*@param Screen real estate (available width).
*@return Intiger position on the screen matrix.
*/
unsigned short calculateSliderArrowPos(short value, Spectrum spectrum, short range)
{
    return (unsigned short)(range * value / (spectrum.getMax() - spectrum.getMin()));
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

    virtual void updateArrow() {}
    virtual void loadPage() {}
};

/**
*This class stores items to be displayed. Some can be selected to show other pages.
*It contains pointers to the items it can displayed and an arrow position to the currently selectable item.
*/
class MenuPage : public Page
{
private:
    std::vector<Item*> items;
    short arrowPos = 1;
public:
    MenuPage(std::string _title, Page* _back = nullptr) : Page(_title, _back) {}
    /**
    *Adds an item to the ones that the menu page contains.
    *@param Pointer to the item to add.
    */
    void addItem(Item* item) { items.push_back(item); }

    /**
    *Moves the selection arrow position by a step in a choosen direction.
    *It can go circularly between the first and last options.
    *@param Direction of movement. True for upwards, false for downwards.
    */
    void move(bool upDirection)
    {
        if (upDirection && arrowPos == 1) arrowPos = items.size() + 1;
        else if (!upDirection && arrowPos == items.size() + 1) arrowPos = 1;
        else upDirection ? arrowPos-- : arrowPos++;
    }
    
    /**
    *Executes option selection. Either returns the page link of the selected item, or goes back/exits if the last option is selected.
    *@return Pointer to the page based on the selection.
    */
    Page* ok()
    {
        if (arrowPos != items.size() + 1) return items[arrowPos - 1]->getLink();
        else return getBack();
    }
    
    /**
    *Updates the part of the screen where the arrow is displayed.
    *The second colomn of every colomn (except of the title colomn) is replaced with a space character, then the selection arrow character is placed at the right position.
    */
    void updateArrow()
    {
        for (short lineNum = 1; lineNum < HEIGHT; lineNum++)
        {
            Screen::writeChar(lineNum, 1, ' ');
        }
        Screen::writeChar(arrowPos, 1, SELECTIONARROWCHAR);
    }

    /**
    *Loads the page on the screen.
    *Resets the arrow position, displays the title, lists the options, clears the remaining of the screen, updates the arrow.
    */
    void loadPage()
    {
        arrowPos = 1;
        Screen::writeLine(0, getTitle());
        short lineNum = 1;
        for (; lineNum <= items.size(); lineNum++) Screen::writeLine(lineNum, "   " + items[lineNum - 1]->getName());
        getBack() == nullptr ? Screen::writeLine(lineNum, "   Exit") : Screen::writeLine(lineNum, "   Back");
        lineNum++;
        for (; lineNum < HEIGHT; lineNum++) Screen::writeLine(lineNum, "");
        updateArrow();
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

/**
*This class stores a value item to be displayed and changed as a slider.
*It contains a spectrum for referance to the value item.
*/
class SliderPage : public ValueSetterPage
{
private:
    Spectrum spectrum;

    void foo() {}
public:
    SliderPage(std::string _title, Page* _back, Spectrum _spectrum) : ValueSetterPage(_title, _back), spectrum(_spectrum) {}

    /**
    *Increases the value according to the direction.
    *If it would exceeds the maximum or minimum values, these exact values are set.
    *@param Direction of movement. True for positive, false for negative.
    */
    void move(bool upDirection)
    {
        if (upDirection && getValueItem()->getValue() + spectrum.getStep() > spectrum.getMax()) getValueItem()->setValue(spectrum.getMax());
        else if (!upDirection && getValueItem()->getValue() - spectrum.getStep() < spectrum.getMin()) getValueItem()->setValue(spectrum.getMin());
        else upDirection ? getValueItem()->setValue(getValueItem()->getValue() + spectrum.getStep()) : getValueItem()->setValue(getValueItem()->getValue() - spectrum.getStep());
    }

    /**
    *Leaves the value setter page.
    *@return Pointer to the back page.
    */
    Page* ok() { return getValueItem()->getLink(); }

    /**
    *Updates the part of the screen where the arrow and value are displayed.
    *Rewrites the value, clears the line of the arrow, places the arrow at the correct position.
    */
    void updateArrow()
    {
        Screen::writeLine(1, "Value: " + std::to_string(getValueItem()->getValue()));
        Screen::writeLine(4, std::string(WIDTH - 1, ' '));
        Screen::writeChar(4, calculateSliderArrowPos(getValueItem()->getValue(), spectrum, WIDTH), SLIDERARROWCHAR);
    }
    /**
    *Loads the page on the screen.
    *Displays the title, minimum and maximum values, value and slider arrow, clears all other lines of the screen.
    */
    void loadPage()
    {
        Screen::writeLine(0, getTitle());
        Screen::writeLine(2, "Min: " + std::to_string(spectrum.getMin()) + ", Max: " + std::to_string(spectrum.getMax()));
        Screen::writeLine(3, "");
        Screen::writeLine(5, std::string(WIDTH - 1, HORIZONTALLINECHAR));
        for (short lineNum = 6; lineNum < HEIGHT; lineNum++) Screen::writeLine(lineNum, "");
        updateArrow();
    }
};

/**
*This class stores a value item to be displayed and changed via text options.
*It contains a list of options to choose from and an arrow position to the currently selectable item.
*/
class TextOptsPage : public ValueSetterPage
{
private:
    std::vector<std::pair<std::string, short>> opts;
    short arrowPos = 1;
    
    void foo() {}
public:
    TextOptsPage(std::string _title, Page* _back) : ValueSetterPage(_title, _back) {}

    /**
    *Adds a text option to the ones that the text option page contains.
    *@param Text of the option.
    *@param Intiger value corrisponding to the option.
    */
    void addOpt(std::string _text, short _value) { opts.push_back(std::pair<std::string, short>(_text, _value)); }
    
    /**
    *Provides a default value by the first option. 
    *@return Value of the first option.
    */
    short getFirstOptionValue() { return opts[0].second; }
    
    /**
    *Moves the selection arrow position by a step in a choosen direction, and changes the value accordingly.
    *It can go circularly between the first and last options.
    *@param Direction of movement. True for upwards, false for downwards.
    */
    void move(bool upDirection)
    {
        if (upDirection && arrowPos == 1) arrowPos = opts.size();
        else if (!upDirection && arrowPos == opts.size()) arrowPos = 1;
        else upDirection ? arrowPos-- : arrowPos++;
        getValueItem()->setValue(opts[arrowPos - 1].second);
    }
    /**
    *Leaves the value setter page.
    *@return Pointer to the back page.
    */
    Page* ok() { return getBack(); }

    /**
    *Updates the part of the screen where the arrow is displayed.
    *The second colomn of every colomn (except of the title colomn) is replaced with a space character, then the selection arrow character is placed at the right position.
    */
    void updateArrow()
    {
        for (short lineNum = 1; lineNum < HEIGHT; lineNum++)
        {
            if (lineNum != arrowPos) Screen::writeChar(lineNum, 1, ' ');
            else Screen::writeChar(lineNum, 1, SELECTIONARROWCHAR);
        }
    }

    /**
    *Loads the page on the screen.
    *Resets the arrow position, displays the title, lists the options, clears the remaining of the screen, updates the arrow.
    */
    void loadPage()
    {
        arrowPos = 1;
        Screen::writeLine(0, getTitle());
        short lineNum = 1;
        for (; lineNum <= opts.size(); lineNum++) Screen::writeLine(lineNum, "   " + opts[lineNum - 1].first);
        for (; lineNum < HEIGHT; lineNum++) Screen::writeLine(lineNum, "");
        Screen::writeChar(arrowPos, 1, SELECTIONARROWCHAR);
        updateArrow();
    }
};

#endif
