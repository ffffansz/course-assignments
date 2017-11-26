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

	std::vector<dynamic_bitset<>> roundKeys;   //每轮的密钥，密钥大小为48 bit，vector大小为16
	
	KeyGen(const dynamic_bitset<>& initKey);
	
	~KeyGen();
private:
	dynamic_bitset<> initKey_;      //64 bit
	dynamic_bitset<> initKey56_;    //56 bit
	dynamic_bitset<> initKey56_L_;  //initKey56的左半部分
	dynamic_bitset<> initKey56_R_;  //initKey56的右半部分

	/* 产生指定轮数的key */
	dynamic_bitset<> genRoundKey_(unsigned round);
};

