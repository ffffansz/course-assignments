#pragma once
#include "DesBlock.h"

using namespace boost;

class DES_CTR
{
public:
	/* ���Ҫ���ܣ��������ģ�����Ϊһ��ָ����ַ�����shared_ptr����֮��Ȼ */
	DES_CTR(const dynamic_bitset<>& key, const dynamic_bitset<>& IV, const shared_ptr<std::string>& pPltxt, const shared_ptr<std::string>& pCitxt);
	DES_CTR(const dynamic_bitset<>& key, const dynamic_bitset<>& IV, const shared_ptr<dynamic_bitset<>>& pPlFileBits, const shared_ptr<dynamic_bitset<>>& pCiFileBits);
	void encry();
	void encry_f();
	void decry();
	void decry_f();
	void setPltxt(shared_ptr<std::string>& pPltxt);
	void setCitxt(shared_ptr<std::string>& pCitxt);
	shared_ptr<std::string> getCitxt();
	shared_ptr<std::string> getPltxt();
	shared_ptr<dynamic_bitset<>> getCiFileBits();
	shared_ptr<dynamic_bitset<>> getPlFileBits();

	~DES_CTR();
private:
	shared_ptr<std::string> pPltxt_;
	shared_ptr<std::string> pCitxt_;
	shared_ptr<dynamic_bitset<>> pPlFileBits_;
	shared_ptr<dynamic_bitset<>> pCiFileBits_;
	shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>> pPlbitsets_;  // bitset��ʽ�����ģ������Ϊ���룬�������䣻�����Ϊ�������ȥ�����
	shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>> pCibitsets_;  // bitset��ʽ�����ģ������Ϊ���룬�������䣻�����Ϊ�������ȥ�����
	bool op_;
	dynamic_bitset<> key_;
	dynamic_bitset<> IV_;
	dynamic_bitset<> counter_;
	void updateCounter_(unsigned i);
	void resetCounter_();
};

