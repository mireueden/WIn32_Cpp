#include "iArray.h"

iArray::iArray(MethodArray method)
{
	node = NULL;
	count = 0;
	this->method = method;
}

iArray::~iArray()
{
	removeAll();
}

void iArray::add(void* data)
{
	Node* n = new Node;
	n->prev = node;
	n->data = data;

	node = n;
	count++;
}

void iArray::add(int index, void* data)
{
	Node* n = node;
	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
		{
			Node* t = new Node;
			t->prev = n->prev;
			t->data = data;

			n->prev = t;
			count++;
			return;
		}
		n = n->prev;
	}
	// 못 넣었으니까 여기가지 왔겠지?
	add(data);
}


void* iArray::at(int index)
{
	int i = count - 1;
	for (Node* n = node; n; n = n->prev, i--)
	{
		if (i == index)
			return n->data;
	}
	return NULL;
}

void iArray::removeAll()
{
	for (Node* n = node; n; )
	{
		//n = n->prev;
		Node* t = n->prev;
		if (method)
			method(n->data);// delete 
		delete n;
		n = t;
	}
	node = NULL;
	count = 0;
}

void iArray::remove(int index)
{
	Node* prevN = NULL;
	Node* n = node;
	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
		{
			if (prevN)
				prevN->prev = n->prev;
			else// if (prevN == NULL)
				node = n->prev;

			if (method)
				method(n->data);
			delete n;

			count--;
			return;
		}

		prevN = n;
		n = n->prev;
	}
}

