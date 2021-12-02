#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "page.h"

/**
*This function initiates changes associated to the 'up' button. These are type dependent changes, both internal and interface modifications.
*@param Pointer to the page to be affected.
*/
void upButtonAction(Page* page)
{
	page->move(true);
	page->updateArrow();
}

/**
*This function initiates changes associated to the 'down' button. These are type dependent changes, both internal and interface modifications.
*@param Pointer to the page to be affected.
*/
void downButtonAction(Page* page)
{
	page->move(false);
	page->updateArrow();
}

/**
*This function intitiates changes associated to the 'ok' button. These are type dependent changes, both internal and interface modifications.
*@param Pointer to the page to be affected.
*@return Pointer to the page that the 'ok' action leads to.
*/
Page* okButtonAction(Page* page)
{
	page = page->ok();
	page == nullptr ? Screen::clear() : page->loadPage();
	return page;
}

#endif
