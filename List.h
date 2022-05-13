#pragma once

class Node{
public:
	Node(int item);

	bool contains(int item);
	bool remove(int item);
	void insert(int item);

	int my_item;
	Node* next;
};