#include "Ecosys.h"

void func(double tmp, RealModifier* kek) {
	tmp += 10;
	kek->set_active(false);
}

int main() {
	std::string str;
	Container* Room = new Container(str);
	str = "temp";
	Container* Bottle = new Container(str);
	Room->add_container(Bottle);
	
	Modifier<int> *tmp = new Modifier<int>(str, str);
	//Temperature->Init("temperature",func,"temperature");
	Room->add_modifier(tmp);

}