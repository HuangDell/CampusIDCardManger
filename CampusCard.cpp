#include "CampusCard.h"
using namespace std;


void CampusCard::menu()
{
	int choose;
	while (true)
	{
		printf("       校园卡管理系统\n");
		printf("1.查询所有用户  2.查看特定用户\n");
		printf("3.办理校园卡    4.返回主菜单\n");
		cin >> choose;
		switch (choose)
		{
		case 1:
			showCards();
			break;
		case 2:
			int id;
			printf("请输入查询的校园卡号:");
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
	printf("ID\t  姓名\t学号\t学院\t\t余额\t绑定的储蓄卡\n");
	for (auto& token : tokens)
		printf("%d\t%s\t%d\t%s\t\t%d\t%s\n", token.id, token.name.c_str(), token.studyid, token.depart.c_str(), token.money, token.bindingid.c_str());
}

void CampusCard::showCards(int id)
{
	auto token = connector->getCard("campuscard",id)[0];
	int choose;
	printf("ID\t  姓名\t学号\t学院\t\t余额\t绑定的储蓄卡\n");
	printf("%d\t%s\t%d\t%s\t\t%d\t%s\n", token.id, token.name.c_str(), token.studyid, token.depart.c_str(), token.money, token.bindingid.c_str());
	while (true)
	{
		printf("当前账户支持以下操作：\n");
		printf("1. 绑定储蓄卡  2. 充值卡片\n");
		printf("3. 查看流水\n");
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
	printf("姓名:%s,校园卡号:%d,余额:%d\n", token.name.c_str(), campus,token.money);
	if (deposit_cards.size())
	{
		int ch_id;
		int money;
		bool isIn = false;
		printf("绑定的储蓄卡有:");
		for (auto deposit_id : deposit_cards)
			printf("%d ", deposit_id);
		printf("\n");
		Again:
		printf("请选择转账的储蓄卡:");
		cin >> ch_id;
		for (auto temp : deposit_cards)
			if (temp == ch_id)
			{
				isIn = true;
				break;
			}
		if (!isIn)
		{
			printf("未查询到卡号为:%d的储蓄卡\n", ch_id);
			goto Again;
		}
		auto deposit_token = connector->getCard("depositcard",ch_id)[0];
		
		printf("储蓄卡所有人:%s,卡号:%d,余额:%d\n", deposit_token.name.c_str(),deposit_token.id,deposit_token.money);
		printf("请输入转入金额:");
		while (cin >> money)
		{
			if (money <= 0)
				printf("请重新输入:");
			else if (deposit_token.money < money)
				printf("余额不足!请重新输入:");
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
			printf("转账成功!\n");
			string info = "从储蓄卡:" + std::to_string(ch_id) + ",中转入金额:" + std::to_string(money);
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
	printf("请输入需要绑定的储蓄卡号:");
	cin >> deposit;
	auto tokens = connector->getCard("depositcard",deposit);
	if(tokens.empty())
	{
		printf("未查询到卡号为:%d的储蓄卡\n", deposit);
		goto Again;
	}
	else
	{
		if (check())
		{
			connector->updateCard("depositcard",tokens[0], "bind");
			string info = "绑定储蓄卡:" + std::to_string(deposit);
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
	printf("请输入你所在的学院：");
	cin >> token.depart;
	printf("您的学号是:%d，您的姓名是:%s，您所在的学院是%s。\n",token.studyid,token.name.c_str(),token.depart.c_str());
	if (check())
	{
		int id_card = connector->applyCard("campuscard",token);
	 	printf("您的校园卡办理成功，卡号是：%d\n", id_card);
		string info = "开通校园卡";
		connector->record("campuscard", id_card, info);
	}
	else
		goto Again;
}
