#include "iSort.h"

#include <stdio.h>

iSort::iSort(int max)
{
	_s = new Sort[max];
	s = new Sort*[max];
	for (int i = 0; i < max; i++)
		s[i] = &_s[i];

	num = 0;
	this->max = max;
}

iSort::~iSort()
{
	delete _s;
	delete s;
}

void iSort::init(int max)
{
	if (this->max < max)
	{
		delete _s;
		delete s;

		_s = new Sort[max];
		s = new Sort * [max];
		for (int i = 0; i < max; i++)
			s[i] = &_s[i];

		this->max = max;
	}
	num = 0;
}

void iSort::add(float y)
{
	Sort* t = s[num];
	t->index = num;
	t->y = y;
	num++;
}

void iSort::update(MethodSort method)
{
	Sort* t;

	int n = num - 1;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (method(s[i]->y, s[j]->y))
			{
				t = s[i];
				s[i] = s[j];
				s[j] = t;
			}
		}
	}
}






#include "stdio.h"

//static iSort* sort = NULL;
//void SortInit(int max)
//{
//	if (sort == NULL)
//		sort = new iSort(max);
//	sort->init(max);
//}
//
//void SortAdd(float y) { sort->add(y); }
//void SortUpdate(MethodSort method) { sort->update(method); }
//int SortGet(int i) { return sort->get(i); }
//
//static iSort* sort = NULL;
//void xSort::init(int max)
//{
//	if (sort == NULL)
//		sort = new iSort(max);
//	sort->init(max);
//}
//
//void xSort::add(float y) { sort->add(y); }
//void xSort::update(MethodSort method) { sort->update(method); }
//int xSort::get(int i) { return sort->get(i); }