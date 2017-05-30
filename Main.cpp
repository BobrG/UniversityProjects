#include "Ecosys.h"


void heat(double& tmp) {
	tmp =tmp + 10;
}

void choose_modifier(Modifier** tmp) {
	*tmp = new Modifier("temperature", heat,"temperature", 20);
}


int main() {
	// container - pairs, where each pair would have following struct.
	// <std::string type, std::function<T&> user_func>.
	// use std::find to locate required functions. 
	////test
	std::string str;
	//Container* Room = new Container(str);
	//str = "temp";
	//Container* Bottle = new Container(str);
	//Room->add_container(Bottle);
	/*int value = 10;
	Modifier *tmp = new Modifier(str, str, value);
	auto f1 = std::bind(&heat, std::placeholders::_1, tmp);
	tmp->add_handler(f1);
	tmp->update(tmp,f1);*/
	//Modifier<int> *tmp_1 = new Modifier<int>(str, str, value);
	//Room->add_modifier(tmp);
	//Feature<int> *tmp_2 = new Feature<int>(str, str, f1, value);
	//Room->add_feature(tmp_2);
	std::cout << "Choose container/object" << std::endl;
	std::cout << "1 - Container." << std::endl;
	std::cout << "2 - Object." << std::endl;
	int key;
	std::cin >> key;
	switch (key)
	{
	case 1: {
		Container* tmp_0 = new Container();
		std::cout << "Input Container name: ";
		std::string str_0;
		std::getline(std::cin,str_0);
		tmp_0->set_name(str_0);
		
		//choose_modifier(&tmp);
		
		break; }
	case 2: {

		break; }
	default:
		std::cout << "Wrong choice!" << std::endl;
		break;
	}


}