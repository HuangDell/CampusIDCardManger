#pragma once
#include <mysql/jdbc.h>
#include <string>
#include <vector>
#include "Token.h"
#include "Record.h"

class SQLConnector
{
private:
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;
	sql::Statement* state;
	SQLConnector();
	~SQLConnector();
	static SQLConnector* connector;


public:
	int applyCard(std::string && table,Token &token);
	void updateCard(std::string table,Token&,std::string);
	void record(std::string table, int& id, std::string &info);
	std::vector<Token> getCard(std::string && table,int id=0);
	static SQLConnector* getInstance() { return connector; }
	std::vector<Record> getRecord(std::string table, int id = 0);
	bool check(string && table,int studyID);
};

