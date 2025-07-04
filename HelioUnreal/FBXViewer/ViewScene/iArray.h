#pragma once

#ifndef NULL
#define NULL 0
#endif // !NULL

struct Node
{
	Node* prev;
	//Node* next;
	void* data;
};

typedef void (*MethodArray)(void* data);

struct iArray
{
	Node* node;// tail, lastNode
	//Node* head;
	int count;
	MethodArray method;

	iArray(MethodArray method = NULL);
	virtual ~iArray();

	void add(void* data);
	void add(int index, void* data);// !!!

	void* at(int index);

	void removeAll();
	void remove(int index);// !!!

	
};