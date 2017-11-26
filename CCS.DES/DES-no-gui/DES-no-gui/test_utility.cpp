#include <iostream>
#include <vector>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include <boost/utility/binary.hpp>
#include <boost/array.hpp>
#include "DesBlock.h"

int main() {
	
	///*
	boost::dynamic_bitset<> pltxt(std::string("0001001000110100010101101010101111001101000100110010010100110110"));
	boost::dynamic_bitset<> key(std::string("1010101010111011000010010001100000100111001101101100110011011101"));
	des::DesBlock test(key, pltxt);
	test.encry();
	std::cout << test.getCipherBits() << std::endl;
	//*/
	/*
	boost::dynamic_bitset<> test1(5, BOOST_BINARY(10011));
	size_t size1 = test1.size();
	std::cout << size1 << std::endl;
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p1 = des::util::spliter(test1, 1, 4);
	std::cout << p1.first << " " << p1.second << std::endl; 
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p2 = des::util::spliter(test1, 2, 3);
	std::cout << p2.first << " " << p2.second << std::endl;
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p3 = des::util::spliter(test1, 3, 2);
	std::cout << p3.first << " " << p3.second << std::endl;
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p4 = des::util::spliter(test1, 4, 1);
	std::cout << p4.first << " " << p4.second << std::endl;

	boost::dynamic_bitset<> test2(6, BOOST_BINARY(101011));
	size_t size2 = test2.size();
	std::cout << size2 << std::endl;
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p21 = des::util::spliter(test2, 1, 5);
	std::cout << p21.first << " " << p21.second << std::endl;
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p22 = des::util::spliter(test2, 2, 4);
	std::cout << p22.first << " " << p22.second << std::endl;
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p23 = des::util::spliter(test2, 3, 3);
	std::cout << p23.first << " " << p23.second << std::endl;
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p24 = des::util::spliter(test2, 4, 2);
	std::cout << p24.first << " " << p24.second << std::endl;
	std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<>> p25 = des::util::spliter(test2, 5, 1);
	std::cout << p25.first << " " << p25.second << std::endl;
	*/
	return 0;
}