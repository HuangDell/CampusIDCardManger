#include "DepositCard.h"
using namespace std;

void DepositCard::menu()
{
	int choose;
	while (true)
	{
		system("cls");
		printf("\t\t\t\t    储蓄卡管理系统\n");
		drawLine(LENGTH);
		printf("\t\t\t|1.查询所有用户        2.查看特定用户|\n");
		printf("\t\t\t|3.办理储蓄卡          4.返回主菜单  |\n");
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
			printf("\t\t\t请输入查询的卡号:");
			cin >> id;
			if (connector->getCard("depositcard", id).size() == 0)
				printf("\t\t\t未查询到卡号为:%d的储蓄卡。\n", id);
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
	printf("\t\t\t\t    储蓄卡管理系统\n");
	drawLine(67, 1);
	printf("\t|ID\t   姓名\t\t学号\t  余额\t绑定的校园卡\t开卡日期  |\n");
	for (auto& token : tokens)
		printf("\t|%d  %-8s\t%d  %d\t%-9s\t%s|\n", token.id, token.name.c_str(), token.studyid,token.money, token.bindingid.c_str(),token.date.c_str());
	drawLine(67, 1);
	if (id)
	{
		int choose;
		printf("\t\t\t当前账户支持以下操作:\n");
		drawLine(28);
		printf("\t\t\t|1. 绑定校园卡  2. 充值卡片|\n");
		printf("\t\t\t|3. 查看流水    3. 返回菜单|\n");
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
		printf("\t\t\t您已达绑定校园卡数目上限！\n");
		return;
	}
	Again:
	printf("\t\t\t请输入需要绑定的校园卡号：");
	cin >> campus;
	auto tokens = connector->getCard("campuscard",campus);
	if (tokens.empty())
	{
		printf("\t\t\t未查询到校园卡号为:%d的校园卡\n", campus);
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
	printf("\t\t\t您的学号是%d，您的姓名是%s。\n",token.studyid,token.name.c_str());
	if (connector->check("depositcard", token.studyid))
	{
		printf("\t\t\t您已达储蓄卡办理数目上限！\n");
	}
	else
		if (check())
		{
			int id_card = connector->applyCard("depositcard", token);
			printf("\t\t\t您的储蓄卡办理成功，卡号是：%d\n", id_card);
			printf("\t\t\t是否立即绑定校园卡？\n");
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
	printf("\t\t\t姓名:%s,储蓄卡号:%d,余额:%d\n", token.name.c_str(), deposit,token.money);
	printf("\t\t\t请输入充值金额:");
	while (cin >> money)
	{
		if (money <= 0)
			printf("请重新输入:");
		else
		{
			token.money += money;
			break;
		}
	}
	if (check())
	{
		connector->updateCard("depositcard",token, "money");
		printf("\t\t\t充值成功!\n");
	}
}
