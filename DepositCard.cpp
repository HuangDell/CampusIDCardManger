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
			break;
		case 4:return;
		default:
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
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
		printf("\t|%d  %-8s\t%d  %d\t%-9s\t%s|\n", token.id, token.name.c_str(), token.studyid, token.money, token.bindingid.c_str(), token.date.c_str());
	drawLine(67, 1);
	int choose;
	if (id)
	{
		printf("\t\t\t当前账户支持以下操作:\n");
		drawLine(28);
		printf("\t\t\t|1. 绑定校园卡  2. 充值卡片|\n");
		printf("\t\t\t|3. 查看流水    4. 转账    |\n");
		printf("\t\t\t|5. 返回菜单               |\n");
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
		case 4:
			transfer(id);
			break;
		case 5:
			return;
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
	while (!(cin >> campus))
	{
		printf("\t\t\t请重新输入:");
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
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
		
}

void DepositCard::deposit(int deposit)
{
	int money;
	auto token = connector->getCard("depositcard",deposit)[0];
	printf("\t\t\t姓名:%s,储蓄卡号:%d,余额:%d\n", token.name.c_str(), deposit,token.money);
	printf("\t\t\t请输入充值金额:");
	while (!(cin >> money))
	{
		if (money <= 0)
			printf("\t\t\t请重新输入:");
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	}
	token.money += money;
	if (check())
	{
		connector->updateCard("depositcard",token, "money");
		printf("\t\t\t充值成功!\n");
	}
}


void DepositCard::transfer(int id)
{
	auto self = connector->getCard("depositcard", id)[0];
	int to_id;
	Token token;
	string type;
	printf("\t\t\t请输入转入卡号(校园卡或者储蓄卡):");
	while (!(cin >> to_id))
	{
		printf("\t\t\t请重新输入:");
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	auto campus=connector->getCard("campuscard", to_id);
	auto deposit = connector->getCard("depositcard", to_id);
	if (campus.size() == 0 && deposit.size() == 0)
	{
		printf("\t\t\t查找不到卡号为%d的校园卡或者储蓄卡\n", to_id);
		return;
	}
	else if (campus.size() == 0)
	{
		token = deposit[0];
		type = "depositcard";
	}
	else
	{
		token = campus[0];
		type = "campuscard";
	}
	int money;
	printf("\t\t\t请输入转入金额:");
	while (cin >> money)
	{
		if (money <= 0 || money>self.money)
			printf("\t\t\t请重新输入:");
		else
		{
			token.money += money;
			self.money -= money;
			break;
		}
	}
	if (check())
	{
		connector->updateCard(type,token, "money");
		connector->updateCard("depositcard", self, "money");
		printf("\t\t\t充值成功!\n");
		string info = "向" + to_string(to_id) + "转出" + to_string(money) + "元";
		string info_to ="从"+ to_string(id) + "转入" + to_string(money) + "元";
		connector->record("depositcard", id, info);
		connector->record(type, to_id, info_to);
	}
}
