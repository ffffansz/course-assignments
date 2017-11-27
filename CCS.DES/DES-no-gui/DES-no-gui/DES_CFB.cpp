#include "DES_CFB.h"


void DES_CFB::setPltxt(shared_ptr<std::string>& pPltxt)
{
	pPltxt_ = pPltxt;
	return;
}

void DES_CFB::setCitxt(shared_ptr<std::string>& pCitxt)
{
	pCitxt_ = pCitxt;
	return;
}

shared_ptr<std::string> DES_CFB::getCitxt()
{
	return pCitxt_;
}

shared_ptr<std::string> DES_CFB::getPltxt()
{
	return pPltxt_;
}

shared_ptr<dynamic_bitset<>> DES_CFB::getCiFileBits()
{
	return pCiFileBits_;
}

shared_ptr<dynamic_bitset<>> DES_CFB::getPlFileBits()
{
	return pPlFileBits_;
}

DES_CFB::~DES_CFB()
{
}


DES_CFB::DES_CFB(const dynamic_bitset<>& key, const dynamic_bitset<>& IV, const shared_ptr<std::string>& pPltxt, const shared_ptr<std::string>& pCitxt)
	: key_(key), pPltxt_(pPltxt), pCitxt_(pCitxt), IV_(IV), shiftReg_(IV)
{
	assert(!key_.empty());
	assert(pPltxt->size() != 0 || pCitxt->size() != 0);
	if (pPltxt->size() != 0 && pCitxt->size() == 0) {
		op_ = false;
		pPlbitsets_ = des::util::strToBitsets(*pPltxt_);
		pCibitsets_ = make_shared<std::vector<shared_ptr<dynamic_bitset<>>>>();
		assert(pPlbitsets_->size() != 0);
		*((*pPlbitsets_)[pPlbitsets_->size() - 1]) = des::util::bytes_paded_PKCS7(*((*pPlbitsets_)[pPlbitsets_->size() - 1]), 64);  //对最后一个可能不足64位的bitset进行填充
	}
	else {
		op_ = true;
		pCibitsets_ = des::util::strToBitsets(*pCitxt_);
		pPlbitsets_ = make_shared<std::vector<shared_ptr<dynamic_bitset<>>>>();
		assert(pCibitsets_->size() != 0);
		assert((*pCibitsets_)[pCibitsets_->size() - 1]->size() == 64); // 约定输入的解密的bitset均为64位
		//*((*pCibitsets_)[pCibitsets_->size() - 1]) = des::util::bytes_paded_PKCS7(*((*pCibitsets_)[pCibitsets_->size() - 1]), 64);  //对最后一个可能不足64位的bitset进行填充
	}
}

DES_CFB::DES_CFB(const dynamic_bitset<>& key, const dynamic_bitset<>& IV, const shared_ptr<dynamic_bitset<>>& pPlFileBits, const shared_ptr<dynamic_bitset<>>& pCiFileBits)
	: key_(key), IV_(IV), pPlFileBits_(pPlFileBits), pCiFileBits_(pCiFileBits)
{
	assert(!key_.empty());
	assert(pPlFileBits->size() != 0 || pCiFileBits->size() != 0);
	if (pPlFileBits->size() != 0 && pCiFileBits->size() == 0) {
		op_ = false;
		pPlbitsets_ = make_shared<std::vector<shared_ptr<dynamic_bitset<>>>>();
		std::vector<dynamic_bitset<>> tmp = des::util::spliter_n(*pPlFileBits, 64);
		for (std::vector<dynamic_bitset<>>::iterator it = tmp.begin(); it != tmp.end(); it++) {
			shared_ptr<dynamic_bitset<>> local = boost::make_shared<dynamic_bitset<>>(*it);
			pPlbitsets_->push_back(local);
		}
		pCibitsets_ = make_shared<std::vector<shared_ptr<dynamic_bitset<>>>>();
		assert(pPlbitsets_->size() != 0);
		*((*pPlbitsets_)[pPlbitsets_->size() - 1]) = des::util::bytes_paded_PKCS7(*((*pPlbitsets_)[pPlbitsets_->size() - 1]), 64);  //对最后一个可能不足64位的bitset进行填充
	}
	else {
		op_ = true;
		pCibitsets_ = make_shared<std::vector<shared_ptr<dynamic_bitset<>>>>();
		std::vector<dynamic_bitset<>> tmp = des::util::spliter_n(*pCiFileBits, 64);
		for (std::vector<dynamic_bitset<>>::iterator it = tmp.begin(); it != tmp.end(); it++) {
			shared_ptr<dynamic_bitset<>> local = boost::make_shared<dynamic_bitset<>>(*it);
			pCibitsets_->push_back(local);
		}
		pPlbitsets_ = make_shared<std::vector<shared_ptr<dynamic_bitset<>>>>();
		assert(pCibitsets_->size() != 0);
		assert((*pCibitsets_)[pCibitsets_->size() - 1]->size() == 64); // 约定输入的解密的bitset均为64位
																	   //*((*pCibitsets_)[pCibitsets_->size() - 1]) = des::util::bytes_paded_PKCS7(*((*pCibitsets_)[pCibitsets_->size() - 1]), 64);  //对最后一个可能不足64位的bitset进行填充
	}
}
void DES_CFB::encry()
{
	assert(op_ == false);
	for (size_t i = 0; i < pPlbitsets_->size(); i++) {
		des::DesBlock partRet(key_, shiftReg_, dynamic_bitset<>());
		partRet.encry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(*((*pPlbitsets_)[i]) ^ des::util::leftmostBits(partRet.getCipherBits(), (*pPlbitsets_)[i]->size()));
		pCibitsets_->push_back(pPartRet);
		shiftReg_ = des::util::padLeftShift(shiftReg_, *((*pCibitsets_)[i]), (*pPlbitsets_)[i]->size());
	}
	pCitxt_ = des::util::bitsetsToStr(*pCibitsets_);
	shiftReg_ = IV_;
	return;
}

void DES_CFB::encry_f()
{
	assert(op_ == false);
	for (size_t i = 0; i < pPlbitsets_->size(); i++) {
		des::DesBlock partRet(key_, shiftReg_, dynamic_bitset<>());
		partRet.encry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(*((*pPlbitsets_)[i]) ^ des::util::leftmostBits(partRet.getCipherBits(), (*pPlbitsets_)[i]->size()));
		pCibitsets_->push_back(pPartRet);
		shiftReg_ = des::util::padLeftShift(shiftReg_, *((*pCibitsets_)[i]), (*pPlbitsets_)[i]->size());
	}
	pCiFileBits_ = des::util::multi_combiner(*pCibitsets_);
	shiftReg_ = IV_;
	return;
}

void DES_CFB::decry()
{
	assert(op_ == true);
	des::DesBlock firstPartRet(key_, shiftReg_, dynamic_bitset<>());
	firstPartRet.encry();
	shared_ptr<dynamic_bitset<>> pFirstPartRet = boost::make_shared<dynamic_bitset<>>(*((*pCibitsets_)[0]) ^ des::util::leftmostBits(firstPartRet.getCipherBits(), (*pCibitsets_)[0]->size()));
	pPlbitsets_->push_back(pFirstPartRet);
	shiftReg_ = des::util::padLeftShift(shiftReg_, *((*pCibitsets_)[0]), (*pCibitsets_)[0]->size());
	for (size_t i = 1; i < pCibitsets_->size(); i++) {
		des::DesBlock partRet(key_, shiftReg_, dynamic_bitset<>());
		partRet.encry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(*((*pCibitsets_)[i]) ^ des::util::leftmostBits(partRet.getCipherBits(), (*pCibitsets_)[i]->size()));
		pPlbitsets_->push_back(pPartRet);
		shiftReg_ = des::util::padLeftShift(shiftReg_, *((*pCibitsets_)[i]), (*pCibitsets_)[i]->size());
	}
	assert(pPlbitsets_->size() != 0);
	*((*pPlbitsets_)[pPlbitsets_->size() - 1]) = des::util::bytes_unpaded_PKCS7(*((*pPlbitsets_)[pPlbitsets_->size() - 1]));
	pPltxt_ = des::util::bitsetsToStr(*pPlbitsets_);
	shiftReg_ = IV_;
	return;
}

void DES_CFB::decry_f()
{
	assert(op_ == true);
	des::DesBlock firstPartRet(key_, shiftReg_, dynamic_bitset<>());
	firstPartRet.encry();
	shared_ptr<dynamic_bitset<>> pFirstPartRet = boost::make_shared<dynamic_bitset<>>(*((*pCibitsets_)[0]) ^ des::util::leftmostBits(firstPartRet.getCipherBits(), (*pCibitsets_)[0]->size()));
	pPlbitsets_->push_back(pFirstPartRet);
	shiftReg_ = des::util::padLeftShift(shiftReg_, *((*pCibitsets_)[0]), (*pCibitsets_)[0]->size());
	for (size_t i = 1; i < pCibitsets_->size(); i++) {
		des::DesBlock partRet(key_, shiftReg_, dynamic_bitset<>());
		partRet.encry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(*((*pCibitsets_)[i]) ^ des::util::leftmostBits(partRet.getCipherBits(), (*pCibitsets_)[i]->size()));
		pPlbitsets_->push_back(pPartRet);
		shiftReg_ = des::util::padLeftShift(shiftReg_, *((*pCibitsets_)[i]), (*pCibitsets_)[i]->size());
	}
	assert(pPlbitsets_->size() != 0);
	*((*pPlbitsets_)[pPlbitsets_->size() - 1]) = des::util::bytes_unpaded_PKCS7(*((*pPlbitsets_)[pPlbitsets_->size() - 1]));
	pPlFileBits_ = des::util::multi_combiner(*pPlbitsets_);
	shiftReg_ = IV_;
	return;
}
