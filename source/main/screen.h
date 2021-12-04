#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
    display.setCursor(0, lineNum * 10);
    display.println(input.c_str());
    display.display();
    /*
		oled.setTextXY(lineNum, 0);
    oled.putString(input.c_str());
    */
	}
  
  /**
  *This function clears the screen. All characters will disappear.
  */
  static void clear() { display.clearDisplay();/*oled.clearDisplay();*/ }
};

#endif
