#pragma once
class Node
{
public:
	Node(void);
	~Node(void);

public:
	Node* next;
	Node* previous;
	int data;
};

