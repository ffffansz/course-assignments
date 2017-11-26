#include "des_utility.h"
#include <iostream>
#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <boost/utility/binary.hpp>
#include <boost/array.hpp>

int main() {
	boost::dynamic_bitset<> bs1(5, BOOST_BINARY(10000));
	boost::dynamic_bitset<> test;
	/*
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p = desutil::spliter(bs1, 3, 2);
	std::cout << p.first << std::endl;
	std::cout << p.second << std::endl;
	test = desutil::leftmostBits(bs1, 3);
	std::cout << test.size() << std::endl;
	std::cout << test << std::endl;
	test = desutil::rightmostBits(bs1, 2);
	std::cout << test.size() << std::endl;
	std::cout << test << std::endl;
	*/
	boost::dynamic_bitset<> bs2(8, BOOST_BINARY(11111));
	boost::array<unsigned, 4> table = { 1, 2, 3, 1 };
	test = desutil::permute(bs1, table);
	std::cout << test << std::endl;
	//Out: 1001
	test.resize(8);
	test = desutil::bytes_paded_PKCS7(bs2, 32);
	std::cout << test << std::endl;
	//0001 1111 0000 0011 0000 0011 0000 0011
	test = desutil::bytes_unpaded_PKCS7(test);
	std::cout << test << std::endl;
	//0001 1111
	return 0;
}