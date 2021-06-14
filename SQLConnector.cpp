#pragma once
#include "SQLConnector.h"
#include "json.hpp"
#include <fstream>
#include <sstream>

using namespace sql;
using nlohmann::json;
using namespace std;


SQLConnector::SQLConnector() //链接服务器
{
	ifstream fs ("config.json");//Json文档存储服务器信息
	string s;
	json config;
	fs >> config;
	driver = mysql::get_driver_instance();
	try {
		con = driver->connect(config["ip"].get<string>(), config["user"].get<string>(), config["password"].get<string>());
		con->setSchema("card");
		state = con->createStatement();
		state->execute("set names gb2312");
		cout << "成功连接服务器!\n";
	}
	catch (...)
	{
		cout << "连接服务器失败！请联系管理人员！\n";//无法连接到数据库
		system("pause");
		exit(-1);
	}

}
SQLConnector::~SQLConnector() 
{
	delete driver;
	delete con;
	delete state;
}


int SQLConnector::applyCard(std::string && table,Token &token)//使用Token来传递开户信息
{
	int id_card;
	stringstream ss;
	ss << "insert into " + table<<"(StudyID,Name) values("<<token.studyid<<",'"+token.name+"')";
	state->execute(ss.str());
	string temp = "select ID from " + table + " where StudyID=" + to_string(token.studyid);
	unique_ptr <ResultSet> res(state->executeQuery(temp));
	res->next();
	id_card = res->getInt("ID");
	if (table == "campuscard")
		state->executeUpdate("update campuscard set Department ='" + token.depart + "' where StudyID=" + std::to_string(token.studyid));
	else if (table == "depositcard")
		state->executeUpdate("update depositcard set Date=CURDATE() where StudyID=" + std::to_string(token.studyid));
	return id_card;
}

void SQLConnector::updateCard(string  table,Token&token,string mode)
{
	string update;
	if (mode == "bind")//两种更新情况，一是绑定卡更新
			update = "update " + table + " set BindingID=CONCAT(IFNULL(BindingID,''),'" +token.bindingid + " ') where ID=" + to_string(token.id);
	else if (mode == "money")//二是金额更新
		update = "update " + table + " set Money=" + to_string(token.money)+" where ID="+to_string(token.id);
	state->executeUpdate(update);

}

vector<Token> SQLConnector::getCard(string && table,int id )//id是可选项
{
	vector<Token> tokens;//使用类Token将从数据库中读取的数据传递
	string query = "select * from "+table;
	if (id)
		query += " where ID=" + to_string(id);
	unique_ptr <ResultSet> res(state->executeQuery(query));
	while (res->next())
	{
		Token token;
		token.name = res->getString("Name");
		token.id = res->getInt("ID");
		token.bindingid = res->getString("BindingID");
		token.money = res->getInt("Money");
		token.studyid = res->getInt("StudyID");
		try {
			token.date = res->getString("Date");
		}
		catch (...) {}
		try{
			token.depart = res->getString("Department");
		}catch(...){}
		tokens.push_back(token);
	}
	return tokens;
}

void SQLConnector::record(string table, int& id, string& info)//记录流水
{
	string t;
	if (table == "campuscard")
		t = "校园卡";
	else
		t = "储蓄卡";
	string insert = "insert into turnover (ID,Type,What,Date) values(" + to_string(id) + ",'"+t+"','"+info+"',NOW())";
	state->execute(insert);
}

vector<Record> SQLConnector::getRecord(string table, int id)//从数据库中读取信息
{
	vector<Record> records;
	string type = table == "campuscard" ? "'校园卡'" : "'储蓄卡'";
	string query;
	query = "select * from turnover where Type=" + type;
	if (id)
		query += " and ID=" + to_string(id);
	unique_ptr <ResultSet> res(state->executeQuery(query));
	while (res->next())
	{
		Record record;
		record.xID = res->getInt64("xID");
		record.ID = res->getInt("ID");
		record.Date = res->getString("Date");
		record.Type = res->getString("Type");
		record.What = res->getString("What");
		records.push_back(record);
	}
	return records;
}

bool SQLConnector::check(string &&table,int studyID)
{
	int num=0;
	string query = "select ID from "+table+" where StudyID="+to_string(studyID);
	unique_ptr <ResultSet> res(state->executeQuery(query));
	while (res->next())
		num++;
	return num ==(table == "campuscard" ? 1 : 3);

}
