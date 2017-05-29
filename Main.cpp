#include "Ecosys.h"

void func(int& tmp, RealModifier* kek) {
	tmp =tmp + 10;
	kek->set_active(false);
}

void ftest(int& str, RealModifier* kek) {
	std::cout << str;
}

int main() {
	//test
	std::string str;
	Container* Room = new Container(str);
	str = "temp";
	Container* Bottle = new Container(str);
	Room->add_container(Bottle);
	int value = 10;
	Modifier<int> *tmp = new Modifier<int>(str, str, value);
	auto f1 = std::bind(&func, std::placeholders::_1, tmp);
	tmp->add_handler(f1);
	Modifier<int> *tmp_1 = new Modifier<int>(str, str, value);
	Room->add_modifier(tmp);
	Feature<int> *tmp_2 = new Feature<int>(str, str, f1, value);
	Room->add_feature(tmp_2);
}