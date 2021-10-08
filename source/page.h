#ifndef __PAGE_H__
#define __PAGE_H__

#include <string>
#include <vector>
#include "item.h"

class Page {
private:
    std::string title;
public:
    Page(std::string _title) : title(_title) {}
    std::string getTitle() { return title; }
};

#endif
