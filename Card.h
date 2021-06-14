#pragma once
#include "SQLConnector.h"
#include "Token.h"
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#define LENGTH 38
#define FOCUS() printf("\t\t\t")
inline void drawLine(int length,int tab=3) //�滭���
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
	virtual void  menu() = 0;//���������menu
	virtual void showCards(int id = 0) = 0;//�����������ʾ��Ϣ
	virtual void deposit(int id)=0;//������亯�����
	virtual void binding(int id)=0;//�����������
	virtual void cost(int id)=0;//�������������
	bool check();
	void showRecord(std::string table, int id = 0);
};

