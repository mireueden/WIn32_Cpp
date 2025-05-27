#include "iPopup.h"

#include "iStd.h"

iPopup::iPopup()
{

	array = new iArray(cbArray);
}

iPopup::~iPopup()
{
}

void iPopup::cbArray(void* data)
{
	iImage* img = (iImage*)data;
	delete img;
}

void iPopup::add(iImage img)
{

}

void iPopup::paint(float dt, iPoint position)
{
	for (int i = 0; i < array->count; i++)
	{
		iImage* img = (iImage*)array->at(i);
		img->paint(dt, position);
	}
}

void iPopup::show(bool show)
{
}
