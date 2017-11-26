#include <iostream>
#include <vector>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include <boost/utility/binary.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "DesBlock.h"
#include "DES_ECB.h"

using namespace boost;

int main() {
	
	/*
	boost::dynamic_bitset<> pltxt(std::string("0001001000110100010101101010101111001101000100110010010100110110"));
	boost::dynamic_bitset<> key(std::string("1010101010111011000010010001100000100111001101101100110011011101"));
	des::DesBlock test1(key, pltxt);
	test1.encry();
	std::cout << test1.getCipherBits() << std::endl;

	boost::dynamic_bitset<> citxt(test1.getCipherBits());
	des::DesBlock test2(key, dynamic_bitset<>(), citxt);
	test2.decry();
	std::cout << test2.getPlainBits() << std::endl;

	*/
	shared_ptr<std::string> pPltxt = make_shared<std::string>("你好，helloworld!");
	shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
	dynamic_bitset<> key(std::string("1010101010111011000010010001100000100111001101101100110011011101"));
	DES_ECB test_encry(key, pPltxt, pEmptyStr);
	test_encry.encry();
	std::cout << *test_encry.getCitxt() << std::endl;

	DES_ECB test_decry(key, pEmptyStr, test_encry.getCitxt());
	test_decry.decry();
	std::cout << *test_decry.getPltxt() << std::endl;
	return 0;
}