#pragma once
#include "Card.h"
using namespace std;

void Card::applyCard(int &id,string &name)
{
	printf("请输入你的姓名：");
	while(cin >> name)
	{
		if (name[0] >= '0' && name[0] <= '9')
			printf("输入有误，请重新输入：");
		else
			break;
	}
	printf("请输入你的学号(8位)：");
	while (!(cin >> id))
	{
		printf("输入有误，请重新输入：");
		cin.clear();
		fflush(stdin);
	}
}

void Card::showCards()
{

}

void Card::deposit(int money)
{

}
void Card::pay(int money)
{

}
void Card::find(int id, std::string table)
{

}

bool Card::check()
{
	Again:
	string choose;
	printf("确认请输入Y,取消请输入N\n");
	cin >> choose;
	if (choose == "Y" || choose == "y")
		return true;
	else if (choose == "N" || choose == "n")
		return false;
	else
		goto Again;
}

void Card::showRecord(string&& table, int id)
{
	if (id)
	{
		
	}

}
