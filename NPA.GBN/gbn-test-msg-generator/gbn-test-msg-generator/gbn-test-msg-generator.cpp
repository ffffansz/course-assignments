#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

unsigned calDigits(int num) {
	unsigned digits = 0;
	if (num == 0) return 1;
	while (num != 0) {
		digits++;
		num /= 10;
	}
	return digits;
}

std::string genSpaceStr(int curnum, unsigned maxDigits) {
	std::string str;
	unsigned dig = calDigits(curnum);
	for (unsigned i = dig; i < maxDigits; i++)
		str += " ";
	return str;
}

int main()
{
	std::ofstream ofs("testmsg.txt");
	unsigned size = 0;
	std::cout << "Enter the number of messages you'd like to generate: ";
	while (size <= 0) {
		std::cin >> size;
		if (size <= 0) {
			std::cout << "Please enter a positive number: ";
		}
	}
	unsigned size_dig = calDigits(size);
	if (ofs.is_open()) {
		for (unsigned i = 0; i < size; i++) {
			ofs << "Seq:" + boost::lexical_cast<std::string>(i) + genSpaceStr(i, size_dig) +  " This is a GBN testing message.\n";
		}
		ofs.close();
	}
    return 0;
}

