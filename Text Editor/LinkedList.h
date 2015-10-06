#pragma once
#include "Node.h"
class LinkedList
{
public:
	LinkedList(void);
	~LinkedList(void);

	void append(int data);
	void traverseAndPrint();
	void deleteElement(int position);
	void printHeadTail();
private:
	Node* head;
	Node* tail;
};

