#pragma once
#include "DesBlock.h"

using namespace boost;

class DES_ECB
{
public:
	/* ���Ҫ���ܣ��������ģ�����Ϊһ��ָ����ַ�����shared_ptr����֮��Ȼ */
	DES_ECB(const dynamic_bitset<>& key, const shared_ptr<std::string>& pPltxt, const shared_ptr<std::string>& pCitxt);
	void encry();
	void decry();
	void setPltxt(shared_ptr<std::string>& pPltxt);
	void setCitxt(shared_ptr<std::string>& pCitxt);
	shared_ptr<std::string> getCitxt();
	shared_ptr<std::string> getPltxt();
	
	~DES_ECB();
private:
	shared_ptr<std::string> pPltxt_;
	shared_ptr<std::string> pCitxt_;
	shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>> pPlbitsets_;  // bitset��ʽ�����ģ������Ϊ���룬�������䣻�����Ϊ�������ȥ�����
	shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>> pCibitsets_;  // bitset��ʽ�����ģ������Ϊ���룬�������䣻�����Ϊ�������ȥ�����
	bool op_;
	dynamic_bitset<> key_;
};

