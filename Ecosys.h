#pragma once
#include <vector>
#include <iostream>

// Each modficator can attach only feature from the depth but cannot affect features which are higher in the list
// of containers and object!!!
// So that we can create connections of modifiers and features from container so that they do not know about
// features from the head!!!


// modifier inherits methods of feature class because it can be described as a feature with possibility of
// influence. This relations between features and modifiers are important because modifiers can influence on other modifiers.

class Modifier { // class of modifiers which influence on each object or container
public:
	void connect(Feature* _Feature);

	void disconnect(Feature* _Feature);

	virtual void notify(Feature* _Feature = 0, void* what = 0);
private:
	std::vector<Feature*> dependers;
};


class Feature { // class of features which define every object or container 
public:
	Feature();

	virtual void update(Modifier& _Modifier, void* what = 0);
	
	virtual ~Feature();
};

class Object {
private:

	std::vector<Feature*> _Features;

public:
	Object();

	void add_feature(Feature* _Feature); 

	void remove_feature(Feature* _Feature);

	void handle_modification(); // object gets a signal about changes in container and handle event;
};

class Container {
private:

	std::vector<Object*> _Objects;
	std::vector<Container*> _Containers;
	std::vector<Feature*> _Features;
	std::vector<Modifier*> _Modifiers;

	void notify();
public:
	Container(); 

	void add_object(Object& _Object); // add and affect with modificator; ???

	void add_container(Container& _Container);

};
