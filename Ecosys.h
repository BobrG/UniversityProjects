#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

// So, I understand the structure as follows:
// Our smallest constituent unit is Feature. Features are subscribers or observers. They only react
// on changes which come from other unit - Modifier. Modifier is a Feature which can influence other Features.
// The interaction between Features and Modifiers is defined by Containers and Objects. 
// And here comes the User, who defines the hierarchy of Containers and Objects in particular case. 
// In console the User can choose what each Container stores - what Objects and what Containers. 
// Features of various Objects and Containers are define at the start, however some new Features can be 
// gathered during the work of the programm. 
// In the nutshell the structure can be described in some kind of a tree:
// C1 --stores--> O1 && (C2 --stores--> O2 && O3)
// C1 (ex. room) possess Feature 1 (ex.colour), Feature 2 (ex.comfortable), Modifier 1 (ex.heat 100 degrees), Modifier 2 (ex.entertainment Mr.Robot season 2);
// O1 (ex. human) possess Feature 1 (ex.geek), Feature 2 (ex.colour);
// C2 (ex. bottle) possess Feature 1 (ex. colour), Modifier 1 (ex.open/closed);
// O2 (ex. water) possess Feature 1 (ex.heat 10 degrees), Feature 2 (ex.condition liquid);
// O3 (ex. ice) possess Feature 1 (ex.heat -10 degrees), Feature 2 (ex.condition solid);
// What modifier Open of C2 means for us? It means that when the modifier of Heat will change it will influence on temperature of O2 or O3 
// without any obstacle. And if User will change the modifier Open on Closed then the changes in Heat on higher levels will be 
// monitored by modifier Closed on the lower levels.
// IMPORTANT!!! The pattern Pub/Sub is requiered to prevent Modifiers which have no relation with some Features influence them. 
// (ex.Mr.Robot TV show can influence on Geek feature on Human object, but Feature of Colour stays indiferent to this Modifier)

// Each modficator can attach only feature from the depth but cannot affect features which are higher in the list
// of containers and object!!!
// So that we can create connections of modifiers and features from container so that they do not know about
// features from the head!!!

// modifier inherits methods of feature class because it can be described as a feature with possibility of
// influence. This relations between features and modifiers are important because modifiers can influence on other modifiers.
class Feature;

class Modifier { // class of modifiers which influence on each object or container
public:
	virtual void set_type(std::string _type);
	std::string get_type() { return type; }	

	bool connect(Feature* _Feature);
	bool connect(Modifier* _Modifier);

	void disconnect(Feature* _Feature);
	void disconnect(Modifier* _Modifier);

	virtual void update(Modifier& _Modifier);

	void notify();

	virtual ~Modifier() = 0;
private:
	std::string type;
	std::vector<Feature*> dependers_f;
	std::vector<Modifier*> dependers_m;
};


class Feature { // class of features which define every object or container 
public:
	Feature();

	virtual void set_type(std::string _type);
	std::string get_type() { return type; }

	virtual void update(Modifier& _Modifier, void* what = 0);
	
	virtual virtual ~Feature() = 0;
private:
	std::string type;
	// consider next:
	// Modifier* _Modifier;
	// or maybe we do not need it as we use Modifier as argument in update function;
};

class Object { // object which contain features only
public:
	virtual void Init();

	void add_feature(Feature* _Feature); 

	void remove_feature(Feature* _Feature);

protected:
	std::vector<Feature*> _Features;
};

class Container: public Object { // container inherits ability of holding features from object and adds ability of modifying features;
public:
	void Init() { Headmaster = NULL; }

	void set_name(std::string& tmp) { name = tmp; }
	std::string get_name() { return name; }
	
	void set_headmaster(Container* _Container);

	void add_modifier(Modifier* _Modifier);

	void add_object(Object* _Object); // add and affect with modificator; ???

	void add_container(Container* _Container);
private:
	std::string name;
	std::vector<Container*> _Containers;
	std::vector<Object*> _Objects;
	std::vector<Modifier*> _Modifiers;
	Container* Headmaster;
};
