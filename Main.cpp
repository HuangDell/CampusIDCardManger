#pragma once
#include "CampusCard.h"
#include "DepositCard.h"
#include "SQLConnector.h"
using std::cin;
using std::cout;
using std::endl;

SQLConnector* SQLConnector::connector = new  SQLConnector();
Card* CampusCard::instance = new CampusCard();
Card* DepositCard::instance = new DepositCard();


int main(void)
{
	int choose;
	Card* card=NULL;
	while (true)
	{
		system("cls");
		printf("1.����У԰��ҵ��    2.�����ҵ��\n");
		 cin >> choose;
		switch (choose)
		{
		case 1:
			card = CampusCard::getInstance();
			break;
		case 2:
			card = DepositCard::getInstance();
			break;
		}
		card->menu();
		system("pause");
	}
}
