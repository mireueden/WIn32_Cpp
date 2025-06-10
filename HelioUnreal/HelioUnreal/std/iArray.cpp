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
	// n->next = node;
	n->data = data;

	node = n;
	count++;
}

// 원하는 위치에 추가
void iArray::add(int index, void* data)
{
	Node* n = node;
	for (int i = count - 1; i > -1; i--)
	{
		// n
		if (i == index)
		{
			// n을 가리키는 이전 node
			Node* t = new Node;
			t->prev = n->prev;
			t->data = data;

			n->prev = t;
			count++;
			
			return;
		}
		n = n->prev;
	}


#if 0
	// next
	Node* n = node;
	for (int i = count - 1; i < count - 1; i++)
	{
		if (i == index)
		{
			Node* t = new Node;
			t->next = n->next;
			t->data = data;
			n->next = t;

			count++;
			return;
		}
		n = n->next;
	}
#endif
	// 못 넣어서 여기까지 도달
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
	for (Node* n = node;n!=NULL;)
	{
		// n = n->prev
		Node* t = n->prev;
		if (method)
			method(n->data);
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
			else // if(prevN == NULL)
				node = n->prev;

			if (method)
			{
				method(n->data);
			}
			delete n;
			return;
		}
		prevN = n;
		n = n->prev;
	}
}

// Ver.Next Node
//void iArray::remove(int index)
//{
//	Node* n = node;
//	if (index == 0)
//	{
//		node = node->next;
//		if (method)
//			method(n->data);
//		delete n;
//		count--;
//		return;
//	}
//
//	Node* prev = n;
//	n = node->next; 
//	for (int i = 1; i < count; i++)
//	{
//		if (i == index)
//		{
//			prev->next = n->next;
//
//			if (method)
//				method(n->data);
//			delete n;
//			count--;
//			return;
//		}
//		prev = n;
//		n = node->next;
//	}
//}
