#include "Ecosys.h"
                                                                                                                                                                                                                                                                                                              //BG
// functions which define modifier's and feature's behaviour
void temp_m(double eff, Modifier* kek) {
	kek->set_value(kek->get_value() + eff);
	kek->set_effect(eff);
}
void temp_f(double eff, Feature* kek) {
	kek->set_value(kek->get_value() + eff);
}
void cap_m(double eff, Modifier* kek) {
	kek->set_value(kek->get_value() + eff/2);
	kek->set_effect(kek->get_value()/2);
}


int main() {
	// creating container Room
	std::string str = "Room";
	Container* tmp = new Container(str);
	// creating container Bottle
	str = "Bottle";
	Container* tmp_1 = new Container(str);
	// creating container Table
	str = "Table";
	Container* tmp_2 = new Container(str);
	// creating object Milk
	str = "Milk";
	Object* tmp_3 = new Object(str);
	str = "Temperature";
	Feature* feat = new Feature(str, str, 30);
	// using std::bind to wrap behaviour functions
	auto f1 = std::bind(&temp_f, std::placeholders::_1, feat);
	feat->define_handler(f1);
	tmp_3->add_feature(feat);
	//Modifier* modif_1 = new Modifier(str, "Table temperature", 10.0);
	//auto f2 = std::bind(&temp_m, std::placeholders::_1, modif_1);
	//modif_1->define_handler(f2);
	Modifier* modif_2 = new Modifier(str, "Cap", 5);
	auto f3 = std::bind(&cap_m, std::placeholders::_1, modif_2);
	modif_2->define_handler(f3);
	//tmp_2->add_modifier(modif_1);
	tmp_1->add_modifier(modif_2);
	tmp_2->add_container(tmp_1);
	tmp_1->add_object(tmp_3);
	tmp->add_container(tmp_2);
	Modifier* modif_3 = new Modifier(str, "Room temperature", 20);
	auto f4 = std::bind(&temp_m, std::placeholders::_1, modif_3);
	modif_3->define_handler(f4);
	tmp->add_modifier(modif_3);
	// affecting temperature
	str = "Temperature";
	int affect = 100; // degrees;
	tmp->make_affect(str, affect);
																																																																													
}