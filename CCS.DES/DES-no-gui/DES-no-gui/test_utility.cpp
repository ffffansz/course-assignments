#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include <boost/utility/binary.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "DesBlock.h"
#include "DES_ECB.h"
#include "DES_CBC.h"
#include "DES_CFB.h"
#include "DES_OFB.h"
#include "DES_CTR.h"

using namespace boost;

int main() {
	shared_ptr<std::string> pPltxt = make_shared<std::string>("hello, world!你好，新世界！");
	shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
	dynamic_bitset<> key(std::string("1010101010111011000010010001100000100111001101101100110011011101"));
	dynamic_bitset<> IV(std::string("1010101010111011000010010001100000100111001101101100110011011101"));
	
	DES_ECB ecb_encry(key, pPltxt, pEmptyStr);
	ecb_encry.encry();
	std::cout << *ecb_encry.getCitxt() << std::endl;

	DES_ECB ecb_decry(key, pEmptyStr, ecb_encry.getCitxt());
	ecb_decry.decry();
	std::cout << *ecb_decry.getPltxt() << std::endl;

	DES_CBC cbc_encry(key, IV, pPltxt, pEmptyStr);
	cbc_encry.encry();
	std::cout << *cbc_encry.getCitxt() << std::endl;

	DES_CBC cbc_decry(key, IV, pEmptyStr, cbc_encry.getCitxt());
	cbc_decry.decry();
	std::cout << *cbc_decry.getPltxt() << std::endl;

	DES_CFB cfb_encry(key, IV, pPltxt, pEmptyStr);
	cfb_encry.encry();
	std::cout << *cfb_encry.getCitxt() << std::endl;

	DES_CFB cfb_decry(key, IV, pEmptyStr, cfb_encry.getCitxt());
	cfb_decry.decry();
	std::cout << *cfb_decry.getPltxt() << std::endl;

	DES_OFB ofb_encry(key, IV, pPltxt, pEmptyStr);
	ofb_encry.encry();
	std::cout << *ofb_encry.getCitxt() << std::endl;

	DES_OFB ofb_decry(key, IV, pEmptyStr, ofb_encry.getCitxt());
	ofb_decry.decry();
	std::cout << *ofb_decry.getPltxt() << std::endl;

	DES_CTR ctr_encry(key, IV, pPltxt, pEmptyStr);
	ctr_encry.encry();
	std::cout << *ctr_encry.getCitxt() << std::endl;

	DES_CTR ctr_decry(key, IV, pEmptyStr, ctr_encry.getCitxt());
	ctr_decry.decry();
	std::cout << *ctr_decry.getPltxt() << std::endl;
	
	/*
	std::ifstream in;
	std::ofstream out;
	shared_ptr<dynamic_bitset<>> filesBits = make_shared<dynamic_bitset<>>(2048);
	shared_ptr<dynamic_bitset<>> emptyFilesBits = make_shared<dynamic_bitset<>>();

	
	std::bitset<2048> plain; //256 B
	in.open("C://Users//Libre//Desktop//pl.png", std::ios::binary);
	out.open("C://Users//Libre//Desktop//tmp.txt");
	in.read((char*)&plain, sizeof(plain));
	unsigned cnt = 0;
	while (in.read((char*)&plain, sizeof(plain))) {
		//plain.to_string
		shared_ptr<dynamic_bitset<>> filesBits = boost::make_shared<dynamic_bitset<>>(plain.to_string());
		DES_ECB ebc_encry_f(key, filesBits, emptyFilesBits);
		ebc_encry_f.encry_f();
		plain.reset();
		std::string str;
		to_string(*ebc_encry_f.getCiFileBits(), str);
		plain = std::bitset<2048>(str);
		out.write((char*)&plain, 2048 / 8);
		plain.reset();
		std::cout << cnt << std::endl;
		cnt++;
	}
	in.close();
	out.close();

	in.open("C://Users//Libre//Desktop//tmp.txt", std::ios::binary);
	out.open("C://Users//Libre//Desktop//ret.png");
	//in.read((char*)&plain, sizeof(plain));
	cnt = 0;
	while (in.read((char*)&plain, sizeof(plain))) {
		//plain.to_string
		shared_ptr<dynamic_bitset<>> filesBits = boost::make_shared<dynamic_bitset<>>(plain.to_string());
		DES_ECB ebc_decry_f(key, emptyFilesBits, filesBits);
		ebc_decry_f.decry_f();
		plain.reset();
		std::string str;
		to_string(*ebc_decry_f.getPlFileBits(), str);
		plain = std::bitset<2048>(str);
		out.write((char*)&plain, 2048 / 8);
		plain.reset();
		std::cout << cnt << std::endl;
		cnt++;
	}
	in.close();
	out.close();
	*/
	return 0;
}