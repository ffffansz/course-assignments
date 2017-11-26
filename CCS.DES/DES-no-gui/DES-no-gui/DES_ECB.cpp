#include "DES_ECB.h"


void DES_ECB::setPltxt(shared_ptr<std::string>& pPltxt)
{
	pPltxt_ = pPltxt;
	return;
}

void DES_ECB::setCitxt(shared_ptr<std::string>& pCitxt)
{
	pCitxt_ = pCitxt;
	return;
}

shared_ptr<std::string> DES_ECB::getCitxt()
{
	return pCitxt_;
}

shared_ptr<std::string> DES_ECB::getPltxt()
{
	return pPltxt_;
}

DES_ECB::~DES_ECB()
{
}


DES_ECB::DES_ECB(const dynamic_bitset<>& key, const shared_ptr<std::string>& pPltxt, const shared_ptr<std::string>& pCitxt)
	: key_(key), pPltxt_(pPltxt), pCitxt_(pCitxt)
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

void DES_ECB::encry()
{
	assert(op_ == false);
	for (size_t i = 0; i < pPlbitsets_->size(); i++) {
		des::DesBlock partRet(key_, *((*pPlbitsets_)[i]), dynamic_bitset<>());
		partRet.encry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(partRet.getCipherBits());
		pCibitsets_->push_back(pPartRet);
	}
	pCitxt_ = des::util::bitsetsToStr(*pCibitsets_);
	return;
}

void DES_ECB::decry()
{
	assert(op_ == true);
	for (size_t i = 0; i < pCibitsets_->size(); i++) {
		des::DesBlock partRet(key_, dynamic_bitset<>(), *((*pCibitsets_)[i]));
		partRet.decry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(partRet.getPlainBits());
		pPlbitsets_->push_back(pPartRet);
	}
	assert(pPlbitsets_->size() != 0);
	*((*pPlbitsets_)[pPlbitsets_->size() - 1]) = des::util::bytes_unpaded_PKCS7(*((*pPlbitsets_)[pPlbitsets_->size() - 1]));
	pPltxt_ = des::util::bitsetsToStr(*pPlbitsets_);
	return;
}
