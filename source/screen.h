#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <iostream>

constexpr short HEIGHT = 8, WIDTH = 32;
constexpr char SELECTIONARROWCHAR = '>';
constexpr char SLIDERARROWCHAR = 'V';
constexpr char HORIZONTALLINECHAR = '=';

class Screen
{
private:
	char screenMatrix[HEIGHT][WIDTH];
public:
	auto getScreenMatrix() { return screenMatrix; }
	void writeLine(short lineNum, std::string input)
	{
		short pos = 0;
		for (; pos < input.length(); pos++) screenMatrix[lineNum][pos] = input[pos];
		for (; pos < WIDTH; pos++) screenMatrix[lineNum][pos] = '\0';
	}
	void clear() { for (short lineNum = 0; lineNum < HEIGHT; lineNum++) writeLine(lineNum, ""); }
	void print() { for (char* line : screenMatrix) std::cout << line << std::endl; }
};

#endif
