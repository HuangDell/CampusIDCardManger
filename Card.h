#pragma once
#include "SQLConnector.h"
#include "Token.h"
#include <memory>
#include <iostream>
#include <string>
#include <sstream>

class Card
{
protected:
	SQLConnector* connector = SQLConnector::getInstance();
public:
	virtual void applyCard(int &id,std::string &name);
	virtual void  menu() = 0;
	bool check();
	void showRecord(std::string table, int id = 0);
};

