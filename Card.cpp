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


bool Card::check()
{
	Again:
	string choose;
	printf("\t\t\t确认请输入Y,取消请输入N\n");
	cin >> choose;
	if (choose == "Y" || choose == "y")
		return true;
	else if (choose == "N" || choose == "n")
		return false;
	else
		goto Again;
}

void Card::showRecord(string table, int id)
{
	auto records = connector->getRecord(table, id);
	printf("流水号\t卡号\t类型\t记录\t\t\t日期\t\n");
	for (auto record : records)
	{
		printf("%lld %d %s %s %s\n", record.xID, record.ID, record.Type.c_str(), record.What.c_str(), record.Date.c_str());
	}
}
