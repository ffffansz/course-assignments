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

namespace desutil {
	/* 将一个bitset分为左右两个部分，左右的长度要给出 */
	extern std::pair<dynamic_bitset<>, dynamic_bitset<>>
		spliter(const dynamic_bitset<>& ori, unsigned l, unsigned r);

	/* 将一个bitset分为若干个n bits的分组（从高到低），允许最后一个分组不足n bits */
	extern std::vector<dynamic_bitset<>>
		spliter_n(const dynamic_bitset<>& ori, unsigned n);

	/* 取一个bitset最左边的n位 */
	extern dynamic_bitset<>
		leftmostBits(const dynamic_bitset<>& ori, unsigned n);

	/* 取一个bitset最右的n位 */
	extern dynamic_bitset<>
		rightmostBits(const dynamic_bitset<>& ori, unsigned n);

	/* 将两个bitset连接起来 */
	extern dynamic_bitset<>
		combiner(const dynamic_bitset<>& ori_l, const dynamic_bitset<>& ori_r);

	/* 异或器， 要求进行异或的两个bitset的size必须相同，返回异或的结果 */
	/* 略，dynamic_bitset里有operator ^ (b1, b2) */

	/* 循环左移位器，将一个bitset向左循环移位n位，返回移位后的bitset */
	extern dynamic_bitset<>
		cycLeftShift(const dynamic_bitset<>& ori, unsigned n);

	/* 填充移位器，将一个bitset向左移位n位，然后用一个m bits的bitset的最左n位填充 */
	extern dynamic_bitset<>
		padLeftShift(const dynamic_bitset<>& ori, const dynamic_bitset<>& padbs, unsigned n);

	/* 从一个bitset中去掉一个指定的位，从高位数，最小为0 */
	extern void
		removeBit(dynamic_bitset<>& ori, unsigned pos);

	/* 置换，输入为一个bitset和一个置换表，返回应用置换得到的bitset */
	template<size_t N>
	dynamic_bitset<>
		permute(const dynamic_bitset<>& ori, array<unsigned, N> permutationTable);

	/* 将一个m位的bitset填充至n位（低位填充，按字节填充，即m, n必须是8的倍数），按照PKCS7的标准，返回填充后的bitset */
	extern dynamic_bitset<>
		bytes_paded_PKCS7(const dynamic_bitset<>& ori, size_t n);

	/* 将一个*可能*填充过的bitset去掉填充（低位填充，按字节填充），按照PKCS7的标准，返回去掉填充后的bitset */
	dynamic_bitset<>
		bytes_unpaded_PKCS7(const dynamic_bitset<>& ori);
}
