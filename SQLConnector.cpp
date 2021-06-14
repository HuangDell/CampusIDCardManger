#pragma once
#include "SQLConnector.h"
#include "json.hpp"
#include <fstream>
#include <sstream>

using namespace sql;
using nlohmann::json;
using namespace std;


SQLConnector::SQLConnector() //���ӷ�����
{
	ifstream fs ("config.json");//Json�ĵ��洢��������Ϣ
	string s;
	json config;
	fs >> config;
	driver = mysql::get_driver_instance();
	try {
		con = driver->connect(config["ip"].get<string>(), config["user"].get<string>(), config["password"].get<string>());
		con->setSchema("card");
		state = con->createStatement();
		state->execute("set names gb2312");
		cout << "�ɹ����ӷ�����!\n";
	}
	catch (...)
	{
		cout << "���ӷ�����ʧ�ܣ�����ϵ������Ա��\n";//�޷����ӵ����ݿ�
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


int SQLConnector::applyCard(std::string && table,Token &token)//ʹ��Token�����ݿ�����Ϣ
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
	if (mode == "bind")//���ָ��������һ�ǰ󶨿�����
			update = "update " + table + " set BindingID=CONCAT(IFNULL(BindingID,''),'" +token.bindingid + " ') where ID=" + to_string(token.id);
	else if (mode == "money")//���ǽ�����
		update = "update " + table + " set Money=" + to_string(token.money)+" where ID="+to_string(token.id);
	state->executeUpdate(update);

}

vector<Token> SQLConnector::getCard(string && table,int id )//id�ǿ�ѡ��
{
	vector<Token> tokens;//ʹ����Token�������ݿ��ж�ȡ�����ݴ���
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

void SQLConnector::record(string table, int& id, string& info)//��¼��ˮ
{
	string t;
	if (table == "campuscard")
		t = "У԰��";
	else
		t = "���";
	string insert = "insert into turnover (ID,Type,What,Date) values(" + to_string(id) + ",'"+t+"','"+info+"',NOW())";
	state->execute(insert);
}

vector<Record> SQLConnector::getRecord(string table, int id)//�����ݿ��ж�ȡ��Ϣ
{
	vector<Record> records;
	string type = table == "campuscard" ? "'У԰��'" : "'���'";
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
