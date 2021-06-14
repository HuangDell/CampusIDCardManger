#pragma once
#include "Card.h"
using namespace std;

void Card::applyCard(int &id,string &name)//申请校园卡的通用部分
{
	printf("\t\t\t请输入你的姓名：");
	while(cin >> name)
	{
		if (name[0] >= '0' && name[0] <= '9')
			printf("\t\t\t输入有误，请重新输入：");
		else
			break;
	}
	printf("\t\t\t请输入你的学号(8位)：");
	while (!(cin >> id)|| to_string(id).size()!=8)
	{
		printf("\t\t\t请重新输入:");
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

}


bool Card::check()//判断用户是否确认
{
	Again:
	string choose;
	printf("\t\t\t确认请输入Y,取消请输入N\n");
	FOCUS();
	cin >> choose;
	if (choose == "Y" || choose == "y")
		return true;
	else if (choose == "N" || choose == "n")
		return false;
	else
		goto Again;
}

void Card::showRecord(string table, int id)//打印流水信息
{
	auto records = connector->getRecord(table, id);
	drawLine(70, 1);
	printf("\t|流水号     卡号     类型   记录\t\t  日期\t             |\n");
	for (auto record : records)
		printf("\t|%lld %d %s %-18s    %s|\n", record.xID, record.ID, record.Type.c_str(), record.What.c_str(), record.Date.c_str());
	drawLine(70, 1);
}


 bool cmp(Token t1, Token t2)//排序
{
	return t1.money > t2.money;
}
