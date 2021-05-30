#pragma once
#include "Card.h"
class CampusCard :public Card
{
	static Card*instance;
public:
	void showCards();
	void showCards(int id);
	void applyCard();
	void menu();
	void deposit(int id);
	void binding(int id);

	static Card* getInstance() { return instance; }
};
