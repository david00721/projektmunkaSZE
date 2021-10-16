#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdexcept>
#include <iostream>
#include "page.h"

constexpr short HEIGHT = 8, WIDTH = 32;

class Screen
{
private:
	char sceenMatrix[HEIGHT][WIDTH];
	void writeLine(short lineNum, std::string input)
	{
		short pos = 0;
		for (; pos < input.length(); pos++) sceenMatrix[lineNum][pos] = input[pos];
		for (; pos < WIDTH; pos++) sceenMatrix[lineNum][pos] = '\0';
	}
	void changeChar(short y, short x, char input) { sceenMatrix[y][x] = input; }
public:
	void loadPage(MenuPage* menuPage)
	{
		menuPage->setArrowPos(1);
		writeLine(0, menuPage->getTitle());
		short lineNum = 1;
		for (; lineNum <= menuPage->itemCount(); lineNum++) writeLine(lineNum, "   " + menuPage->getItem(lineNum - 1)->getName());
		for (; lineNum < HEIGHT; lineNum++) writeLine(lineNum, "");
		changeChar(menuPage->getArrowPos(), 1, '>');
	}
	void updateArrow(MenuPage* menuPage)
	{
		for (short lineNum = 1; lineNum < HEIGHT; lineNum++)
		{
			if (sceenMatrix[lineNum][1] == '>') sceenMatrix[lineNum][1] = ' ';
			if (lineNum == menuPage->getArrowPos()) sceenMatrix[lineNum][1] = '>';
		}
	}
	void print() { for (char* line : sceenMatrix) std::cout << line << std::endl; }
};

#endif
