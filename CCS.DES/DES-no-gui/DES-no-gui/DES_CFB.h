#pragma once
#include "DesBlock.h"

using namespace boost;

class DES_CFB
{
public:
	/* 如果要加密，则传入明文，密文为一个指向空字符串的shared_ptr；反之亦然 */
	DES_CFB(const dynamic_bitset<>& key, const dynamic_bitset<>& IV, const shared_ptr<std::string>& pPltxt, const shared_ptr<std::string>& pCitxt);
	void encry();
	void decry();
	void setPltxt(shared_ptr<std::string>& pPltxt);
	void setCitxt(shared_ptr<std::string>& pCitxt);
	shared_ptr<std::string> getCitxt();
	shared_ptr<std::string> getPltxt();

	~DES_CFB();
private:
	shared_ptr<std::string> pPltxt_;
	shared_ptr<std::string> pCitxt_;
	shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>> pPlbitsets_;  // bitset形式的明文，如果作为输入，会进行填充；如果作为输出，会去掉填充
	shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>> pCibitsets_;  // bitset形式的密文，如果作为输入，会进行填充；如果作为输出，会去掉填充
	bool op_;
	dynamic_bitset<> key_;
	dynamic_bitset<> IV_;
	dynamic_bitset<> shiftReg_;
};

