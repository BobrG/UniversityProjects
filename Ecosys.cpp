#include "Ecosys.h"

void Modifier::add_type(std::string _type) { type.push_back(_type); }
void Modifier::set_name(std::string _name) { name = _name; }

bool Modifier::connect(Feature* _Feature) {
	if (std::find(type.begin(),type.end(),_Feature->get_type()) != type.end()) {
		dependers_f.push_back(_Feature);
		return true;
	}
	else
		return false;
}
bool Modifier::connect(Modifier* _Modifier) {
	std::vector<std::string> tmp;
	_Modifier->get_type(tmp);
	for (size_t i = 0; i < tmp.size(); ++i) {
		if (std::find(type.begin(), type.end(), tmp) != type.end()) {
			dependers_m.push_back(_Modifier);
			return true;
		}
	}
	return false;
}


void Modifier::disconnect(Feature* _Feature) {
	dependers_f.erase(std::remove(dependers_f.begin(), dependers_f.end(), _Feature), dependers_f.end());
}
void Modifier::disconnect(Modifier* _Modifier) {
	dependers_m.erase(std::remove(dependers_m.begin(), dependers_m.end(), _Modifier), dependers_m.end());
}

void Modifier::update(Modifier* _Modifier, void* what = 0) {
	// change values;
	std::cout << "Modifier " << _Modifier->get_name() << " changed." <<std::endl;
	_Modifier->notify();
}

void Modifier::notify(void* what = 0) {
	for (size_t i = 0; i < dependers_f.size(); ++i) {
		dependers_f[i]->update(this);
	}
	for (size_t i = 0; i < dependers_m.size(); ++i) {
		dependers_m[i]->update(this);
	}
}

void Feature::Init() {
	// define values;
	name = "abstract";
	type = "abstract";
}

void Feature::set_type(std::string _type) { type = _type; }

void Temperature_Feature::Init() {
	name = "temperature";
	type = "temperature";
	degrees_c = 0;
}

void Temperature_Feature::update(Modifier* _Modifier, void* what = 0) {

}

void Object::Init() {
	// define Object's values, start features, etc;
	_Features.resize(0);
}

void Object::set_headmaster(Container* _Container) {
	Headmaster = _Container;
}

void Object::add_feature(Feature* _Feature) {
	_Features.push_back(_Feature);
}
void Object::remove_feature(Feature* _Feature) {
	_Features.erase(std::remove(_Features.begin(), _Features.end(), _Feature), _Features.end());
}

Feature* Object::get_feature(int i) {
	if (i < _Features.size()) {
		return _Features[i];
	}
	else
		return NULL;
}

void Container::Init() {
	// define Container's values, start features, start modifiers  etc;
	_Modifiers.resize(0);
	_Features.resize(0);
	_Objects.resize(0);
	_Containers.resize(0);
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
	_Object->set_headmaster(this);
	_Objects.push_back(_Object);
	Feature* tmp_f;
	for (size_t i = 0; (tmp_f = _Object->get_feature(i)) != NULL; ++i) {
		for (Container* tmp = this; tmp != NULL; tmp = tmp->Headmaster) {
			for (size_t j = 0; j < _Modifiers.size(); ++j) {
				tmp->_Modifiers[j]->connect(tmp_f);
			}
		}
	}
}