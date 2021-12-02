#ifndef __PAGE_H__
#define __PAGE_H__

#include <vector>
#include <string>

class Page()
{
private:
  std::string title;
  std::vector<std::pair<std::string, short>> opts;
  short arrowPos = 0;
public:
  Page(std::string title) : title(title) {}
  void addOpt(std::string _text, short _value = 0) { opts.push_back(std::pair<std::string, short>(_text, _value)); }
  std::string getOptName(short pos) { return opts[pos].first; }
  short getOptValue(short pos) { return opts[pos].second; }
  void moveArrow(bool upDirection)
  {
    if (upDirection && arrowPos == 0) arrowPos = opts.size() - 1;
    else if (downDirection && arrowPos == opts.size() - 1) arrowPos = 0;
    else else upDirection ? arrowPos-- : arrowPos++;
  }
  void updateArrow()
  {
    for (short line = 1; line <= opts.size(); line++) Screen::writeChar(line, 1, ' ');
    Screen::writeChar(arrowPos + 1, 1, '>');
  }
  void printPage()
  {
    arrowPos = 0;
    Screen::writeLine(0, title);
    for (short line = 1; line <= opts.size(); line++) Screen::writeLine(line, "   " + opts[line].first());
    Screen::writeChar(arrowPos + 1, 1, '>');
  }
}

Page mainMenu = Page("MAIN MENU");
mainMenu.addOpt("Bulb timer");
mainMenu.addOpt("Light trsh");
mainMenu.addOpt("Screen timer");
mainMenu.addOpt("Monitoring");

Page lightTreshold = Page("LIGHT TRSH");
lightTreshold.addOpt("Very low light", 1000);
lightTreshold.addOpt("Low light", 800);
lightTreshold.addOpt("Medium light", 500);
lightTreshold.addOpt("High light", 200);
lightTreshold.addOpt("Very high light", 10);

Page bulbTimer = Page("BULB TIMER");
bulbTimer.addOpt("5 seconds", 5);
bulbTimer.addOpt("10 seconds", 10);
bulbTimer.addOpt("30 seconds", 30);
bulbTimer.addOpt("1 minute", 60);

Page screenTimer = Page("SCREEN TIMER");
screenTimer.addOpt("5 seconds", 5);
screenTimer.addOpt("10 seconds", 10);
screenTimer.addOpt("30 seconds", 30);
screenTimer.addOpt("1 minute", 60);

#endif
