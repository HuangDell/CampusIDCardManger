#pragma once
#include "Card.h"
using namespace std;

void Card::applyCard(int &id,string &name)//����У԰����ͨ�ò���
{
	printf("\t\t\t���������������");
	while(cin >> name)
	{
		if (name[0] >= '0' && name[0] <= '9')
			printf("\t\t\t�����������������룺");
		else
			break;
	}
	printf("\t\t\t���������ѧ��(8λ)��");
	while (!(cin >> id)|| to_string(id).size()!=8)
	{
		printf("\t\t\t����������:");
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

}


bool Card::check()//�ж��û��Ƿ�ȷ��
{
	Again:
	string choose;
	printf("\t\t\tȷ��������Y,ȡ��������N\n");
	FOCUS();
	cin >> choose;
	if (choose == "Y" || choose == "y")
		return true;
	else if (choose == "N" || choose == "n")
		return false;
	else
		goto Again;
}

void Card::showRecord(string table, int id)//��ӡ��ˮ��Ϣ
{
	auto records = connector->getRecord(table, id);
	drawLine(70, 1);
	printf("\t|��ˮ��     ����     ����   ��¼\t\t  ����\t             |\n");
	for (auto record : records)
		printf("\t|%lld %d %s %-18s    %s|\n", record.xID, record.ID, record.Type.c_str(), record.What.c_str(), record.Date.c_str());
	drawLine(70, 1);
}


 bool cmp(Token t1, Token t2)//����
{
	return t1.money > t2.money;
}
