#pragma once
#include <string>
struct Token {//用于类与类直接的信息传递
	std::string name;
	std::string bindingid;
	std::string depart;
	std::string date;
	int money;
	int studyid;
	int id;
	Token():money(0),studyid(0),id(0){}
};

