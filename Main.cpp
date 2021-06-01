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
	system("mode con: cols=102 lines=34");//设置程序边框的大小
	system("color F0");//设置程序的背景色和前景色
	while (true)
	{
		system("cls");
		printf("\t\t\t\t    智能卡管理系统\n");
		drawLine(LENGTH);
		printf("\t\t\t|1.管理校园卡业务    2.管理储蓄卡业务|\n");
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
