#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "page.h"

void openAction(Page* page, Screen* screen)
{
	page->loadPage(screen);
	screen->print();
}

void upButtonAction(Page* page, Screen* screen)
{
	page->move(true);
	page->updateArrow(screen);
	screen->print();
}

void downButtonAction(Page* page, Screen* screen)
{
	page->move(false);
	page->updateArrow(screen);
	screen->print();
}

Page* okButtonAction(Page* page, Screen* screen)
{
	page = page->ok();
	openAction(page, screen);
	return page;
}

#endif
