#include "CampusCard.h"
using namespace std;
extern bool cmp(Token, Token);

void CampusCard::menu()
{
	int choose;
	while (true)
	{
		system("cls");
		printf("\t\t\t\t    校园卡管理系统\n");
		drawLine(LENGTH);
		printf("\t\t\t|1.查询所有用户        2.查看特定用户|\n");
		printf("\t\t\t|3.办理校园卡          4.返回主菜单  |\n");
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
			printf("\t\t\t请输入查询的校园卡号:");
			cin >> id;
			if (connector->getCard("campuscard", id).size() == 0)
				printf("\t\t\t未查询到卡号为:%d的校园卡。\n", id);
			else
				showCards(id);
			break;
		case 3:
			applyCard();
			break;
		case 4:
			return;
		default:
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');;
			continue;
		}
		FOCUS();
		system("pause");
	}
}


void CampusCard::showCards(int id)
{
	system("cls");
	printf("\t\t\t\t    校园卡管理系统\n");
	int max = 1;
	auto tokens= connector->getCard("campuscard",id);
	for (auto& token : tokens)
		max = token.bindingid.size() / 9 > max ? token.bindingid.size() / 9 : max;
	drawLine(60+max*8,1);
	printf("\t|ID\t   姓名\t\t学号\t  学院\t\t  余额\t绑定的储蓄卡");
	for (int i = 12; i < (max==1?12:9*max); i++)
		printf(" ");
	printf("|\n");
	for (auto& token : tokens)
		printf("\t|%d  %-8s\t%d  %-16s%d\t%-*s|\n", token.id, token.name.c_str(), token.studyid, token.depart.c_str(), token.money,max==1?12:9*max, token.bindingid.c_str());
	drawLine(60+max*8,1);
	if(id)
	{
		int choose;
		printf("\t\t\t当前账户支持以下操作：\n");
		drawLine(28);
		printf("\t\t\t|1. 绑定储蓄卡  2. 充值卡片|\n");
		printf("\t\t\t|3. 查看流水    4. 消费    |\n");
		printf("\t\t\t|5. 返回菜单               |\n");
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
			cost(id);
		case 5:
			return;
		default:
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
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
		printf("\t\t\t绑定的储蓄卡有:");
		for (auto deposit_id : deposit_cards)
			printf("%d ", deposit_id);
		printf("\n");
	Again:
		printf("\t\t\t请选择转账的储蓄卡:");
		while (!(cin >> ch_id))
		{
			printf("\t\t\t请重新输入:");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');;
		}
		for (auto temp : deposit_cards)
			if (temp == ch_id)
			{
				isIn = true;
				break;
			}
		if (!isIn)
		{
			printf("\t\t\t未查询到卡号为:%d的储蓄卡\n", ch_id);
			goto Again;
		}
		auto deposit_token = connector->getCard("depositcard", ch_id)[0];

		printf("\t\t\t储蓄卡所有人:%s,卡号:%d,余额:%d\n", deposit_token.name.c_str(), deposit_token.id, deposit_token.money);
		printf("\t\t\t请输入转入金额:");
		while (cin >> money)
		{
			if (money <= 0)
				printf("\t\t\t请重新输入:");
			else if (deposit_token.money < money)
				printf("\t\t\t余额不足!请重新输入:");
			else
			{
				deposit_token.money -= money;
				token.money += money;
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');;
		}
		if (check())
		{
			connector->updateCard("depositcard", deposit_token, "money");
			connector->updateCard("depositcard", token, "money");
			printf("\t\t\t转账成功!\n");
			string info = "从储蓄卡:" + std::to_string(ch_id) + ",中转入金额:" + std::to_string(money);
			connector->record("campuscard", campus, info);
		}
		else {
			goto Again;
		}
	}
	else
		printf("\t\t\t没有绑定的储蓄卡,请先绑定!\n");
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
		printf("\t\t\t您绑定的储蓄卡已达数目上限\n");
		return;
	}
	Again:
	printf("\t\t\t请输入需要绑定的储蓄卡号:");
	while (!(cin >> deposit))
	{
			printf("\t\t\t请重新输入:");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');;
	}
	auto tokens = connector->getCard("depositcard",deposit);
	if(tokens.empty())
	{
		printf("\t\t\t未查询到卡号为:%d的储蓄卡\n", deposit);
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
			string info = "绑定储蓄卡" + std::to_string(deposit);
			connector->record("campuscard", campus, info);
			printf("\t\t\t绑定成功!\n");
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
	{ 
		printf("\t\t\t该学号已经申请过校园卡!\n");
		return;
	}
	printf("\t\t\t请输入你所在的学院：");
	cin >> token.depart;
	printf("\t\t\t您的学号是:%d，您的姓名是:%s，您所在的学院是%s。\n",token.studyid,token.name.c_str(),token.depart.c_str());
	if (check())
	{
		int id_card = connector->applyCard("campuscard",token);
	 	printf("\t\t\t您的校园卡办理成功，卡号是：%d\n", id_card);
		string info = "开通校园卡";
		connector->record("campuscard", id_card, info);
	}
}

void CampusCard::cost(int id)
{
	int cost;
	auto campus_token = connector->getCard("campuscard", id)[0];
	printf("\t\t\t请输入你想要消费的金额:");
	while (!(cin >> cost))
	{
		if (cost <= 0)
			printf("\t\t\t请重新输入:");
		else
		{
			printf("\t\t\t请重新输入:");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');;
		}
	
	}
	if (campus_token.money < cost && campus_token.bindingid.size() != 0)
	{
		printf("\t\t\t余额不足!已绑定储蓄卡,是否组合支付?\n");
		if (check())
		{
			cost -= campus_token.money;
			if (campus_token.money)
			{
				string info = "消费:" + to_string(campus_token.money) + "元";
				connector->updateCard("depositcard", campus_token, "money");
				connector->record("campuscard", id, info);
			}
			std::stringstream ss(campus_token.bindingid);
			std::vector<int> deposit_cards;
			int temp;
			while (ss >> temp)
				deposit_cards.push_back(temp);
			vector<Token> deposit;
			for (auto& num : deposit_cards)
				deposit.push_back(connector->getCard("depositcard", num)[0]);
			sort(deposit.begin(), deposit.end(), cmp);
			int sum = 0;
			for (auto& token : deposit)
				sum += token.money;
			if (sum < cost)
			{
				printf("\t\t\t组合支付余额不足!\n");
				return;
			}
			if (deposit[0].money < cost)
			{
				string info = "消费:" + to_string(deposit[0].money) + "元";
				deposit[0].money = 0;
				cost -= deposit[0].money;
				connector->record("depositcard", deposit[0].id, info);
			}
			else if (deposit[0].money >= cost)
			{
				string info = "消费:" + to_string(cost) + "元";
				deposit[0].money -= cost;
				cost = 0;
				connector->record("depositcard", deposit[0].id, info);
			}
			if (deposit[1].money < cost)
			{
				string info = "消费:" + to_string(deposit[1].money) + "元";
				deposit[1].money = 0;
				cost -= deposit[1].money;
				connector->record("depositcard", deposit[1].id, info);
			}
			else if (deposit[1].money >= cost && cost != 0)
			{
				string info = "消费:" + to_string(cost) + "元";
				deposit[1].money -= cost;
				cost = 0;
				connector->record("depositcard", deposit[1].id, info);
			}
			if (cost != 0)
			{
				string info = "消费:" + to_string(cost) + "元";
				deposit[2].money -= cost;
				connector->record("depositcard", deposit[2].id, info);
			}
			for (auto& token : deposit)
				connector->updateCard("depositcard", token, "money");
			printf("\t\t\t消费成功\n");
			return;
		}
	}
	if (cost)
	{
		campus_token.money -= cost;
		string info = "消费:" + to_string(cost) + "元";
		connector->updateCard("depositcard", campus_token, "money");
		connector->record("campuscard", id, info);
	}
}


