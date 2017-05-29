#include "Ecosys.h"

bool RealModifier::connect(RealFeature* _Feature) {
	if (std::find(type.begin(),type.end(),_Feature->get_type()) != type.end()) {
		dependers_f.push_back(_Feature);
		return true;
	}
	else
		return false;
}
bool RealModifier::connect(RealModifier* _Modifier) {
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


void RealModifier::disconnect(RealFeature* _Feature) {
	dependers_f.erase(std::remove(dependers_f.begin(), dependers_f.end(), _Feature), dependers_f.end());
}
void RealModifier::disconnect(RealModifier* _Modifier) {
	dependers_m.erase(std::remove(dependers_m.begin(), dependers_m.end(), _Modifier), dependers_m.end());
}

void RealModifier::notify() {
	for (size_t i = 0; i < dependers_f.size(); ++i) {
		dependers_f[i]->update(this);
	}
	for (size_t i = 0; i < dependers_m.size(); ++i) {
		dependers_m[i]->update(this);
	}
}

RealModifier::~RealModifier() {
	is_active = false;
}

//template <typename T> 
//void Modifier<T>::update(RealModifier* _Modifier) {
//	// change values;
//	handler(value);
//	std::cout << "Modifier " << _Modifier->get_name() << " changed." <<std::endl;
//	_Modifier->notify();
//}

RealFeature::~RealFeature() {
	is_active = false;
}

Object::Object() {
	// define Object's values, start features, etc;
	_Features.resize(0);
	name = "none";
}

void Object::set_headmaster(Container* _Container) {
	Headmaster = _Container;
}

void Object::add_feature(RealFeature* _Feature) {
	_Features.push_back(_Feature);
}
void Object::remove_feature(RealFeature* _Feature) {
	_Features.erase(std::remove(_Features.begin(), _Features.end(), _Feature), _Features.end());
}

RealFeature* Object::get_feature(size_t i) {
	// add exception catcher!
	return _Features.at(i);
	
}

//void Container::Init() {
//	// define Container's values, start features, start modifiers  etc;
//	_Modifiers.resize(0);
//	_Features.resize(0);
//	_Objects.resize(0);
//	_Containers.resize(0);
//}

void Container::set_headmaster(Container* _Container) {
	Headmaster = _Container;
}

void Container::add_feature(RealFeature* _Feature) {
	_Features.push_back(_Feature);
}

void Container::remove_feature(RealFeature* _Feature) {
	_Features.erase(std::remove(_Features.begin(), _Features.end(), _Feature), _Features.end());
}

void Container::add_modifier(RealModifier* _Modifier) {
	_Modifiers.push_back(_Modifier);
}
void Container::add_container(Container* _Container) {
	_Container->set_headmaster(this);
	_Containers.push_back(_Container);
	std::cout << "Container " << _Container->get_name() << " was added." << std::endl;
	// rising up from certain container to the first container;
	for (size_t j = 0; j < _Container->_Modifiers.size(); ++j) {
		for (Container* tmp = this; tmp != NULL; tmp = tmp->Headmaster) {
			for (size_t i = 0; i < _Modifiers.size(); ++i) {
				tmp->_Modifiers[i]->connect(_Container->_Modifiers[j]);
			}
		}
	}
	for (size_t j = 0; j < _Container->_Features.size(); ++j) {
		for (Container* tmp = this; tmp != NULL; tmp = tmp->Headmaster) {
			for (size_t i = 0; i < _Modifiers.size(); ++i) {
				tmp->_Modifiers[i]->connect(_Container->_Features[j]);
			}
		}
	}
}
void Container::add_object(Object* _Object) {
	_Object->set_headmaster(this);
	_Objects.push_back(_Object);
	std::cout << "Object " << _Object->get_name() << "was added." << std::endl;
	RealFeature* tmp_f;
	for (size_t i = 0; (tmp_f = _Object->get_feature(i)) != NULL; ++i) {
		for (Container* tmp = this; tmp != NULL; tmp = tmp->Headmaster) {
			for (size_t j = 0; j < _Modifiers.size(); ++j) {
				tmp->_Modifiers[j]->connect(tmp_f);
			}
		}
	}
}