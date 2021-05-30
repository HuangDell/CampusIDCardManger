#include "DepositCard.h"
using namespace std;

void DepositCard::menu()
{
	int choose;
	while (true)
	{
		printf("       �������ϵͳ\n");
		printf("1.��ѯ�����û�  2.�鿴�ض��û�\n");
		printf("3.�����    4.�������˵�\n");
		cin >> choose;
		switch (choose)
		{
		case 1:
			showCards();
			break;
		case 2:
			int id;
			printf("�������ѯ�Ŀ���:");
			cin >> id;
			showCards(id);
			break;
		case 3:
			applyCard();
		case 4:return;
		default:
			break;
		}
	}

}
void DepositCard::showCards(int id)
{
	auto tokens = connector->getCard("depositcard", id);
	printf("ID\t  ����\tѧ��\t���\t�󶨵�У԰��\t��������\n");
	for (auto& token : tokens)
		printf("%d\t%s\t%d\t%d\t%s\t%s\n", token.id, token.name.c_str(), token.studyid,token.money, token.bindingid.c_str(),token.date.c_str());
	if (id)
	{
		int choose;
		printf("��ǰ�˻�֧�����²���:\n");
		printf("1.��У԰��  2.��ֵ��Ƭ\n");
		printf("3.�鿴��ˮ\n");
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
	Again:
	printf("��������Ҫ�󶨵�У԰���ţ�");
	cin >> campus;
	auto tokens = connector->getCard("campuscard",campus);
	if (tokens.empty())
	{
		printf("δ��ѯ��У԰����Ϊ:%d��У԰��\n", campus);
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
	printf("����ѧ����%d������������%s��\n",token.studyid,token.name.c_str());
	if (check())
	{
		int id_card = connector->applyCard("depositcard",token);
		printf("���Ĵ������ɹ��������ǣ�%d\n", id_card);
		printf("�Ƿ�������У԰����\n");
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
	printf("����:%s,�����:%d,���:%d\n", token.name.c_str(), deposit,token.money);
	printf("�������ֵ���:");
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
		printf("��ֵ�ɹ�!\n");
	}
}
