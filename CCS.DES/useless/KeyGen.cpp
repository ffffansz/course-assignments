#include "DesBlock.h"

des::KeyGen::KeyGen(const dynamic_bitset<>& initKey)
	: initKey_(initKey)
{
	initKey56_ = des::util::permute(initKey_, des::table::PARITY_DROP_TABLE);
	std::pair<dynamic_bitset<>, dynamic_bitset<>> p = des::util::spliter(initKey56_, 28, 28);
	initKey56_L_ = p.first;
	initKey56_L_ = p.second;
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
