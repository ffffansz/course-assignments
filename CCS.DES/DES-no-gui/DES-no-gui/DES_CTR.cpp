#include "DES_CTR.h"


void DES_CTR::setPltxt(shared_ptr<std::string>& pPltxt)
{
	pPltxt_ = pPltxt;
	return;
}

void DES_CTR::setCitxt(shared_ptr<std::string>& pCitxt)
{
	pCitxt_ = pCitxt;
	return;
}

shared_ptr<std::string> DES_CTR::getCitxt()
{
	return pCitxt_;
}

shared_ptr<std::string> DES_CTR::getPltxt()
{
	return pPltxt_;
}

DES_CTR::~DES_CTR()
{
}

void DES_CTR::updateCounter_(unsigned i)
{
	std::string cntStr, iStr;
	dynamic_bitset<> iBs(64, i);
	to_string(counter_, cntStr);
	to_string(iBs, iStr);
	char carry = '0';
	std::string::reverse_iterator it1 = cntStr.rbegin();      //从低位开始
	std::string::reverse_iterator it2 = iStr.rbegin();
	for (; it1 != cntStr.rend(); it1++, it2++) {
		if (*it1 == '0' && *it2 == '0') {
			*it1 = carry;
			carry = '0';
			continue;
		}
		if (*it1 == '1' && *it2 == '0') {
			*it1 = carry == '0' ? '1' : '0';
			//carry不变
			continue;
		}
		if (*it1 == '0' && *it2 == '1') {
			*it1 = carry == '0' ? '1' : '0';
			//carry不变
			continue;
		}
		if (*it1 == '1' && *it2 == '1') {
			*it1 = carry;
			carry = '1';
		}
	}
	counter_ = dynamic_bitset<>(cntStr);
	return;
}

void DES_CTR::resetCounter_()
{
	counter_ = IV_;
	return;
}


DES_CTR::DES_CTR(const dynamic_bitset<>& key, const dynamic_bitset<>& IV, const shared_ptr<std::string>& pPltxt, const shared_ptr<std::string>& pCitxt)
	: key_(key), pPltxt_(pPltxt), pCitxt_(pCitxt), IV_(IV), counter_(IV)
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

void DES_CTR::encry()
{
	assert(op_ == false);
	for (size_t i = 0; i < pPlbitsets_->size(); i++) {
		des::DesBlock partRet(key_, counter_, dynamic_bitset<>());
		partRet.encry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(*((*pPlbitsets_)[i]) ^ partRet.getCipherBits());
		pCibitsets_->push_back(pPartRet);
		updateCounter_(i);
	}
	pCitxt_ = des::util::bitsetsToStr(*pCibitsets_);
	counter_ = IV_;
	return;
}

void DES_CTR::decry()
{
	assert(op_ == true);
	for (size_t i = 0; i < pCibitsets_->size(); i++) {
		des::DesBlock partRet(key_, counter_, dynamic_bitset<>());
		partRet.encry();
		shared_ptr<dynamic_bitset<>> pPartRet = boost::make_shared<dynamic_bitset<>>(*((*pCibitsets_)[i]) ^ partRet.getCipherBits());
		pPlbitsets_->push_back(pPartRet);
		updateCounter_(i);
	}
	assert(pPlbitsets_->size() != 0);
	*((*pPlbitsets_)[pPlbitsets_->size() - 1]) = des::util::bytes_unpaded_PKCS7(*((*pPlbitsets_)[pPlbitsets_->size() - 1]));
	pPltxt_ = des::util::bitsetsToStr(*pPlbitsets_);
	counter_ = IV_;
	return;
}
