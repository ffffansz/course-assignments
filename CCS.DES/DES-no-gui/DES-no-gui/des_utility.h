#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/array.hpp>

using namespace boost;

namespace desutil{
	/* 将一个bitset分为左右两个部分，左右的长度要给出 */
	std::pair<dynamic_bitset<>, dynamic_bitset<>>
		spliter(const dynamic_bitset<>& ori, unsigned l, unsigned r) {
		assert(ori.size() == l + r);
		dynamic_bitset<> lbs = dynamic_bitset<>();
		dynamic_bitset<> rbs = dynamic_bitset<>();
		unsigned i = 0;
		for (i = 0; i < l - 1; i++)				rbs.push_back(ori[i]);
		for (; i < ori.size(); i++)				lbs.push_back(ori[i]);
		return std::pair<dynamic_bitset<>, dynamic_bitset<>>(lbs, rbs);
	}

	/* 将一个bitset分为若干个n bits的分组（从高到低），允许最后一个分组不足n bits */
	std::vector<dynamic_bitset<>>
		spliter_n(const dynamic_bitset<>& ori, unsigned n) {
		size_t size = ori.size();
		std::string s;
		to_string(ori, s);
		std::vector<dynamic_bitset<>> ret;
		std::string::iterator its = s.begin();
		unsigned nowpos = 0;
		for (; nowpos + n < s.size(); nowpos += n) {
			ret.push_back(dynamic_bitset<>(std::string(s.begin() + nowpos, s.begin() + nowpos + n)));
		}
		ret.push_back(dynamic_bitset<>(std::string(s.begin() + nowpos, s.end())));
		return ret;
	}

	/* 取一个bitset最左边的n位 */
	dynamic_bitset<>
		leftmostBits(const dynamic_bitset<>& ori, unsigned n) {
		std::pair<dynamic_bitset<>, dynamic_bitset<>> p = spliter(ori, n, ori.size() - n);
		return p.first;
	}

	/* 取一个bitset最右的n位 */	
	dynamic_bitset<>
		rightmostBits(const dynamic_bitset<>& ori, unsigned n) {
		std::pair<dynamic_bitset<>, dynamic_bitset<>> p = spliter(ori, ori.size() - n, n);
		return p.second;
	}

	/* 将两个bitset连接起来 */
	dynamic_bitset<>
		combiner(const dynamic_bitset<>& ori_l, const dynamic_bitset<>& ori_r) {
		std::string str1, str2;
		to_string(ori_l, str1);
		to_string(ori_r, str2);
		dynamic_bitset<> ret(str1 + str2);
		return ret;
	}

	/* 异或器， 要求进行异或的两个bitset的size必须相同，返回异或的结果 */
	/* 略，dynamic_bitset里有operator ^ (b1, b2) */

	/* 循环左移位器，将一个bitset向左循环移位n位 */
	void
		cycLeftShift(dynamic_bitset<>& ori, unsigned n) {
		n %= ori.size();
		assert(n <= ori.size());
		std::string s;
		to_string(ori, s);
		std::string substr1(s.begin(), s.begin() + n);
		std::string substr2(s.begin() + n, s.end());
		ori = dynamic_bitset<>(substr2 + substr1);
		return;
	}

	/* 填充移位器，将一个bitset向左移位n位，然后用一个m bits的bitset的最左n位填充 */
	void
		padLeftShift(dynamic_bitset<>& ori, const dynamic_bitset<>& padbs, unsigned n) {
		assert(n <= padbs.size());
		std::string s1, s2;
		to_string(ori, s1);
		to_string(padbs, s2);
		std::string substr1(s1.begin() + n, s1.end());
		std::string substr2(s2.begin(), s2.begin() + n);
		ori = dynamic_bitset<>(substr1 + substr2);
		return;
	}

	/* 从一个bitset中去掉一个指定的位，从高位数，最小为0 */
	void 
		removeBit(dynamic_bitset<>& ori, unsigned pos) {
		std::string s;
		to_string(ori, s);
		s.erase(s.begin() + pos);
		ori = dynamic_bitset<>(s);
		return;
	}

	/* 置换，输入为一个bitset和一个置换表，返回应用置换得到的bitset */
	template<size_t N>
	dynamic_bitset<>
		permute(const dynamic_bitset<>& ori, array<unsigned, N> permutationTable) {
		size_t tableSize = permutationTable.size();
		size_t oribsSize = ori.size();
		dynamic_bitset<> ret(tableSize);
		for (size_t i = 0; i < tableSize; i++) {
			assert(oribsSize - permutationTable[i] < oribsSize);
			ret[tableSize - 1 - i] = ori[oribsSize - permutationTable[i]];
		}
		return ret;
	}

	/* 将一个m位的bitset填充至n位（低位填充，按字节填充，即m, n必须是8的倍数），按照PKCS7的标准，返回填充后的bitset */
	dynamic_bitset<>
		bytes_paded_PKCS7(const dynamic_bitset<>& ori, size_t n) {
		size_t oriSize = ori.size();
		assert(oriSize <= n);
		assert(oriSize % 8 == 0);
		assert(n % 8 == 0);
		std::string str;
		to_string(ori, str);
		unsigned byteCnt = (n - oriSize) / 8;
		std::bitset<8> tmpStdBs(byteCnt);   // 将要填充的字节数转化为二进制
		for (unsigned i = 0; i < byteCnt; i++) {
			str += tmpStdBs.to_string();
		}
		return dynamic_bitset<>(str);
	}

	/* 将一个*可能*填充过的bitset去掉填充（低位填充，按字节填充），按照PKCS7的标准，返回去掉填充后的bitset */
	dynamic_bitset<>
		bytes_unpaded_PKCS7(const dynamic_bitset<>& ori) {
		assert(ori.size() % 8 == 0);
		std::vector<dynamic_bitset<>> vec = spliter_n(ori, 8); // 按字节划分
		unsigned sameByteCnt = 0;
		std::vector<dynamic_bitset<>>::reverse_iterator rit = vec.rbegin(); // 从后往前
		for (; rit != vec.rend() - 1; rit++) {
			if (*rit == *(rit + 1))
				sameByteCnt++;
			else
				break;        // 反向迭代器指向最后一个相同的byte
		}
		sameByteCnt++;
		std::bitset<8> padByte(sameByteCnt);
		std::string str;
		to_string(vec[vec.size() - 1], str);
		if (padByte.to_string() == str) {  // 符合PKCS7的填充规则
			std::string bsHead;
			to_string(ori, bsHead);
			bsHead.erase(bsHead.begin() + (bsHead.size() - sameByteCnt * 8), bsHead.end());
			return dynamic_bitset<>(bsHead);
		}
		return ori;
	}
}