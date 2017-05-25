#include "Ecosys.h"

void Object::handle_modification() {
	for (size_t i = 0; i < _Features.size(); ++i) {
		_Features[i]->update();
	}
}

void Container::notify() {
	for (size_t i = 0; i < _Objects.size(); ++i) {
		_Objects[i]->handle_modification();
	}
	for (size_t i = 0; i < _Containers.size(); ++i) {
		_Containers[i]->handle_modification();
		_Containers[i]->notify();
	}
}