#pragma once
#include "Card.h"
using namespace std;

void Card::applyCard(int &id,string &name)
{
	printf("���������������");
	while(cin >> name)
	{
		if (name[0] >= '0' && name[0] <= '9')
			printf("�����������������룺");
		else
			break;
	}
	printf("���������ѧ��(8λ)��");
	while (!(cin >> id))
	{
		printf("�����������������룺");
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
	printf("ȷ��������Y,ȡ��������N\n");
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
