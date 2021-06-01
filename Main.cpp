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
	system("mode con: cols=102 lines=34");//���ó���߿�Ĵ�С
	system("color F0");//���ó���ı���ɫ��ǰ��ɫ
	while (true)
	{
		system("cls");
		printf("\t\t\t\t    ���ܿ�����ϵͳ\n");
		drawLine(LENGTH);
		printf("\t\t\t|1.����У԰��ҵ��    2.�����ҵ��|\n");
		drawLine(LENGTH);
		FOCUS();
		 cin >> choose;
		switch (choose)
		{
		case 1:
			card = CampusCard::getInstance();
			break;
		case 2:
			card = DepositCard::getInstance();
			break;
		default:
			//fflush(stdin);
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		card->menu();
		FOCUS();
		system("pause");
	}
}
