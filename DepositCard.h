#pragma once
#include "Card.h"
class DepositCard :public Card//DepositCard����
{
	static Card* instance;
public:
	void applyCard();
	void menu();
	void showCards(int id=0);
	void binding(int id);
	void deposit(int id);
	void cost(int id) {};
	void  transfer(int id);//��cost����Ϊtransfer
	static Card* getInstance() { return instance; }
};
