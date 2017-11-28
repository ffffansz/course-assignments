#pragma once
#include <vector>
#include <iostream>
#include <boost/array.hpp>
#include <boost/dynamic_bitset.hpp>
#include "des_utility.h"
#include "des_tables.h"

using namespace boost;

class KeyGen
{
public:

	std::vector<dynamic_bitset<>> roundKeys;   //ÿ�ֵ���Կ����Կ��СΪ48 bit��vector��СΪ16
	
	KeyGen(const dynamic_bitset<>& initKey);
	
	~KeyGen();
private:
	dynamic_bitset<> initKey_;      //64 bit
	dynamic_bitset<> initKey56_;    //56 bit
	dynamic_bitset<> initKey56_L_;  //initKey56����벿��
	dynamic_bitset<> initKey56_R_;  //initKey56���Ұ벿��

	/* ����ָ��������key */
	dynamic_bitset<> genRoundKey_(unsigned round);
};

