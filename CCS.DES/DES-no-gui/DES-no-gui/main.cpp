#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
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

const char hexchar_u[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
const char hexchar_l[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
std::map<char, std::string> binchar;

void welcome();
int ch_ed(const std::string& mode);
void show_main_menu();
void run_testcase();
bool valid_hexchar(const char& c);
std::string hex2bin(const std::string& hexstr);
std::string bin2hex(const std::string& binstr);
void ecb();
void cbc();
void cfb();
void ctr();
void ofb();

int main() {
	welcome();
	bool exit = false;
	while (!exit) {
		bool valid_input_chocice = false;
		while (!valid_input_chocice) {
			show_main_menu();
			std::cout << "��ѡ��Ҫʹ�õ�ģʽ�������Ӧ����ţ�������exit�˳�����>>> ";
			std::string c;
			std::cin >> c;
			if (!(c == "1" || c == "2" || c == "3" || c == "4" || c == "5" || c == "6" || c == "exit")) {
				std::cout << "�����������������" << std::endl;
				continue;
			}
			if (c == "1") {
				ecb();
				continue;
			}
			if (c == "2") {
				cbc();
				continue;
			}
			if (c == "3") {
				cfb();
				continue;
			}
			if (c == "4") {
				ofb();
				continue;
			}
			if (c == "5") {
				ctr();
				continue;
			}
			if (c == "6") {
				run_testcase();
				continue;
			}
			if (c == "exit") {
				std::cout << "�����˳�DES��/���ܳ���..." << std::endl;
				system("pause");
				return 0;
			}
		}
	}
	return 0;
}

void run_testcase() {
	std::ofstream log;
	log.open(".\\log.txt", std::ios::app);
	if (!log.is_open()) {
		std::cout << "log.txt����ʧ��" << std::endl;
	}
	std::string test_str;
	std::string test_key;
	std::string test_iv;
	std::cout << "\n�����������ַ��� >>> ";
	std::cin >> test_str;
	bool valid_input_key = false;
	while (!valid_input_key) {
		std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
		std::cin >> test_key;
		test_key = test_key == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(test_key);
		if (test_key.size() != 64)
			std::cout << "������������������." << std::endl;
		else
			valid_input_key = true;
	}
	bool valid_input_iv = false;
	while (!valid_input_iv) {
		std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
		std::cin >> test_iv;
		test_iv = test_iv == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(test_iv);
		if (test_iv.size() != 64)
			std::cout << "������������������." << std::endl;
		else
			valid_input_iv = true;
	}
	shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(test_str);
	shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
	dynamic_bitset<> key(test_key);
	dynamic_bitset<> IV(test_iv);

	std::string key_to_show = bin2hex(test_key);
	std::string iv_to_show = bin2hex(test_iv);

	std::cout << "\n��ʼDES��/���ܲ���..." << std::endl;
	std::cout << "���������ģ�" << test_str << std::endl;
	std::cout << "��/������Կ��" << key_to_show << std::endl;
	std::cout << "��/����IV��" << iv_to_show << std::endl;

	log << "��ʼDES��/���ܲ���..." << std::endl;
	log << "���������ģ�" << test_str << std::endl;
	log << "��/������Կ��" << key_to_show << std::endl;
	log << "��/����IV��" << iv_to_show << std::endl;

	

	std::cout << "\nEBCģʽ  >>>" << std::endl;
	log << "\nEBCģʽ  >>>" << std::endl;
	
	DES_ECB ecb_encry(key, pPltxt, pEmptyStr);
	ecb_encry.encry();
	//std::cout << "���ܽ�� >>> " << *ecb_encry.getCitxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *ecb_encry.getCitxt() << std::endl;

	DES_ECB ecb_decry(key, pEmptyStr, ecb_encry.getCitxt());
	ecb_decry.decry();
	//std::cout << "���ܽ�� >>> " << *ecb_decry.getPltxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *ecb_decry.getPltxt() << std::endl;

	std::cout << "\nCBCģʽ  >>>" << std::endl;
	log << "\nCBCģʽ  >>>" << std::endl;

	DES_CBC cbc_encry(key, IV, pPltxt, pEmptyStr);
	cbc_encry.encry();
	//std::cout << "���ܽ�� >>> " << *cbc_encry.getCitxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *cbc_encry.getCitxt() << std::endl;

	DES_CBC cbc_decry(key, IV, pEmptyStr, cbc_encry.getCitxt());
	cbc_decry.decry();
	//std::cout << "���ܽ�� >>> " << *cbc_decry.getPltxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *cbc_decry.getPltxt() << std::endl;

	std::cout << "\nCFBģʽ  >>>" << std::endl;

	DES_CFB cfb_encry(key, IV, pPltxt, pEmptyStr);
	cfb_encry.encry();
	//std::cout << "���ܽ�� >>> " << *cfb_encry.getCitxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *cfb_encry.getCitxt() << std::endl;

	DES_CFB cfb_decry(key, IV, pEmptyStr, cfb_encry.getCitxt());
	cfb_decry.decry();
	//std::cout << "���ܽ�� >>> " << *cfb_decry.getPltxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *cfb_decry.getPltxt() << std::endl;

	std::cout << "\nOFBģʽ  >>>" << std::endl;
	log << "\nOFBģʽ  >>>" << std::endl;

	DES_OFB ofb_encry(key, IV, pPltxt, pEmptyStr);
	ofb_encry.encry();
	//std::cout << "���ܽ�� >>> " << *ofb_encry.getCitxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *ofb_encry.getCitxt() << std::endl;

	DES_OFB ofb_decry(key, IV, pEmptyStr, ofb_encry.getCitxt());
	ofb_decry.decry();
	//std::cout << "���ܽ�� >>> " << *ofb_decry.getPltxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *ofb_decry.getPltxt() << std::endl;

	std::cout << "\nCTRģʽ  >>>" << std::endl;
	log << "\nCTRģʽ  >>>" << std::endl;

	DES_CTR ctr_encry(key, IV, pPltxt, pEmptyStr);
	ctr_encry.encry();
	//std::cout << "���ܽ�� >>> " << *ctr_encry.getCitxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *ctr_encry.getCitxt() << std::endl;

	DES_CTR ctr_decry(key, IV, pEmptyStr, ctr_encry.getCitxt());
	ctr_decry.decry();
	//std::cout << "���ܽ�� >>> " << *ctr_decry.getPltxt() << std::endl;
	std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
	log << "���ܽ�� >>> " << *ctr_decry.getPltxt() << std::endl;

	log.close();
	/*
	/* �ļ�����
	{
		std::ifstream in;
		std::ofstream out;
		unsigned long long size;
		in.open("F://DevFiles/course-assignments/CCS.DES/DES-no-gui/Release/test.txt", std::ios::binary);
		out.open("F://DevFiles/course-assignments/CCS.DES/DES-no-gui/Release/encry_test.txt", std::ios::binary);
		in.seekg(0, std::ios::end);
		size = in.tellg();
		std::cout << "size of :" << in.tellg() << std::endl;
		in.seekg(0, std::ios::beg);
		//boost::dynamic_bitset<> buffer(size * 8);
		shared_ptr<dynamic_bitset<>> pEmptyFileBits = boost::make_shared<dynamic_bitset<>>();
		std::string longStr;
		std::bitset<8> aByte;
		std::string aByteStr;
		size_t byteCnt = 0;
		while (in.read((char *)&aByte, 1)) {
			aByteStr = aByte.to_string();
			longStr += aByteStr;
			byteCnt++;
			// һ�μ���256 Byte
			if (byteCnt == 256) {
				shared_ptr<dynamic_bitset<>> pFilePlBits = boost::make_shared<dynamic_bitset<>>(longStr);

				DES_ECB ecb_encry_f(key, pFilePlBits, pEmptyFileBits);
				ecb_encry_f.encry_f();
				std::string ciStr;
				to_string(*ecb_encry_f.getCiFileBits(), ciStr);
				std::bitset<256 * 8> out_buffer(ciStr);
				out.write((char *)& out_buffer, 256);
				byteCnt = 0;
			}
			aByte.reset();
		}
		/* ��������N Byte������256 B�������ֽڼ���
		shared_ptr<dynamic_bitset<>> pFilePlBits = boost::make_shared<dynamic_bitset<>>(longStr);
		//std::bitset<8> eachByte;
		for (int i = 0; i < longStr.size(); i += 8) {
			shared_ptr<dynamic_bitset<>> pFilePlBits = boost::make_shared<dynamic_bitset<>>(longStr.substr(i, 8));
			DES_ECB ecb_encry_f(key, pFilePlBits, pEmptyFileBits);
			ecb_encry_f.encry_f();
			std::string ciStr;
			to_string(*ecb_encry_f.getCiFileBits(), ciStr);
			std::bitset<64> eachByte(ciStr);
			out.write((char *)& eachByte, 1);
		}
		//in.read((char *)&aByte, 1);
		//std::cout << buffer << std::endl;
		in.close();
		out.close();
	}
	
	/* �ļ�����
	{
		std::ifstream in;
		std::ofstream out;
		unsigned long long size;
		in.open("F://DevFiles/course-assignments/CCS.DES/DES-no-gui/Release/encry_test.txt", std::ios::binary);
		out.open("F://DevFiles/course-assignments/CCS.DES/DES-no-gui/Release/decry_test.txt", std::ios::binary);
		in.seekg(0, std::ios::end);
		size = in.tellg();
		std::cout << "size of :" << in.tellg() << std::endl;
		in.seekg(0, std::ios::beg);
		//boost::dynamic_bitset<> buffer(size * 8);
		shared_ptr<dynamic_bitset<>> pEmptyFileBits = boost::make_shared<dynamic_bitset<>>();
		std::string longStr;
		std::bitset<8> aByte;
		std::string aByteStr;
		size_t byteCnt = 0;
		while (in.read((char *)&aByte, 1)) {
			aByteStr = aByte.to_string();
			longStr += aByteStr;
			byteCnt++;
			/* һ�ν���256 Byte
			if (byteCnt == 256) {
				shared_ptr<dynamic_bitset<>> pFileCiBits = boost::make_shared<dynamic_bitset<>>(longStr);

				DES_ECB ecb_decry_f(key, pEmptyFileBits, pFileCiBits);
				ecb_decry_f.decry_f();
				std::string plStr;
				to_string(*ecb_decry_f.getPlFileBits(), plStr);
				std::bitset<256 * 8> out_buffer(plStr);
				out.write((char *)& out_buffer, 256);
				byteCnt = 0;
			}
			aByte.reset();
		}
		/* ��������N Byte������256 B�������ֽڽ���
		shared_ptr<dynamic_bitset<>> pFilePlBits = boost::make_shared<dynamic_bitset<>>(longStr);
		//std::bitset<8> eachByte;
		for (int i = 0; i < longStr.size(); i += 64) {
			shared_ptr<dynamic_bitset<>> pFileCiBits = boost::make_shared<dynamic_bitset<>>(longStr.substr(i, 64));
			DES_ECB ecb_decry_f(key, pEmptyFileBits, pFileCiBits);
			ecb_decry_f.decry_f();
			std::string plStr;
			to_string(*ecb_decry_f.getPlFileBits(), plStr);
			std::bitset<64> eachByte(plStr);
			out.write((char *)& eachByte, 8);
		}
		//in.read((char *)&aByte, 1);
		//std::cout << buffer << std::endl;
		in.close();
		out.close();
	}
	*/
	return;
}

void welcome()
{
	for (unsigned i = 0; i < 16; i++) {
		std::bitset<4> bin(i);
		binchar[hexchar_u[i]] = bin.to_string();
		binchar[hexchar_l[i]] = bin.to_string();
	}
	std::cout << "<--------------- DES��/���ܳ��� --------------->" << std::endl;
	std::cout << "���ߣ�15336036  15�����繤��  ������" << std::endl;
	return;
}

int ch_ed(const std::string& mode)
{
	std::cout << "\n<--------------- DES";
	std::cout << " - " << mode << "��/����ģʽ --------------->" << std::endl;
	std::cout << "1. ����" << std::endl;
	std::cout << "2. ����" << std::endl;
	//std::cout << "����back�����ϼ��˵�" << std::endl;
	std::string c;
	bool valid_input = false;
	while (!valid_input) {
		std::cout << "��ѡ��Ҫʹ�õ�ģʽ�������Ӧ����ţ�������back�����ϼ��˵��� >>> ";
		std::cin >> c;
		if (!(c == "1" || c == "2" || c == "back")) {
			std::cout << "������������������";
			continue;
		}
		if (c == "1")	return 0;
		if (c == "2")	return 1;
		if (c == "back")	return -1;
	}
	return -1;
}

void show_main_menu()
{
	//std::cout << "\n<--------------- DES��/���ܳ��� --------------->" << std::endl;
	std::cout << "\n1. ECBģʽ ��Electronic Codebook Mode��" << std::endl;
	std::cout << "2. CBCģʽ ��Cipher Block Chaining Mode��" << std::endl;
	std::cout << "3. CFBģʽ ��Cipher Feedback Mode��" << std::endl;
	std::cout << "4. OFBģʽ ��Output Feedback Mode��" << std::endl;
	std::cout << "5. CTRģʽ ��Counter Mode��" << std::endl;
	std::cout << "6. ����ģʽ��TestCase" << std::endl;
	return;
}

bool valid_hexchar(const char& c) {
	if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
		return false;
	return true;
}

std::string hex2bin(const std::string & hexstr)
{
	std::string binstr;
	for (size_t i = 0; i < hexstr.size(); i++) {
		if (!valid_hexchar(hexstr[i]))
			return "";
		binstr += binchar[hexstr[i]];
	}

	return binstr;
}

std::string bin2hex(const std::string & binstr)
{
	if (binstr.size() % 4 != 0)
		return "";
	std::string hexstr;
	size_t i = 0;
	for (size_t i = 0; i < binstr.size(); i += 4) {
		if (!(binstr[i] == '0' || binstr[i] == '1'))			return "";
		if (!(binstr[i + 1] == '0' || binstr[i + 1] == '1'))	return "";
		if (!(binstr[i + 2] == '0' || binstr[i + 2] == '1'))	return "";
		if (!(binstr[i + 3] == '0' || binstr[i + 3] == '1'))	return "";
		std::bitset<4> ahex(binstr.substr(i, 4));
		hexstr += hexchar_u[ahex.to_ulong()];
	}

	return hexstr;
}

void ecb()
{

	int c = ch_ed("ECB");
	if (c == -1)	return;
	std::ofstream log;
	log.open(".\\log.txt", std::ios::app);
	if (!log.is_open()) {
		std::cout << "log.txt����ʧ��" << std::endl;
	}
	if (c == 0) {
		std::string en_str;
		std::string key_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		//dynamic_bitset<> IV(test_iv);

		std::string key_to_show = bin2hex(key_str);
		//std::string iv_to_show = bin2hex(test_iv);

		std::cout << "��ʼDES - " << "ECB����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		//std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "ECB����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_ECB ecb_encry(key, pPltxt, pEmptyStr);
		ecb_encry.encry();
		//std::cout << "���ܽ�� >>> " << *ecb_encry.getCitxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *ecb_encry.getCitxt() << std::endl;
	}
	else {
		std::string de_str;
		std::string key_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> de_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		shared_ptr<std::string> pCitxt = boost::make_shared<std::string>(de_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		//dynamic_bitset<> IV(test_iv);

		std::string key_to_show = bin2hex(key_str);
		//std::string iv_to_show = bin2hex(test_iv);

		std::cout << "��ʼDES - " << "ECB����..." << std::endl;
		std::cout << "���������ģ�" << de_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		//std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "ECB����..." << std::endl;
		log << "���������ģ�" << de_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_ECB ecb_decry(key, pEmptyStr, pCitxt);
		ecb_decry.encry();
		//std::cout << "���ܽ�� >>> " << *ecb_decry.getPltxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *ecb_decry.getPltxt() << std::endl;
	}
	log.close();
	return;
}

void cfb()
{

	int c = ch_ed("CFB");
	if (c == -1)	return;
	std::ofstream log;
	log.open(".\\log.txt", std::ios::app);
	if (!log.is_open()) {
		std::cout << "log.txt����ʧ��" << std::endl;
	}
	if (c == 0) {
		std::string en_str;
		std::string key_str;
		std::string iv_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		bool valid_input_iv = false;
		while (!valid_input_iv) {
			std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
			std::cin >> iv_str;
			iv_str = iv_str == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(iv_str);
			if (iv_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_iv = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		dynamic_bitset<> IV(iv_str);

		std::string key_to_show = bin2hex(key_str);
		std::string iv_to_show = bin2hex(iv_str);

		std::cout << "��ʼDES - " << "CFB����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "CFB����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_CFB cfb_encry(key, IV, pPltxt, pEmptyStr);
		cfb_encry.encry();
		//std::cout << "���ܽ�� >>> " << *cfb_encry.getCitxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *cfb_encry.getCitxt() << std::endl;
	}
	else {
		std::string en_str;
		std::string key_str;
		std::string iv_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		bool valid_input_iv = false;
		while (!valid_input_iv) {
			std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
			std::cin >> iv_str;
			iv_str = iv_str == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(iv_str);
			if (iv_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_iv = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		dynamic_bitset<> IV(iv_str);

		std::string key_to_show = bin2hex(key_str);
		std::string iv_to_show = bin2hex(iv_str);

		std::cout << "��ʼDES - " << "CFB����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "CFB����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_CFB cfb_decry(key, IV, pPltxt, pEmptyStr);
		cfb_decry.decry();
		//std::cout << "���ܽ�� >>> " << *cfb_decry.getPltxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *cfb_decry.getPltxt() << std::endl;
	}
	log.close();
	return;
}

void ofb()
{

	int c = ch_ed("OFB");
	if (c == -1)	return;
	std::ofstream log;
	log.open(".\\log.txt", std::ios::app);
	if (!log.is_open()) {
		std::cout << "log.txt����ʧ��" << std::endl;
	}
	if (c == 0) {
		std::string en_str;
		std::string key_str;
		std::string iv_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		bool valid_input_iv = false;
		while (!valid_input_iv) {
			std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
			std::cin >> iv_str;
			iv_str = iv_str == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(iv_str);
			if (iv_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_iv = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		dynamic_bitset<> IV(iv_str);

		std::string key_to_show = bin2hex(key_str);
		std::string iv_to_show = bin2hex(iv_str);

		std::cout << "��ʼDES - " << "OFB����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "OFB����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_OFB ofb_encry(key, IV, pPltxt, pEmptyStr);
		ofb_encry.encry();
		//std::cout << "���ܽ�� >>> " << *ofb_encry.getCitxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *ofb_encry.getCitxt() << std::endl;
	}
	else {
		std::string en_str;
		std::string key_str;
		std::string iv_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		bool valid_input_iv = false;
		while (!valid_input_iv) {
			std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
			std::cin >> iv_str;
			iv_str = iv_str == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(iv_str);
			if (iv_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_iv = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		dynamic_bitset<> IV(iv_str);

		std::string key_to_show = bin2hex(key_str);
		std::string iv_to_show = bin2hex(iv_str);

		std::cout << "��ʼDES - " << "OFB����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "OFB����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_OFB ofb_decry(key, IV, pPltxt, pEmptyStr);
		ofb_decry.decry();
		//std::cout << "���ܽ�� >>> " << *ofb_decry.getPltxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *ofb_decry.getPltxt() << std::endl;
	}
	log.close();
	return;
}

void cbc()
{

	int c = ch_ed("CBC");
	if (c == -1)	return;
	std::ofstream log;
	log.open(".\\log.txt", std::ios::app);
	if (!log.is_open()) {
		std::cout << "log.txt����ʧ��" << std::endl;
	}
	if (c == 0) {
		std::string en_str;
		std::string key_str;
		std::string iv_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		bool valid_input_iv = false;
		while (!valid_input_iv) {
			std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
			std::cin >> iv_str;
			iv_str = iv_str == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(iv_str);
			if (iv_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_iv = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		dynamic_bitset<> IV(iv_str);

		std::string key_to_show = bin2hex(key_str);
		std::string iv_to_show = bin2hex(iv_str);

		std::cout << "��ʼDES - " << "CBC����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "CBC����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_CBC cbc_encry(key, IV, pPltxt, pEmptyStr);
		cbc_encry.encry();
		//std::cout << "���ܽ�� >>> " << *cbc_encry.getCitxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *cbc_encry.getCitxt() << std::endl;
	}
	else {
		std::string en_str;
		std::string key_str;
		std::string iv_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		bool valid_input_iv = false;
		while (!valid_input_iv) {
			std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
			std::cin >> iv_str;
			iv_str = iv_str == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(iv_str);
			if (iv_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_iv = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		dynamic_bitset<> IV(iv_str);

		std::string key_to_show = bin2hex(key_str);
		std::string iv_to_show = bin2hex(iv_str);

		std::cout << "��ʼDES - " << "CBC����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "CBC����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_CBC cbc_decry(key, IV, pPltxt, pEmptyStr);
		cbc_decry.decry();
		//std::cout << "���ܽ�� >>> " << *cbc_decry.getPltxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *cbc_decry.getPltxt() << std::endl;
	}
	log.close();
	return;
}

void ctr()
{

	int c = ch_ed("CTR");
	if (c == -1)	return;
	std::ofstream log;
	log.open(".\\log.txt", std::ios::app);
	if (!log.is_open()) {
		std::cout << "log.txt����ʧ��" << std::endl;
	}
	if (c == 0) {
		std::string en_str;
		std::string key_str;
		std::string iv_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		bool valid_input_iv = false;
		while (!valid_input_iv) {
			std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
			std::cin >> iv_str;
			iv_str = iv_str == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(iv_str);
			if (iv_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_iv = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		dynamic_bitset<> IV(iv_str);

		std::string key_to_show = bin2hex(key_str);
		std::string iv_to_show = bin2hex(iv_str);

		std::cout << "��ʼDES - " << "CTR����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "CTR����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_CTR ctr_encry(key, IV, pPltxt, pEmptyStr);
		ctr_encry.encry();
		//std::cout << "���ܽ�� >>> " << *ctr_encry.getCitxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *ctr_encry.getCitxt() << std::endl;
	}
	else {
		std::string en_str;
		std::string key_str;
		std::string iv_str;
		std::cout << "\n������Ҫ���ܵ��ַ��� >>> ";
		std::cin >> en_str;
		bool valid_input_key = false;
		while (!valid_input_key) {
			std::cout << "������16λ��16��������ʾ��Key������df��ʹ��Ĭ��Key��>>> ";
			std::cin >> key_str;
			key_str = key_str == "df" ? "1010101010111011000010010001100000100111001101101100110011011101" : hex2bin(key_str);
			if (key_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_key = true;
		}
		bool valid_input_iv = false;
		while (!valid_input_iv) {
			std::cout << "������16λ��16��������ʾ�ĳ�ʼ������������df��ʹ��Ĭ��IV��>>> ";
			std::cin >> iv_str;
			iv_str = iv_str == "df" ? "1010101010111011000010010001100000100111001101101100110011010000" : hex2bin(iv_str);
			if (iv_str.size() != 64)
				std::cout << "������������������." << std::endl;
			else
				valid_input_iv = true;
		}
		shared_ptr<std::string> pPltxt = boost::make_shared<std::string>(en_str);
		shared_ptr<std::string> pEmptyStr = make_shared<std::string>();
		dynamic_bitset<> key(key_str);
		dynamic_bitset<> IV(iv_str);

		std::string key_to_show = bin2hex(key_str);
		std::string iv_to_show = bin2hex(iv_str);

		std::cout << "��ʼDES - " << "CTR����..." << std::endl;
		std::cout << "���������ģ�" << en_str << std::endl;
		std::cout << "��/������Կ��" << key_to_show << std::endl;
		std::cout << "��/����IV��" << iv_to_show << std::endl;

		log << "��ʼDES - " << "CTR����..." << std::endl;
		log << "���������ģ�" << en_str << std::endl;
		log << "��/������Կ��" << key_to_show << std::endl;

		DES_CTR ctr_decry(key, IV, pPltxt, pEmptyStr);
		ctr_decry.decry();
		//std::cout << "���ܽ�� >>> " << *ctr_decry.getPltxt() << std::endl;
		std::cout << "������ɣ�����log.txt�в鿴�����" << std::endl;
		log << "���ܽ�� >>> " << *ctr_decry.getPltxt() << std::endl;
	}
	log.close();
	return;
}