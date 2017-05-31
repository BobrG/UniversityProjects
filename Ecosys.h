#pragma once
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>

// Modifier adds handler to it's features!

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
// C1 (ex. room) possess Feature 1 (ex.colour), Feature 2 (ex.comfortable), Modifier 1 (ex.Temperature 100 degrees), Modifier 2 (ex.entertainment Mr.Robot season 2);
// O1 (ex. human) possess Feature 1 (ex.geek), Feature 2 (ex.colour);
// C2 (ex. bottle) possess Feature 1 (ex. colour), Modifier 1 (ex.open/closed);
// O2 (ex. water) possess Feature 1 (ex.Temperature 10 degrees), Feature 2 (ex.condition liquid);
// O3 (ex. ice) possess Feature 1 (ex.Temperature -10 degrees), Feature 2 (ex.condition solid);
// What modifier Open of C2 means for us? It means that when the modifier of Temperature will change it will influence on temperature of O2 or O3 
// without any obstacle. And if User will change the modifier Open on Closed then the changes in Temperature on higher levels will be 
// monitored by modifier Closed on the lower levels.
// IMPORTANT!!! The pattern Pub/Sub is requiered to prevent Modifiers which have no relation with some Features influence them. 
// (ex.Mr.Robot TV show can influence on Geek feature on Human object, but Feature of Colour stays indiferent to this Modifier)

// Each modficator can attach only feature from the depth but cannot affect features which are higher in the list
// of containers and object!!!
// So that we can create connections of modifiers and features from container so that they do not know about
// features from the head!!!

// modifier inherits methods of feature class because it can be described as a feature with possibility of
// influence. This relations between features and modifiers are important because modifiers can influence on other modifiers.

// ADD DERIVED CLASSES!!!

class Feature;


class Modifier {
public:
	Modifier(std::string _type, std::string _name, double _value) { set_name(_name); add_type(_type); is_active = true; value = _value; }

//	Modifier(std::string _type, std::string _name, double _value) { add_type(_type); set_name(_name); set_value(_value); is_active = true; }

	void set_name(std::string _name) { name = _name; }
	std::string get_name() { return name; }

	void set_value(double _value) { value = _value; }
	double get_value() { return value; }
	void set_effect(double _value) { effect = _value; }
	double get_effect() { return effect; }

	void set_active(bool _active) { is_active = _active; }

	void add_type(std::string _type) { type.push_back(_type); }
	void get_type(std::vector<std::string>& _type) { _type = type; }
	bool find_type(std::string _type);

	bool connect(Feature* _Feature);
	bool connect(Modifier* _Modifier);

	void disconnect(Feature* _Feature);
	void disconnect(Modifier* _Modifier);
	
	void define_handler(std::function<void(double)> _handler) { handler = _handler; }
	
	void update(Modifier* _Modifier) {
		std::cout << "Modifier " << name << " changed." << std::endl;
		std::cout << "Old value " << value << std::endl;
		handler(_Modifier->get_effect());
		std::cout << "New value " << value << std::endl;
		notify();
	}
	void update(double _effect) {
		handler(_effect);
		notify();
	}

	void notify();

	virtual ~Modifier();
protected:
	double value;
	double effect;
	std::function<void(double)> handler;
	bool is_active;
	std::string name;
	std::vector<std::string> type;
	std::vector<Feature*> dependers_f;
	std::vector<Modifier*> dependers_m;
};

class Feature { // class of features which define every object or container 
public:
	Feature() { is_active = true; }
	Feature(std::string _name, std::string _type,double _value) { set_name(_name); set_type(_type); set_value(_value); is_active = true; }

	void define_handler(std::function<void(double)> _handler) { handler = _handler; }

	void set_value(double _value) { value = _value; }
	double get_value() { return value; }

	void set_name(std::string _name) { name = _name; }
	std::string get_name() { return name; }

	void set_type(std::string _type) { type = _type; }
	std::string get_type() { return type; }
	
	void update(Modifier* _Modifier) {
		std::cout << "Feature " << name << " has changed." << std::endl;
		std::cout << "Old value: " << value << std::endl;
		handler(_Modifier->get_effect());
		std::cout << "New value: " << value << std::endl;
	}
	
	~Feature();
protected:
	std::function<void(double)> handler;
	double value;
	std::string name;
	std::string type;
	bool is_active;
};


// container and object 
class Container;

class Object { // object which contain features only
public:
	Object();
	Object(std::string _name) : name(_name) {}

	void set_name(std::string _name) { name = _name; }
	std::string get_name() { return name; }

	void connect_modifier(Modifier* _Modifier);

	void add_feature(Feature* _Feature); 

	void remove_feature(Feature* _Feature);

	void set_headmaster(Container* _Container);

	Feature* get_feature(size_t i);

private:
	Container* Headmaster;
	std::string name;
	std::vector<Feature*> _Features;
};

class Container { // container inherits ability of holding features from object and adds ability of modifying features;
public:
	Container() { Headmaster = NULL; _Features.resize(0); _Modifiers.resize(0); _Containers.resize(0); _Objects.resize(0); }
	Container(std::string& _name) : 
		name(_name) { Headmaster = NULL; _Features.resize(0); _Modifiers.resize(0); _Containers.resize(0); _Objects.resize(0); }
	
	void set_name(std::string& tmp) { name = tmp; }
	std::string get_name() { return name; }

	void set_headmaster(Container* _Container);

	void add_feature(Feature* _Feature);

	void remove_feature(Feature* _Feature);

	void connect_modifier(Modifier* _Modifier);

	void add_modifier(Modifier* _Modifier);

	void add_object(Object* _Object); 

	void add_container(Container* _Container);

	void make_affect(const std::string _type, double aff);
private:
	std::string name;
	std::vector<Feature*> _Features;
	std::vector<Container*> _Containers;
	std::vector<Object*> _Objects;
	std::vector<Modifier*> _Modifiers;
	Container* Headmaster;
};

