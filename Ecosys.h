#pragma once
#include <vector>
#include <memory>
#include <string>
#include <functional>
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


class RealModifier {
public:
	virtual void set_name(std::string _name) { name = _name; }
	virtual std::string get_name() { return name; }

	virtual void add_type(std::string _type) { type.push_back(_type); }
	virtual void get_type(std::vector<std::string> _type) { _type = type; }

	bool connect(Feature* _Feature);
	bool connect(RealModifier* _Modifier);

	void disconnect(Feature* _Feature);
	void disconnect(RealModifier* _Modifier);

	virtual void update(RealModifier* _Modifier) {}
	virtual void notify() {}

protected:
	std::string name;
	std::vector<std::string> type;
	std::vector<Feature*> dependers_f;
	std::vector<RealModifier*> dependers_m;
};
template <typename T> 
class Modifier : public RealModifier { // class of modifiers which influence on each object or container
public:
	void Init(std::string _type, std::function<void(T)> _handler, std::string _name) { add_handler(_handler); add_type(_type); set_name(_name); }

	void add_handler(std::function<void(T)> _handler) { handler = _handler; }

	void update(RealModifier* _Modifier);

	void notify();
protected:
	T value;
	std::function<void(T)> handler;
};

class Feature { // class of features which define every object or container 
public:
	Feature();

	void set_type(std::string _type);
	std::string get_type() { return type; }


	virtual void set_value(std::string what) = 0;

	virtual void update(RealModifier* _Modifier, std::string what = 0) = 0;
	
	virtual ~Feature() = 0;
protected:
	std::string name;
	std::string type;
	bool is_active;
};

class Container;

class Object { // object which contain features only
public:
	Object();

	void set_name(std::string _name) { name = _name; }
	std::string get_name() { return name; }

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
	Container(std::string& _name) : 
		name(_name) { Headmaster = NULL; _Features.resize(0); _Modifiers.resize(0); _Containers.resize(0); _Objects.resize(0); }
	
	//void Init() { Headmaster = NULL; }

	void set_name(std::string& tmp) { name = tmp; }
	std::string get_name() { return name; }

	void set_headmaster(Container* _Container);

	void add_feature(Feature* _Feature);

	void remove_feature(Feature* _Feature);

	void add_modifier(RealModifier* _Modifier);

	void add_object(Object* _Object); 

	void add_container(Container* _Container);
private:
	std::string name;
	std::vector<Feature*> _Features;
	std::vector<Container*> _Containers;
	std::vector<Object*> _Objects;
	std::vector<RealModifier*> _Modifiers;
	Container* Headmaster;
};

