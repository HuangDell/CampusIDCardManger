#pragma once
#include "SQLConnector.h"
#include "Token.h"
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#define LENGTH 38
#define FOCUS() printf("\t\t\t")
inline void drawLine(int length,int tab=3) //»æ»­±í¸ñ
{
	for(int i=0;i<tab;i++)
	printf("\t");
	for (int i = 0; i < length; i++)
		printf("-");
	puts("");
}
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

