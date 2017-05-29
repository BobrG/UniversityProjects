#include "Ecosys.h"

void func(int& tmp, RealModifier* kek = 0) {
	tmp =tmp + 10;
	//kek->se_active(false);
}

void ftest(int& str, RealModifier* kek = 0) {
	std::cout << str;
}

int main() {
	std::string str;
	Container* Room = new Container(str);
	str = "temp";
	Container* Bottle = new Container(str);
	Room->add_container(Bottle);
	int value = 10;
	func(value);
	Modifier<int> *tmp = new Modifier<int>(str, str, value);
	auto f1 = std::bind(&func, std::placeholders::_1, tmp);
	tmp->add_handler(f1);
	Modifier<int> *tmp_1 = new Modifier<int>(str, str, value);
	tmp->update(tmp_1);
	Room->add_modifier(tmp);

}