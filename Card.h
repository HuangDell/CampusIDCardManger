#pragma once
#include "SQLConnector.h"
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include "Token.h"

class Card
{
protected:
	SQLConnector* connector = SQLConnector::getInstance();
public:
	virtual void showCards();
	virtual void applyCard(int &id,std::string &name);
	virtual void deposit(int money);
	virtual void pay(int money);
	virtual void find(int id, std::string table);
	virtual void  menu() = 0;
	bool check();
	void showRecord(std::string&& table, int id = 0);
};

