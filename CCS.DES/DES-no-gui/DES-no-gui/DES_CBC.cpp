#include "DES_CBC.h"


void DES_CBC::setPltxt(shared_ptr<std::string>& pPltxt)
{
	pPltxt_ = pPltxt;
	return;
}

void DES_CBC::setCitxt(shared_ptr<std::string>& pCitxt)
{
	pCitxt_ = pCitxt;
	return;
}

shared_ptr<std::string> DES_CBC::getCitxt()
{
	return pCitxt_;
}

shared_ptr<std::string> DES_CBC::getPltxt()
{
	return pPltxt_;
}

DES_CBC::~DES_CBC()
{
}


DES_CBC::DES_CBC(const dynamic_bitset<>& key, const dynamic_bitset<>& IV, const shared_ptr<std::string>& pPltxt, const shared_ptr<std::string>& pCitxt)
	: key_(key), IV_(IV), pPltxt_(pPltxt), pCitxt_(pCitxt)
{
	assert(key.size() == 64);
	assert(IV.size() == 64);
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

void DES_CBC::encry()
{
	assert(op_ == false);
	dynamic_bitset<> firstTmpBs = (*(*pPlbitsets_)[0]) ^ IV_;
	des::DesBlock firstPartRet(key_, firstTmpBs, dynamic_bitset<>());
	firstPartRet.encry();
	shared_ptr<dynamic_bitset<>> pFirstPartRet = boost::make_shared<dynamic_bitset<>>(firstPartRet.getCipherBits());
	pCibitsets_->push_back(pFirstPartRet);
	for (size_t i = 1; i < pPlbitsets_->size(); i++) {
		dynamic_bitset<> tmpBs = (*(*pPlbitsets_)[i]) ^ (*(*pCibitsets_)[i - 1]);
		des::DesBlock partRet(key_, tmpBs, dynamic_bitset<>());
		partRet.encry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(partRet.getCipherBits());
		pCibitsets_->push_back(pPartRet);
	}
	pCitxt_ = des::util::bitsetsToStr(*pCibitsets_);
	return;
}

void DES_CBC::decry()
{
	assert(op_ == true);
	des::DesBlock firstPartRet(key_, dynamic_bitset<>(), (*(*pCibitsets_)[0]));
	firstPartRet.decry();
	shared_ptr<dynamic_bitset<>> pFisrtPartRet = boost::make_shared<dynamic_bitset<>>(firstPartRet.getPlainBits() ^ IV_);
	pPlbitsets_->push_back(pFisrtPartRet);
	for (size_t i = 1; i < pCibitsets_->size(); i++) {
		des::DesBlock partRet(key_, dynamic_bitset<>(), *((*pCibitsets_)[i]));
		partRet.decry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(partRet.getPlainBits() ^ *((*pCibitsets_)[i - 1]));
		pPlbitsets_->push_back(pPartRet);
	}
	assert(pPlbitsets_->size() != 0);
	*((*pPlbitsets_)[pPlbitsets_->size() - 1]) = des::util::bytes_unpaded_PKCS7(*((*pPlbitsets_)[pPlbitsets_->size() - 1]));
	pPltxt_ = des::util::bitsetsToStr(*pPlbitsets_);
	return;
}
