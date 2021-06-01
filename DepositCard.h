#pragma once
#include "Card.h"
class DepositCard :public Card
{
	static Card* instance;
public:
	void applyCard();
	void menu();
	void showCards(int id=0);
	void binding(int id);
	void deposit(int id);
	void  transfer(int id);
	static Card* getInstance() { return instance; }
};
