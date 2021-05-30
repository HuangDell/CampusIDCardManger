#include "CampusCard.h"
using namespace std;


void CampusCard::menu()
{
	int choose;
	while (true)
	{
		printf("       У԰������ϵͳ\n");
		printf("1.��ѯ�����û�  2.�鿴�ض��û�\n");
		printf("3.����У԰��    4.�������˵�\n");
		cin >> choose;
		switch (choose)
		{
		case 1:
			showCards();
			break;
		case 2:
			int id;
			printf("�������ѯ��У԰����:");
			cin >> id;
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
	}
}

void CampusCard::showCards()
{
	auto tokens= connector->getCard("campuscard");
	printf("ID\t  ����\tѧ��\tѧԺ\t\t���\t�󶨵Ĵ��\n");
	for (auto& token : tokens)
		printf("%d\t%s\t%d\t%s\t\t%d\t%s\n", token.id, token.name.c_str(), token.studyid, token.depart.c_str(), token.money, token.bindingid.c_str());
}

void CampusCard::showCards(int id)
{
	auto token = connector->getCard("campuscard",id)[0];
	int choose;
	printf("ID\t  ����\tѧ��\tѧԺ\t\t���\t�󶨵Ĵ��\n");
	printf("%d\t%s\t%d\t%s\t\t%d\t%s\n", token.id, token.name.c_str(), token.studyid, token.depart.c_str(), token.money, token.bindingid.c_str());
	while (true)
	{
		printf("��ǰ�˻�֧�����²�����\n");
		printf("1. �󶨴��  2. ��ֵ��Ƭ\n");
		printf("3. �鿴��ˮ\n");
		cin >> choose;
		switch (choose)
		{
		case 1:
			binding(id);
		case 2:
			deposit(id);
		case 3:

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
	printf("����:%s,У԰����:%d,���:%d\n", token.name.c_str(), campus,token.money);
	if (deposit_cards.size())
	{
		int ch_id;
		int money;
		bool isIn = false;
		printf("�󶨵Ĵ����:");
		for (auto deposit_id : deposit_cards)
			printf("%d ", deposit_id);
		printf("\n");
		Again:
		printf("��ѡ��ת�˵Ĵ��:");
		cin >> ch_id;
		for (auto temp : deposit_cards)
			if (temp == ch_id)
			{
				isIn = true;
				break;
			}
		if (!isIn)
		{
			printf("δ��ѯ������Ϊ:%d�Ĵ��\n", ch_id);
			goto Again;
		}
		auto deposit_token = connector->getCard("depositcard",ch_id)[0];
		
		printf("���������:%s,����:%d,���:%d\n", deposit_token.name.c_str(),deposit_token.id,deposit_token.money);
		printf("������ת����:");
		while (cin >> money)
		{
			if (money <= 0)
				printf("����������:");
			else if (deposit_token.money < money)
				printf("����!����������:");
			else
			{
				deposit_token.money -= money;
				token.money += money;
				break;
			}
		}
		if (check())
		{
			connector->updateCard("depositcard",deposit_token, "money");
			connector->updateCard("depositcard",token, "money");
			printf("ת�˳ɹ�!\n");
			string info = "�Ӵ��:" + std::to_string(ch_id) + ",��ת����:" + std::to_string(money);
			connector->record("campuscard", campus, info);
		}
		else {
			goto Again;
		}
	}
}

void CampusCard::binding(int campus)
{
	int deposit;
	Again:
	printf("��������Ҫ�󶨵Ĵ����:");
	cin >> deposit;
	auto tokens = connector->getCard("depositcard",deposit);
	if(tokens.empty())
	{
		printf("δ��ѯ������Ϊ:%d�Ĵ��\n", deposit);
		goto Again;
	}
	else
	{
		if (check())
		{
			connector->updateCard("depositcard",tokens[0], "bind");
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
	printf("�����������ڵ�ѧԺ��");
	cin >> token.depart;
	printf("����ѧ����:%d������������:%s�������ڵ�ѧԺ��%s��\n",token.studyid,token.name.c_str(),token.depart.c_str());
	if (check())
	{
		int id_card = connector->applyCard("campuscard",token);
	 	printf("����У԰������ɹ��������ǣ�%d\n", id_card);
		string info = "��ͨУ԰��";
		connector->record("campuscard", id_card, info);
	}
	else
		goto Again;
}
