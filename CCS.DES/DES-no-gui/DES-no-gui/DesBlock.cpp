#include "DesBlock.h"


/* ��һ��bitset��Ϊ�����������֣����ҵĳ���Ҫ���� */
std::pair<dynamic_bitset<>, dynamic_bitset<>>
	des::util::spliter(const dynamic_bitset<>& ori, unsigned l, unsigned r) {
	assert(ori.size() == l + r);
	dynamic_bitset<> lbs = dynamic_bitset<>();
	dynamic_bitset<> rbs = dynamic_bitset<>();
	unsigned i = 0;
	for (i = 0; i < r; i++)				    rbs.push_back(ori[i]);
	for (; i < ori.size(); i++)				lbs.push_back(ori[i]);
	return std::pair<dynamic_bitset<>, dynamic_bitset<>>(lbs, rbs);
}

/* ��һ��bitset��Ϊ���ɸ�n bits�ķ��飨�Ӹߵ��ͣ����������һ�����鲻��n bits */
std::vector<dynamic_bitset<>>
des::util::spliter_n(const dynamic_bitset<>& ori, unsigned n) {
	size_t size = ori.size();
	std::string s;
	to_string(ori, s);
	std::vector<dynamic_bitset<>> ret;
	unsigned nowpos = 0;
	for (; nowpos + n < s.size(); nowpos += n) {
		ret.push_back(dynamic_bitset<>(std::string(s.begin() + nowpos, s.begin() + nowpos + n)));
	}
	ret.push_back(dynamic_bitset<>(std::string(s.begin() + nowpos, s.end())));
	return ret;
}

/* ȡһ��bitset����ߵ�nλ */
dynamic_bitset<>
des::util::leftmostBits(const dynamic_bitset<>& ori, unsigned n) {
	std::pair<dynamic_bitset<>, dynamic_bitset<>> p = spliter(ori, n, ori.size() - n);
	return p.first;
}

/* ȡһ��bitset���ҵ�nλ */
dynamic_bitset<>
des::util::rightmostBits(const dynamic_bitset<>& ori, unsigned n) {
	std::pair<dynamic_bitset<>, dynamic_bitset<>> p = spliter(ori, ori.size() - n, n);
	return p.second;
}

/* ������bitset�������� */
dynamic_bitset<>
des::util::combiner(const dynamic_bitset<>& ori_l, const dynamic_bitset<>& ori_r) {
	std::string str1, str2;
	to_string(ori_l, str1);
	to_string(ori_r, str2);
	dynamic_bitset<> ret(str1 + str2);
	return ret;
}

/* ������� Ҫ�������������bitset��size������ͬ���������Ľ�� */
/* �ԣ�dynamic_bitset����operator ^ (b1, b2) */

/* ѭ������λ������һ��bitset����ѭ����λnλ��������λ���bitset */
dynamic_bitset<>
des::util::cycLeftShift(const dynamic_bitset<>& ori, unsigned n) {
	n %= ori.size();
	assert(n <= ori.size());
	std::string s;
	to_string(ori, s);
	std::string substr1(s.begin(), s.begin() + n);
	std::string substr2(s.begin() + n, s.end());

	return dynamic_bitset<>(substr2 + substr1);
}

/* �����λ������һ��bitset������λnλ��Ȼ����һ��m bits��bitset������nλ��� */
dynamic_bitset<>
des::util::padLeftShift(const dynamic_bitset<>& ori, const dynamic_bitset<>& padbs, unsigned n) {
	assert(n <= padbs.size());
	std::string s1, s2;
	to_string(ori, s1);
	to_string(padbs, s2);
	std::string substr1(s1.begin() + n, s1.end());
	std::string substr2(s2.begin(), s2.begin() + n);

	return dynamic_bitset<>(substr1 + substr2);
}

/* ��һ��bitset��ȥ��һ��ָ����λ���Ӹ�λ������СΪ0 */
void
des::util::removeBit(dynamic_bitset<>& ori, unsigned pos) {
	std::string s;
	to_string(ori, s);
	s.erase(s.begin() + pos);
	ori = dynamic_bitset<>(s);
	return;
}

/* �û�������Ϊһ��bitset��һ���û�������Ӧ���û��õ���bitset */
template<size_t N>
dynamic_bitset<>
des::util::permute(const dynamic_bitset<>& ori, array<unsigned, N> permutationTable) {
	size_t tableSize = permutationTable.size();
	size_t oribsSize = ori.size();
	dynamic_bitset<> ret(tableSize);
	for (size_t i = 0; i < tableSize; i++) {
		assert(oribsSize - permutationTable[i] < oribsSize);
		ret[tableSize - 1 - i] = ori[oribsSize - permutationTable[i]];
	}
	return ret;
}

/* ��һ��mλ��bitset�����nλ����λ��䣬���ֽ���䣬��m, n������8�ı�����������PKCS7�ı�׼�����������bitset */
dynamic_bitset<>
des::util::bytes_paded_PKCS7(const dynamic_bitset<>& ori, size_t n) {
	size_t oriSize = ori.size();
	assert(oriSize <= n);
	assert(oriSize % 8 == 0);
	assert(n % 8 == 0);
	std::string str;
	to_string(ori, str);
	unsigned byteCnt = (n - oriSize) / 8;
	std::bitset<8> tmpStdBs(byteCnt);   // ��Ҫ�����ֽ���ת��Ϊ������
	for (unsigned i = 0; i < byteCnt; i++) {
		str += tmpStdBs.to_string();
	}
	return dynamic_bitset<>(str);
}

/* ��һ��*����*������bitsetȥ����䣨��λ��䣬���ֽ���䣩������PKCS7�ı�׼������ȥ�������bitset */
dynamic_bitset<>
des::util::bytes_unpaded_PKCS7(const dynamic_bitset<>& ori) {
	assert(ori.size() % 8 == 0);
	std::vector<dynamic_bitset<>> vec = spliter_n(ori, 8); // ���ֽڻ���
	unsigned sameByteCnt = 0;
	std::vector<dynamic_bitset<>>::reverse_iterator rit = vec.rbegin(); // �Ӻ���ǰ
	for (; rit != vec.rend() - 1; rit++) {
		if (*rit == *(rit + 1))
			sameByteCnt++;
		else
			break;        // ���������ָ�����һ����ͬ��byte
	}
	sameByteCnt++;
	std::bitset<8> padByte(sameByteCnt);
	std::string str;
	to_string(vec[vec.size() - 1], str);
	if (padByte.to_string() == str) {  // ����PKCS7��������
		std::string bsHead;
		to_string(ori, bsHead);
		bsHead.erase(bsHead.begin() + (bsHead.size() - sameByteCnt * 8), bsHead.end());
		return dynamic_bitset<>(bsHead);
	}
	return ori;
}

/* ��һ��string�ֽ�����ɸ�64 bit��bitset���������һ��bitset����64λ��vec[0]��string��ߵ�64λ������ĳһ��bitset��˵��bs[0]��64λ����͵���һλ */
shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>>
des::util::strToBitsets(const std::string & str)
{
	shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>> ret = make_shared<std::vector<shared_ptr<dynamic_bitset<>>>>();
	if (str.size() == 0)
		return shared_ptr<std::vector<shared_ptr<dynamic_bitset<>>>>();
	assert(sizeof(str[0]) == 1);
	size_t byteSize = str.size(); //������
	unsigned now8Byte = 0;
	for (; 8 * now8Byte + 8 < byteSize; now8Byte ++) {  //ÿ8���ֽ�Ϊһ����λ
		shared_ptr<dynamic_bitset<>> pBs = make_shared<dynamic_bitset<>>(64);
		for (unsigned i = 0; i < 8; i++) {
			for (unsigned j = 0; j < 8; j++) {
				(*pBs)[i * 8 + j] = ((str[8 * now8Byte + i] >> j) & 1);  //��λ�����Ϣ���ӵ͵���
			}
		}
		ret->push_back(pBs);
	}
	shared_ptr<dynamic_bitset<>> pLastBs = make_shared<dynamic_bitset<>>();
	for (unsigned i = 8 * now8Byte; i < byteSize; i++) {
		for (unsigned j = 0; j < 8; j++) {
			pLastBs->push_back((str[i] >> j) & 1);
		}
	}
	ret->push_back(pLastBs);
	return ret;
}

shared_ptr<std::string> des::util::bitsetsToStr(const std::vector<shared_ptr<dynamic_bitset<>>>& bss)
{
	shared_ptr<std::string> pRet = make_shared<std::string>();
	for (size_t i = 0; i < bss.size(); i++) {
		std::string partRet;
		for (unsigned j = 0; j < bss[i]->size() / 8; j++) {
			char c = 0x00;
			for (unsigned k = 0; k < 8; k++) {
				c += ((*bss[i])[8 * j + k]) << k;
			}
			partRet += c;
		}
		*pRet += partRet;
	}
	return pRet;
}



des::DesBlock::DesBlock(const dynamic_bitset<>& initKey, dynamic_bitset<> plbits, dynamic_bitset<> cibits)
	: plbits_(plbits), cibits_(cibits), keygen_(initKey)
{
	assert(!initKey.empty() && (!plbits.empty() || !cibits.empty()));
	if (plbits.empty() && !cibits.empty())	op = true;
	else op = false;
}

void des::DesBlock::encry()
{
	assert(op == 0);  //����
	dynamic_bitset<> afterInitPert = des::util::permute(plbits_, des::table::INI_PERMUTATION_TABLE);
	std::pair<dynamic_bitset<>, dynamic_bitset<>> bsp = des::util::spliter(afterInitPert, 32, 32);
	for (unsigned round = 0; round < 16; round++) {
		bsp.first ^= desFunction_(bsp.second, round);
		if (round != 15)	swap(bsp.first, bsp.second);
	}
	dynamic_bitset<> ret = des::util::combiner(bsp.first, bsp.second);
	cibits_ = des::util::permute(ret, des::table::FIN_PERMUTATION_TABLE);
	return;
}

void des::DesBlock::decry()
{
	assert(op == 1);  //����
	dynamic_bitset<> afterInitPert = des::util::permute(cibits_, des::table::INI_PERMUTATION_TABLE);
	std::pair<dynamic_bitset<>, dynamic_bitset<>> bsp = des::util::spliter(afterInitPert, 32, 32);
	for (unsigned round = 0; round < 16; round++) {
		bsp.first ^= desFunction_(bsp.second, 15 - round);  // ע����ܺͽ���ʹ��Key��˳�������෴
		if (round != 15)	swap(bsp.first, bsp.second);
	}
	dynamic_bitset<> ret = des::util::combiner(bsp.first, bsp.second);
	plbits_ = des::util::permute(ret, des::table::FIN_PERMUTATION_TABLE);
	return;
}

void des::DesBlock::setPlainBits(const dynamic_bitset<>& plbits)
{
	plbits_ = plbits;
	return;
}

void des::DesBlock::setCipherBits(const dynamic_bitset<>& cibits)
{
	cibits_ = cibits;
	return;
}

dynamic_bitset<> des::DesBlock::getPlainBits()
{
	return plbits_;
}

dynamic_bitset<> des::DesBlock::getCipherBits()
{
	return cibits_;
}

void des::DesBlock::clearPlainBits()
{
	plbits_.clear();
	return;
}

void des::DesBlock::clearCipherBits()
{
	cibits_.clear();
	return;
}

des::DesBlock::~DesBlock()
{
}

dynamic_bitset<> des::DesBlock::desFunction_(const dynamic_bitset<>& bs, unsigned round)
{
	dynamic_bitset<> ret = des::util::permute(bs, des::table::EXPANSION_TABLE);  //input 32 bit, output 48 bit
	ret ^= keygen_.roundKeys[round];
	ret = sbox_(ret);  //input 48 bit, output 32 bit
	ret = des::util::permute(ret, des::table::STRAIGHT_PERMUTATION_TABLE);
	return ret;
}

std::pair<dynamic_bitset<>, dynamic_bitset<>> des::DesBlock::mixer_(const dynamic_bitset<>& lbs, const dynamic_bitset<>& rbs, unsigned round)
{
	return std::pair<dynamic_bitset<>, dynamic_bitset<>>();
}

dynamic_bitset<> des::DesBlock::sbox_(const dynamic_bitset<>& bs)
{
	std::vector<dynamic_bitset<>> vec = des::util::spliter_n(bs, 6);  //�ֳ�8��6 bit��bitset
	std::string retstr;
	std::vector<dynamic_bitset<>>::iterator it = vec.begin();
	unsigned boxCnt = 0;
	for (; it != vec.end(); it++, boxCnt++) {
		std::string str;
		to_string(*it, str);
		dynamic_bitset<> rowbs(std::string() + str[0] + str[5]);
		dynamic_bitset<> colbs(std::string(str.begin() + 1, str.end() - 1));
		std::bitset<4> part(des::table::SBOX[boxCnt][rowbs.to_ulong()][colbs.to_ulong()]);
		retstr += part.to_string();
	}

	return dynamic_bitset<>(retstr);
}

des::KeyGen::KeyGen(const dynamic_bitset<>& initKey)
	: initKey_(initKey)
{
	initKey56_ = des::util::permute(initKey_, des::table::PARITY_DROP_TABLE);
	std::pair<dynamic_bitset<>, dynamic_bitset<>> p = des::util::spliter(initKey56_, 28, 28);
	initKey56_L_ = p.first;
	initKey56_R_ = p.second;
	for (unsigned i = 0; i < 16; i++)
		roundKeys.push_back(genRoundKey_(i));
}

des::KeyGen::~KeyGen()
{
}

dynamic_bitset<> des::KeyGen::genRoundKey_(unsigned round)
{
	unsigned shiftCnt = 0;
	for (unsigned i = 0; i < round + 1; i++) {
		shiftCnt += des::table::KEY_GENERATION_SHIFT_TABLE[i];
	}
	dynamic_bitset<> ret_l = des::util::cycLeftShift(initKey56_L_, shiftCnt);
	dynamic_bitset<> ret_r = des::util::cycLeftShift(initKey56_R_, shiftCnt);
	dynamic_bitset<> ret = des::util::combiner(ret_l, ret_r);
	return des::util::permute(ret, des::table::KEY_COMPRESS_TABLE);
}

