#include "CampusCard.h"
using namespace std;


void CampusCard::menu()
{
	int choose;
	while (true)
	{
		system("cls");
		printf("\t\t\t\t    У԰������ϵͳ\n");
		drawLine(LENGTH);
		printf("\t\t\t|1.��ѯ�����û�        2.�鿴�ض��û�|\n");
		printf("\t\t\t|3.����У԰��          4.�������˵�  |\n");
		drawLine(LENGTH);
		FOCUS();
		cin >> choose;
		switch (choose)
		{
		case 1:
			showCards();
			break;
		case 2:
			int id;
			printf("\t\t\t�������ѯ��У԰����:");
			cin >> id;
			if (connector->getCard("campuscard", id).size() == 0)
				printf("\t\t\tδ��ѯ������Ϊ:%d��У԰����\n", id);
			else
				showCards(id);
			break;
		case 3:
			applyCard();
			break;
		case 4:
			return;
		default:
			break;
		}
		FOCUS();
		system("pause");
	}
}


void CampusCard::showCards(int id)
{
	system("cls");
	printf("\t\t\t\t    У԰������ϵͳ\n");
	int max = 1;
	auto tokens= connector->getCard("campuscard",id);
	for (auto& token : tokens)
		max = token.bindingid.size() / 9 > max ? token.bindingid.size() / 9 : max;
	drawLine(60+max*8,1);
	printf("\t|ID\t   ����\t\tѧ��\t  ѧԺ\t\t  ���\t�󶨵Ĵ��");
	for (int i = 12; i < (max==1?12:9*max); i++)
		printf(" ");
	printf("|\n");
	for (auto& token : tokens)
		printf("\t|%d  %-8s\t%d  %-16s%d\t%-*s|\n", token.id, token.name.c_str(), token.studyid, token.depart.c_str(), token.money,max==1?12:9*max, token.bindingid.c_str());
	drawLine(60+max*8,1);
	if(id)
	{
		int choose;
		printf("\t\t\t��ǰ�˻�֧�����²�����\n");
		drawLine(28);
		printf("\t\t\t|1. �󶨴��  2. ��ֵ��Ƭ|\n");
		printf("\t\t\t|3. �鿴��ˮ    4. ���ز˵�|\n");
		drawLine(28);
		FOCUS();
		cin >> choose;
		switch (choose)
		{
		case 1:
			binding(id);
			break;
		case 2:
			deposit(id);
			break;
		case 3:
			showRecord("campuscard", id);
			break;
		case 4:
			return;
		default:
			break;
		}
	}
}

void CampusCard::deposit(int campus)
{
	auto token = connector->getCard("campuscard",campus)[0];
	std::stringstream ss(token.bindingid);
	std::vector<int> deposit_cards;
	int temp;
	while (ss >> temp)
		deposit_cards.push_back(temp);
	if (deposit_cards.size())
	{
		int ch_id;
		int money;
		bool isIn = false;
		printf("\t\t\t�󶨵Ĵ����:");
		for (auto deposit_id : deposit_cards)
			printf("%d ", deposit_id);
		printf("\n");
	Again:
		printf("\t\t\t��ѡ��ת�˵Ĵ��:");
		cin >> ch_id;
		for (auto temp : deposit_cards)
			if (temp == ch_id)
			{
				isIn = true;
				break;
			}
		if (!isIn)
		{
			printf("\t\t\tδ��ѯ������Ϊ:%d�Ĵ��\n", ch_id);
			goto Again;
		}
		auto deposit_token = connector->getCard("depositcard", ch_id)[0];

		printf("\t\t\t���������:%s,����:%d,���:%d\n", deposit_token.name.c_str(), deposit_token.id, deposit_token.money);
		printf("\t\t\t������ת����:");
		while (cin >> money)
		{
			if (money <= 0)
				printf("\t\t\t����������:");
			else if (deposit_token.money < money)
				printf("\t\t\t����!����������:");
			else
			{
				deposit_token.money -= money;
				token.money += money;
				break;
			}
		}
		if (check())
		{
			connector->updateCard("depositcard", deposit_token, "money");
			connector->updateCard("depositcard", token, "money");
			printf("\t\t\tת�˳ɹ�!\n");
			string info = "�Ӵ��:" + std::to_string(ch_id) + ",��ת����:" + std::to_string(money);
			connector->record("campuscard", campus, info);
		}
		else {
			goto Again;
		}
	}
	else
		printf("\t\t\tû�а󶨵Ĵ��,���Ȱ�!\n");
}

void CampusCard::binding(int campus)
{
	int deposit;
	auto token = connector->getCard("campuscard",campus)[0];
	std::stringstream ss(token.bindingid);
	std::vector<int> deposit_cards;
	int temp;
	while (ss >> temp)
		deposit_cards.push_back(temp);
	if (deposit_cards.size() == 3)
	{
		printf("\t\t\t���󶨵Ĵ���Ѵ���Ŀ����\n");
		return;
	}
	Again:
	printf("\t\t\t��������Ҫ�󶨵Ĵ����:");
	cin >> deposit;
	auto tokens = connector->getCard("depositcard",deposit);
	if(tokens.empty())
	{
		printf("\t\t\tδ��ѯ������Ϊ:%d�Ĵ��\n", deposit);
		goto Again;
	}
	else
	{
		if (check())
		{
			tokens[0].bindingid = to_string(campus);
			connector->updateCard("depositcard",tokens[0], "bind");
			auto campus_token = connector->getCard("campuscard", campus)[0];
			campus_token.bindingid = to_string(deposit);
			connector->updateCard("campuscard",campus_token, "bind");
			string info = "�󶨴��:" + std::to_string(deposit);
			connector->record("campuscard", campus, info);
		}
		else
			goto Again;
	}
}

void CampusCard::applyCard()
{
	Again:
	Token token;
	Card::applyCard(token.studyid, token.name);
	if(connector->check("campuscard",token.studyid))
				return;
	{ 
		printf("\t\t\t��ѧ���Ѿ������У԰��!");
		return;
	}
	printf("\t\t\t�����������ڵ�ѧԺ��");
	cin >> token.depart;
	printf("\t\t\t����ѧ����:%d������������:%s�������ڵ�ѧԺ��%s��\n",token.studyid,token.name.c_str(),token.depart.c_str());
	if (check())
	{
		int id_card = connector->applyCard("campuscard",token);
	 	printf("\t\t\t����У԰������ɹ��������ǣ�%d\n", id_card);
		string info = "��ͨУ԰��";
		connector->record("campuscard", id_card, info);
	}
	else
		goto Again;
}
