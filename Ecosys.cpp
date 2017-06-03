#include "Ecosys.h"
// searching for current type;
bool Modifier::find_type(std::string _type) {
	if (std::find(type.begin(), type.end(), _type) != type.end()) {
		return true;
	}
	else
		return false;
}

bool Modifier::connect(Feature* _Feature) {
	if (find_type(_Feature->get_type())) {
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
		if (std::find(type.begin(), type.end(), tmp[i]) != type.end()) {
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

void Modifier::notify() {
	std::cout << "Modifier " << name << " notifies other modifiers and features." << std::endl;
	for (size_t i = 0; i < dependers_f.size(); ++i) {
		dependers_f[i]->update(this);
	}
	for (size_t i = 0; i < dependers_m.size(); ++i) {
		dependers_m[i]->update(this);
	}
}

void Modifier::update(Modifier* _Modifier) {
	std::cout << "Modifier " << name << " changed." << std::endl;
	std::cout << "Old value " << value << std::endl;
	handler(_Modifier->get_effect());
	std::cout << "New value " << value << std::endl;
	notify();
}

void Modifier::update(double _effect) {
	std::cout << "Modifier " << name << " changed." << std::endl;
	std::cout << "Old value " << value << std::endl;
	handler(_effect);
	std::cout << "New value " << value << std::endl;
	notify();
}

Modifier::~Modifier() {
	is_active = false;
}

//template <typename T> 
//void Modifier<T>::update(Modifier* _Modifier) {
//	// change values;
//	handler(value);
//	std::cout << "Modifier " << _Modifier->get_name() << " changed." <<std::endl;
//	_Modifier->notify();
//}

void Feature::update(Modifier* _Modifier){
		std::cout << "Feature " << name << " has changed." << std::endl;
		std::cout << "Old value: " << value << std::endl;
		handler(_Modifier->get_effect());
		std::cout << "New value: " << value << std::endl;
}

Feature::~Feature() {
	is_active = false;
}

Object::Object() {
	// define Object's values, start features, etc;
	_Features.resize(0);
	name = "none";
}
void Object::connect_modifier(Modifier* _Modifier) {
	for (size_t i = 0; i < _Features.size() ; ++i) {
		_Modifier->connect(_Features[i]);
	}
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

Feature* Object::get_feature(size_t i) {
	if (i < _Features.size()) {
		return _Features.at(i);
	}
	else
		return NULL;
	
}

void Container::set_headmaster(Container* _Container) {
	Headmaster = _Container;
}

void Container::add_feature(Feature* _Feature) {
	for (size_t i = 0; i < _Modifiers.size(); ++i) {
		_Modifiers[i]->connect(_Feature);
	}
	_Features.push_back(_Feature);

}

void Container::remove_feature(Feature* _Feature) {
	_Features.erase(std::remove(_Features.begin(), _Features.end(), _Feature), _Features.end());
}
void Container::connect_modifier(Modifier* _Modifier) {
	bool flag = true;
	for (size_t i = 0; i < _Modifiers.size(); ++i) {
		if (_Modifier->connect(_Modifiers[i])) {
			flag = false;
		}
	}
	if (flag) {
		for (size_t i = 0; i < _Containers.size(); ++i) {
			_Containers[i]->connect_modifier(_Modifier);
		}
		for (size_t i = 0; i < _Objects.size(); ++i) {
			_Objects[i]->connect_modifier(_Modifier);
		}
	}
}

void Container::add_modifier(Modifier* _Modifier) {
	for (size_t i = 0; i < _Containers.size(); ++i) {
		_Containers[i]->connect_modifier(_Modifier);
	}
	for (size_t i = 0; i < _Objects.size(); ++i) {
		_Objects[i]->connect_modifier(_Modifier);
	}
	for (size_t i = 0; i < _Features.size(); ++i) {
		_Modifier->connect(_Features[i]);
	}
	_Modifiers.push_back(_Modifier);

}

void Container::add_container(Container* _Container) {
	bool leave = false;
	// set the "head" container in which we add new one - _Container;
	_Container->set_headmaster(this);
	_Containers.push_back(_Container);
	std::cout << "Container " << _Container->get_name() << " was added." << std::endl;
	// rising up from certain container to the first container;
	for (size_t j = 0; j < _Container->_Modifiers.size(); ++j) {
		for (Container* tmp = this; tmp != NULL; tmp = tmp->Headmaster) {
			for (size_t i = 0; i < _Modifiers.size(); ++i) {
				// connecting modifiers of new container to modifiers of upper levels;
				// remembering that the last one modifier (according to the push_back method of vector) is 
				// the master modifier to previous modifiers in vector!
				if (tmp->_Modifiers[i]->connect(_Container->_Modifiers[j])) {
					leave = true;	break;
				}
			}
			if (leave) break;
		}
		leave = false;
	}
	// connecting new feature to the first appeared modifier with similiar type;
	for (size_t j = 0; j < _Container->_Features.size(); ++j) {
		for (Container* tmp = this; tmp != NULL; tmp = tmp->Headmaster) {
			for (size_t i = 0; i < _Modifiers.size(); ++i) {
				if (tmp->_Modifiers[i]->connect(_Container->_Features[j])) {
					leave = true; break;
				}
			}
			if (leave) break;
		}
		leave = false;
	}
}
void Container::add_object(Object* _Object) {
	bool leave = false;
	_Object->set_headmaster(this);
	_Objects.push_back(_Object);
	std::cout << "Object " << _Object->get_name() << " was added." << std::endl;
	Feature* tmp_f;
	// remembering that the last one modifier (according to the push_back method of vector) is 
	// the master modifier to previous modifiers in vector!
	for (size_t i = 0; (tmp_f=_Object->get_feature(i)) != NULL; ++i) {
		for (Container* tmp = this; tmp != NULL; tmp = tmp->Headmaster) {
			for (size_t j = 0; j < _Modifiers.size(); ++j) {
				// connecting to the first modifier with similar type and leaving because first modifier 
				// is already connected to higher modifiers in the tree;
				if (tmp->_Modifiers[j]->connect(tmp_f)) {
					leave = true; break;
				}
			}
			if (leave) break;
		}
		leave = false;
	}
}

void Container::make_affect(const std::string _type, double aff) {
	std::cout << "Affecting modifiers in " << name << " with value " << aff <<std::endl;
	for (size_t i = 0; i < _Modifiers.size(); ++i) {
		if (_Modifiers[i]->find_type(_type)) {
			_Modifiers[i]->update(aff);
		} 
	}

}