#include "iImage.h"

#include "iStd.h"

iImage::iImage()
{
	array = new iArray(cb);
	method = NULL;
	animation = false;
	_aniDt = 0.017;
	aniDt = 0.0f;
	index = 0;
	position = iPointZero;
	tex = NULL;
}


iImage::~iImage()
{
	delete array;
}


void iImage::cb(void* data)
{
	Texture* tex = (Texture*)data;
	freeImage(tex);
}


void iImage::add(Texture* tex)
{
	array->add(tex);
	tex->retainCount++;
}

void iImage::paint(float dt, iPoint position)
{
	if (animation)
	{
		aniDt += dt;
		if (aniDt >= _aniDt)
		{
			aniDt -= _aniDt;
			index++;
			if (index == array->count)
			{
				animation = false;
				index = 0;
				// 애니메이션 끝
				if (method)
					method(this);
			}
		}
	}
	//Texture*
	tex = (Texture*)array->at(index);
	iPoint p = this->position + position;
	drawImage(tex, p.x, p.y, TOP | LEFT);
}

void iImage::startAnimation(MethodImage cb)
{
	method = cb;
	animation = true;
	aniDt = 0.0f;
	index = 0;
}

iRect iImage::touchRect()
{
	iRect rt;
	rt.origin = position;
	rt.size = iSizeMake(tex->width, tex->height);

	return rt;
}
