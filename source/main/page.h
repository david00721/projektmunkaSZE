#ifndef __PAGE_H__
#define __PAGE_H__

#include <vector>
#include <string>

class Page
{
protected:
  std::string title;
public:
  Page(std::string title) : title(title) {}
  virtual short getArrowPos() {}
  virtual void setArrowPos(short) {}
  virtual void moveArrow(bool) {}
  virtual void printPage() {}
  virtual short getOptValue(short pos) {}
  virtual void refreshPage() {}
};

class MenuPage : public Page
{
private:
  std::vector<std::pair<std::string, short>> opts;
  short arrowPos = 0;
  void updateArrow()
  {
    for (short line = 1; line <= opts.size(); line++) Screen::writeChar(line, 1, ' ');
    Screen::writeChar(arrowPos + 1, 1, '>');
  }
public:
  MenuPage(std::string title) : Page(title) {}
  void addOpt(std::string _text, short _value = 0) { opts.push_back(std::pair<std::string, short>(_text, _value)); }
  short getOptValue(short pos) { return opts[pos].second; }
  short getArrowPos() { return arrowPos; }
  void setArrowPos(short _arrowPos) { arrowPos = _arrowPos; }
  void moveArrow(bool upDirection)
  {
    if (upDirection && arrowPos == 0) arrowPos = opts.size() - 1;
    else if (!upDirection && arrowPos == opts.size() - 1) arrowPos = 0;
    else upDirection ? arrowPos-- : arrowPos++;
    updateArrow();
  }
  void printPage()
  {
    Screen::writeLine(0, title);
    for (short line = 1; line <= opts.size(); line++) Screen::writeLine(line, "   " + opts[line - 1].first);
    Screen::writeChar(arrowPos + 1, 1, '>');
  }
};

class MonitoringPage : public Page //singleton
{
private:
  MonitoringPage(std::string title) : Page(title) {}
  bool* motionStatePointer;
  unsigned long* lastTriggerTimePointer;
  short* lightValuePointer;
protected:
  static MonitoringPage* instance;
public:
  static MonitoringPage getInstance()
  {
    if(!instance) instance = new MonitoringPage("MONITORING PAGE");
    return *instance;
  }
  void setMotionStatePointer(bool* _motionStatePointer) { motionStatePointer = _motionStatePointer; }
  void setLastTriggerTimePointer(unsigned long* _lastTriggerTimePointer) { lastTriggerTimePointer = _lastTriggerTimePointer; }
  void setLightValuePointer(short* _lightValuePointer) { lightValuePointer = _lightValuePointer; }
  void printPage()
  {
    Screen::clear();
    Screen::writeLine(0, title);
    Screen::writeLine(1, " Motion:");
    *motionStatePointer ? Screen::writeLine(2, "  YES") : Screen::writeLine(2, "  NO");
    Screen::writeLine(3, " Timer:");
    Screen::writeLine(4, "  " + std::to_string((unsigned long)(millis() - *lastTriggerTime) / 1000) + " sec");
    Screen::writeLine(5, " Light:");
    Screen::writeLine(6, "  " + std::to_string(*lightValuePointer));
    Screen::writeLine(HEIGHT - 1, std::string(WIDTH - std::string("> Back").length(), ' ') + "> Back");
  }
  void refreshPage()
  {
    *motionStatePointer ? Screen::writeLine(2, "  YES") : Screen::writeLine(2, "  NO");
    Screen::writeLine(4, "  " + std::to_string(*lastTriggerTimePointer) + " sec");
    Screen::writeLine(6, "  " + std::to_string(*lightValuePointer));
  }
};
MonitoringPage* MonitoringPage::instance = 0;

#endif
