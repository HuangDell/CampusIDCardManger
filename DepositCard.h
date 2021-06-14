#pragma once
#include "Card.h"
class DepositCard :public Card//DepositCard定义
{
	static Card* instance;
public:
	void applyCard();
	void menu();
	void showCards(int id=0);
	void binding(int id);
	void deposit(int id);
	void cost(int id) {};
	void  transfer(int id);//将cost换名为transfer
	static Card* getInstance() { return instance; }
};
