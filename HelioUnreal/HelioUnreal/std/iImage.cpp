#include "iImage.h"

iImage::iImage()
{
	array = new iArray(cb);
}


iImage::~iImage()
{
	delete array;
}

void freeimage(Texture* tex)
{
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}

	// real tex 지우기
}

void iImage::cb(void* data)
{
	Texture* tex = (Texture*)data;
	freeimage(tex);
	delete tex;
}

void iImage::add(Texture* tex)
{
	array->add(tex);
	tex->retainCount++;
}

void iImage::paint(float dt)
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
				animation = true;
				index = 0;

			}
		}
	}
	Texture* tex = (Texture*)array->at(index);
	drawImage(tex, 0, 0);

}

void iImage::startAnimation(MethodImage cb)
{
	method = cb;
	animation = true;
	aniDt = 0.0f;
	index = 0;
}
