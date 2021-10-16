#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdexcept>
#include <iostream>
#include "page.h"

constexpr short HEIGHT = 8, WIDTH = 32;
constexpr char SELECTIONARROWCHAR = '>';
constexpr char SLIDERARROWCHAR = 'V';
constexpr char HORIZONTALLINECHAR = '=';

class Screen
{
private:
	char screenMatrix[HEIGHT][WIDTH];
	void writeLine(short lineNum, std::string input)
	{
		short pos = 0;
		for (; pos < input.length(); pos++) screenMatrix[lineNum][pos] = input[pos];
		for (; pos < WIDTH; pos++) screenMatrix[lineNum][pos] = '\0';
	}
	short calculateSliderArrowPos(short value, Spectrum spectrum, short range)
	{
		return range * value / (spectrum.getMax() - spectrum.getMin());
	}
public:
	void loadPage(MenuPage* menuPage)
	{
		menuPage->setArrowPos(1);
		writeLine(0, menuPage->getTitle());
		short lineNum = 1;
		for (; lineNum <= menuPage->itemCount(); lineNum++) writeLine(lineNum, "   " + menuPage->getItem(lineNum - 1)->getName());
		for (; lineNum < HEIGHT; lineNum++) writeLine(lineNum, "");
		screenMatrix[menuPage->getArrowPos()][1] = SELECTIONARROWCHAR;
	}
	void updateArrow(MenuPage* menuPage)
	{
		for (short lineNum = 1; lineNum < HEIGHT; lineNum++)
		{
			if (screenMatrix[lineNum][1] == SELECTIONARROWCHAR) screenMatrix[lineNum][1] = ' ';
			if (lineNum == menuPage->getArrowPos()) screenMatrix[lineNum][1] = SELECTIONARROWCHAR;
		}
	}
	void loadPage(SliderPage* sliderPage)
	{
		writeLine(0, sliderPage->getTitle());
		writeLine(1, "Value: " + std::to_string(sliderPage->getValueItem()->getValue()));
		writeLine(2, "Min: " + std::to_string(sliderPage->getSpectrum().getMin()) + ", Max: " + std::to_string(sliderPage->getSpectrum().getMax()));
		writeLine(3, "");
		writeLine(4, std::string(WIDTH - 1, ' '));
		screenMatrix[4][calculateSliderArrowPos(sliderPage->getValueItem()->getValue(), sliderPage->getSpectrum(), WIDTH)] = SLIDERARROWCHAR;
		writeLine(5, std::string(WIDTH-1, HORIZONTALLINECHAR));
		for (short lineNum = 6; lineNum < HEIGHT; lineNum++) writeLine(lineNum, "");
	}
	void print() { for (char* line : screenMatrix) std::cout << line << std::endl; }
	
};

#endif
