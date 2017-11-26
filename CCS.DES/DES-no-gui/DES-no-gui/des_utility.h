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
	/* ��һ��bitset��Ϊ�����������֣����ҵĳ���Ҫ���� */
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

	/* ��һ��bitset��Ϊ���ɸ�n bits�ķ��飨�Ӹߵ��ͣ����������һ�����鲻��n bits */
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

	/* ȡһ��bitset����ߵ�nλ */
	dynamic_bitset<>
		leftmostBits(const dynamic_bitset<>& ori, unsigned n) {
		std::pair<dynamic_bitset<>, dynamic_bitset<>> p = spliter(ori, n, ori.size() - n);
		return p.first;
	}

	/* ȡһ��bitset���ҵ�nλ */	
	dynamic_bitset<>
		rightmostBits(const dynamic_bitset<>& ori, unsigned n) {
		std::pair<dynamic_bitset<>, dynamic_bitset<>> p = spliter(ori, ori.size() - n, n);
		return p.second;
	}

	/* ������bitset�������� */
	dynamic_bitset<>
		combiner(const dynamic_bitset<>& ori_l, const dynamic_bitset<>& ori_r) {
		std::string str1, str2;
		to_string(ori_l, str1);
		to_string(ori_r, str2);
		dynamic_bitset<> ret(str1 + str2);
		return ret;
	}

	/* ������� Ҫ�������������bitset��size������ͬ���������Ľ�� */
	/* �ԣ�dynamic_bitset����operator ^ (b1, b2) */

	/* ѭ������λ������һ��bitset����ѭ����λnλ */
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

	/* �����λ������һ��bitset������λnλ��Ȼ����һ��m bits��bitset������nλ��� */
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

	/* ��һ��bitset��ȥ��һ��ָ����λ���Ӹ�λ������СΪ0 */
	void 
		removeBit(dynamic_bitset<>& ori, unsigned pos) {
		std::string s;
		to_string(ori, s);
		s.erase(s.begin() + pos);
		ori = dynamic_bitset<>(s);
		return;
	}

	/* �û�������Ϊһ��bitset��һ���û�������Ӧ���û��õ���bitset */
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

	/* ��һ��mλ��bitset�����nλ����λ��䣬���ֽ���䣬��m, n������8�ı�����������PKCS7�ı�׼�����������bitset */
	dynamic_bitset<>
		bytes_paded_PKCS7(const dynamic_bitset<>& ori, size_t n) {
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
		bytes_unpaded_PKCS7(const dynamic_bitset<>& ori) {
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
}