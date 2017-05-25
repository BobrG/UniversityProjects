#include "Ecosys.h"

void Modifier::set_type(std::string _type) { type = _type; }

bool Modifier::connect(Feature* _Feature) {
	if (type.compare(_Feature->get_type())) {
		dependers_f.push_back(_Feature);
		return true;
	}
	else
		return false;
}
bool Modifier::connect(Modifier* _Modifier) {
	dependers_m.push_back(_Modifier);
}


void Modifier::disconnect(Feature* _Feature) {
	dependers_f.erase(std::remove(dependers_f.begin(), dependers_f.end(), _Feature), dependers_f.end());
}
void Modifier::disconnect(Modifier* _Modifier) {
	dependers_m.erase(std::remove(dependers_m.begin(), dependers_m.end(), _Modifier), dependers_m.end());
}

void Modifier::update(Modifier& _Modifier) {
	// change values;
	_Modifier.notify();
}

void Modifier::notify() {
	for (size_t i = 0; i < dependers_f.size(); ++i) {
		dependers_f[i]->update(*this);
	}
	for (size_t i = 0; i < dependers_m.size(); ++i) {
		dependers_m[i]->update(*this);
	}
}

Feature::Feature() {
	// define values;
}

void Feature::set_type(std::string _type) { type = _type; }

void Feature::update(Modifier& _Modifier, void* what = 0) {
	// change value;
}

void Object::Init() {
	// define Object's values, start features, etc;
}

void Object::add_feature(Feature* _Feature) {
	_Features.push_back(_Feature);
}
void Object::remove_feature(Feature* _Feature) {
	_Features.erase(std::remove(_Features.begin(), _Features.end(), _Feature), _Features.end());
}

void Container::Init() {
	// define Container's values, start features, start modifiers  etc;
}

void Container::set_headmaster(Container* _Container) {
	Headmaster = _Container;
}

void Container::add_modifier(Modifier* _Modifier) {
	_Modifiers.push_back(_Modifier);
}
void Container::add_container(Container* _Container) {
	_Container->set_headmaster(this);
	_Containers.push_back(_Container);
	// rising up from certain container to the first container;
	for (size_t j = 0; j < _Container->_Modifiers.size(); ++j) {
		for (Container* tmp = this; tmp != NULL; tmp = tmp->Headmaster) {
			for (size_t i = 0; i < _Modifiers.size(); ++i) {
				tmp->_Modifiers[i]->connect(_Container->_Modifiers[j]);
			}
		}
	}
}
void Container::add_object(Object* _Object) {
	_Objects.push_back(_Object);
}