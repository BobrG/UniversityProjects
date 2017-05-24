#pragma once
#include <vector>

class Container;

class Object {

};

class Container {
private:
	std::vector<Object*> _Objects;
public:
	Container(); 

	void add_object(Object* _Object); // add and affect with modificator; ???

	void modificate(); // implement modificator;

	void add_container(Container* _Container);
};
