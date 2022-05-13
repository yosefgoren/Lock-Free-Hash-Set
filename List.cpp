#include "List.h"

Node::Node(int item)
	:my_item(item), next(nullptr){}

bool Node::contains(int item){
	if(my_item == item)
		return true;
	if(next == nullptr)
		return false;
	return next->contains(item);
}
