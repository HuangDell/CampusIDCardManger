#include "DepositCard.h"
using namespace std;

void DepositCard::menu()
{
	int choose;
	while (true)
	{
		system("cls");
		printf("\t\t\t\t    �������ϵͳ\n");
		drawLine(LENGTH);
		printf("\t\t\t|1.��ѯ�����û�        2.�鿴�ض��û�|\n");
		printf("\t\t\t|3.�����          4.�������˵�  |\n");
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
			printf("\t\t\t�������ѯ�Ŀ���:");
			cin >> id;
			if (connector->getCard("depositcard", id).size() == 0)
				printf("\t\t\tδ��ѯ������Ϊ:%d�Ĵ����\n", id);
			else
				showCards(id);
			break;
		case 3:
			applyCard();
		case 4:return;
		default:
			break;
		}
		FOCUS();
		system("pause");
	}

}
void DepositCard::showCards(int id)
{
	auto tokens = connector->getCard("depositcard", id);
	system("cls");
	printf("\t\t\t\t    �������ϵͳ\n");
	drawLine(67, 1);
	printf("\t|ID\t   ����\t\tѧ��\t  ���\t�󶨵�У԰��\t��������  |\n");
	for (auto& token : tokens)
		printf("\t|%d  %-8s\t%d  %d\t%-9s\t%s|\n", token.id, token.name.c_str(), token.studyid,token.money, token.bindingid.c_str(),token.date.c_str());
	drawLine(67, 1);
	if (id)
	{
		int choose;
		printf("\t\t\t��ǰ�˻�֧�����²���:\n");
		drawLine(28);
		printf("\t\t\t|1. ��У԰��  2. ��ֵ��Ƭ|\n");
		printf("\t\t\t|3. �鿴��ˮ    3. ���ز˵�|\n");
		drawLine(28);
		FOCUS();
		cin >> choose;
		switch (choose)
		{
		case 1:binding(id);
			break;
		case 2:deposit(id);
			break;
		case 3:
			showRecord("depositcard", id);
			break;

		}
	}
}

void DepositCard::binding(int deposit)
{
	int campus;
	if (connector->getCard("depositcard", deposit)[0].bindingid.size())
	{
		printf("\t\t\t���Ѵ��У԰����Ŀ���ޣ�\n");
		return;
	}
	Again:
	printf("\t\t\t��������Ҫ�󶨵�У԰���ţ�");
	cin >> campus;
	auto tokens = connector->getCard("campuscard",campus);
	if (tokens.empty())
	{
		printf("\t\t\tδ��ѯ��У԰����Ϊ:%d��У԰��\n", campus);
		goto Again;
	}
	else
	{
		auto token = tokens[0];
		token.bindingid += to_string(deposit);

		if (check())
		{
			connector->updateCard("campuscard",token, "bind");
			auto deposit_token = connector->getCard("depositcard",deposit)[0];
			deposit_token.bindingid = to_string(campus);
			connector->updateCard("depositcard",deposit_token,"bind");
		}
		else
			goto Again;
	}
}

void DepositCard::applyCard()
{
	Again:
	Token token;
	Card::applyCard(token.studyid,token.name);
	printf("\t\t\t����ѧ����%d������������%s��\n",token.studyid,token.name.c_str());
	if (connector->check("depositcard", token.studyid))
	{
		printf("\t\t\t���Ѵﴢ�������Ŀ���ޣ�\n");
	}
	else
		if (check())
		{
			int id_card = connector->applyCard("depositcard", token);
			printf("\t\t\t���Ĵ������ɹ��������ǣ�%d\n", id_card);
			printf("\t\t\t�Ƿ�������У԰����\n");
			if (check())
			{
				binding(id_card);
			}
		}
		else
			goto Again;
}

void DepositCard::deposit(int deposit)
{
	int money;
	auto token = connector->getCard("depositcard",deposit)[0];
	printf("\t\t\t����:%s,�����:%d,���:%d\n", token.name.c_str(), deposit,token.money);
	printf("\t\t\t�������ֵ���:");
	while (cin >> money)
	{
		if (money <= 0)
			printf("����������:");
		else
		{
			token.money += money;
			break;
		}
	}
	if (check())
	{
		connector->updateCard("depositcard",token, "money");
		printf("\t\t\t��ֵ�ɹ�!\n");
	}
}
