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

void Card::showRecord(string table, int id)
{
	auto records = connector->getRecord(table, id);
	printf("��ˮ��\t����\t����\t��¼\t\t\t����\t\n");
	for (auto record : records)
	{
		printf("%lld %d %s %s %s\n", record.xID, record.ID, record.Type.c_str(), record.What.c_str(), record.Date.c_str());
	}
}
