#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <iostream>
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

constexpr short HEIGHT = 8, WIDTH = 15;
constexpr char SELECTIONARROWCHAR = '>';
constexpr char SLIDERARROWCHAR = 'V';
constexpr char HORIZONTALLINECHAR = '=';

class Screen
{
private:
	char screenMatrix[HEIGHT][WIDTH];
  
public:
	auto getScreenMatrix() { return screenMatrix; }
	static void writeLine(short lineNum, std::string input)
	{
		oled.setTextXY(lineNum, 0);
    oled.putString(input.c_str());
	}
 static void writeChar(short lineNum, short colomnNum, char character)
 {
    oled.setTextXY(lineNum, colomnNum);
    oled.putString(&character);
 }
	void clear() { for (short lineNum = 0; lineNum < HEIGHT; lineNum++) writeLine(lineNum, ""); }
	void print() { for (char* line : screenMatrix) std::cout << line << std::endl; }
};

#endif
