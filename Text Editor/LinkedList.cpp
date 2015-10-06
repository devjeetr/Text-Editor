#include "LinkedList.h"
#include <iostream>
using namespace::std;


LinkedList::LinkedList(void)
{
	head = new Node();
	head->data = 0;
	head->previous = 0;
	
	tail = new Node();
	tail->data= 0;
	head->next = tail;
	tail->previous = head;
	tail->next = 0;
}

LinkedList::~LinkedList(void)
{
	delete head;
	head = 0;

	delete tail;
	tail = 0;
}

void LinkedList::append(int data)
{
	tail->previous->next = new Node();
	tail->previous->next->data = data;
	tail->previous->next->previous = tail->previous;
	tail->previous->next->next = tail;
	tail->previous = tail->previous->next;

	printHeadTail();
}

void LinkedList::printHeadTail()
{
	cout<<"----------------"<<endl;
	cout<<"Head's contents"<<endl;
	cout<<"Previous : "<<head->previous<<endl;
	cout<<"Next : "<<head->next<<endl;
	cout<<"Data : "<<head->data<<endl<<endl;
	cout<<"----------------"<<endl<<endl;
	cout<<"Tail's contents"<<endl;
    cout<<"Previous : "<<tail->previous<<endl;
    cout<<"Next : "<<tail->next<<endl;
    cout<<"Data : "<<tail->data<<endl;
	cout<<"----------------"<<endl;
}

void LinkedList::traverseAndPrint()
{
	Node* currentNode;
	currentNode = head->next;

	while(currentNode != tail)
	{
		cout<<"data : "<<currentNode->data<<endl;
		currentNode = currentNode->next;
	}
}

void LinkedList::deleteElement(int position)
{
	Node* currentNode;
	int i;

	currentNode = head->next;
	i = 0;

	while(i<position)
	{
		currentNode = currentNode->next;
		i++;
	}
	
	currentNode->next = currentNode->next->next;
	currentNode->next->previous = currentNode;

}
