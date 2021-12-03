#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

constexpr short HEIGHT = 8, WIDTH = 16;

/**
*This class contains functions to use the screen easily.
*/
class Screen
{  
public:
  /**
  *This function replaces the text in a specific line.
  *@param Number of the line.
  *@param Text to write.
  */
	static void writeLine(short lineNum, std::string input)
	{
    input += std::string(WIDTH - input.length(), ' ');
		oled.setTextXY(lineNum, 0);
    oled.putString(input.c_str());
	}

  /**
  *This function replaces the character in a specific position.
  *@param Number of the line.
  *@param Number of the colomn.
  *@param Character to write.
  */
  static void writeChar(short lineNum, short colomnNum, char character)
  {
    oled.setTextXY(lineNum, colomnNum);
    oled.putString(&character);
  }
  
  /**
  *This function clears the screen. All characters will disappear.
  */
  static void clear() { oled.clearDisplay(); }
};

#endif
