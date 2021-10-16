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

	template<class T>
	void loadAnySelectionPage(T* selectionPage)
	{
		selectionPage->setArrowPos(1);
		writeLine(0, selectionPage->getTitle());
		short lineNum = 1;
		for (; lineNum <= selectionPage->selectionCount(); lineNum++) writeLine(lineNum, "   " + selectionPage->getSelectionText(lineNum - 1));
		for (; lineNum < HEIGHT; lineNum++) writeLine(lineNum, "");
		screenMatrix[selectionPage->getArrowPos()][1] = SELECTIONARROWCHAR;
	}
	template<class T>
	void updateAnySelectionArrow(T* selectionPage)
	{
		for (short lineNum = 1; lineNum < HEIGHT; lineNum++)
		{
			if (screenMatrix[lineNum][1] == SELECTIONARROWCHAR) screenMatrix[lineNum][1] = ' ';
			if (lineNum == selectionPage->getArrowPos()) screenMatrix[lineNum][1] = SELECTIONARROWCHAR;
		}
	}

public:
	void loadPage(MenuPage* menuPage) { loadAnySelectionPage(menuPage); }
	void updateArrow(MenuPage* menuPage) { updateAnySelectionArrow(menuPage); }
	void updateArrow(SliderPage* sliderPage)
	{
		writeLine(1, "Value: " + std::to_string(sliderPage->getValueItem()->getValue()));
		writeLine(4, std::string(WIDTH - 1, ' '));
		screenMatrix[4][calculateSliderArrowPos(sliderPage->getValueItem()->getValue(), sliderPage->getSpectrum(), WIDTH)] = SLIDERARROWCHAR;
	}
	void loadPage(SliderPage* sliderPage)
	{
		updateArrow(sliderPage);
		writeLine(0, sliderPage->getTitle());
		writeLine(2, "Min: " + std::to_string(sliderPage->getSpectrum().getMin()) + ", Max: " + std::to_string(sliderPage->getSpectrum().getMax()));
		writeLine(3, "");
		writeLine(5, std::string(WIDTH-1, HORIZONTALLINECHAR));
		for (short lineNum = 6; lineNum < HEIGHT; lineNum++) writeLine(lineNum, "");
	}
	void loadPage(TextOptsPage* textOptsPage) { loadAnySelectionPage(textOptsPage); }
	void updateArrow(TextOptsPage* textOptsPage) { updateAnySelectionArrow(textOptsPage); }
	void print() { for (char* line : screenMatrix) std::cout << line << std::endl; }
};

#endif
