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
	/* ��һ��bitset��Ϊ�����������֣����ҵĳ���Ҫ���� */
	extern std::pair<dynamic_bitset<>, dynamic_bitset<>>
		spliter(const dynamic_bitset<>& ori, unsigned l, unsigned r);

	/* ��һ��bitset��Ϊ���ɸ�n bits�ķ��飨�Ӹߵ��ͣ����������һ�����鲻��n bits */
	extern std::vector<dynamic_bitset<>>
		spliter_n(const dynamic_bitset<>& ori, unsigned n);

	/* ȡһ��bitset����ߵ�nλ */
	extern dynamic_bitset<>
		leftmostBits(const dynamic_bitset<>& ori, unsigned n);

	/* ȡһ��bitset���ҵ�nλ */
	extern dynamic_bitset<>
		rightmostBits(const dynamic_bitset<>& ori, unsigned n);

	/* ������bitset�������� */
	extern dynamic_bitset<>
		combiner(const dynamic_bitset<>& ori_l, const dynamic_bitset<>& ori_r);

	/* ������� Ҫ�������������bitset��size������ͬ���������Ľ�� */
	/* �ԣ�dynamic_bitset����operator ^ (b1, b2) */

	/* ѭ������λ������һ��bitset����ѭ����λnλ��������λ���bitset */
	extern dynamic_bitset<>
		cycLeftShift(const dynamic_bitset<>& ori, unsigned n);

	/* �����λ������һ��bitset������λnλ��Ȼ����һ��m bits��bitset������nλ��� */
	extern dynamic_bitset<>
		padLeftShift(const dynamic_bitset<>& ori, const dynamic_bitset<>& padbs, unsigned n);

	/* ��һ��bitset��ȥ��һ��ָ����λ���Ӹ�λ������СΪ0 */
	extern void
		removeBit(dynamic_bitset<>& ori, unsigned pos);

	/* �û�������Ϊһ��bitset��һ���û�������Ӧ���û��õ���bitset */
	template<size_t N>
	dynamic_bitset<>
		permute(const dynamic_bitset<>& ori, array<unsigned, N> permutationTable);

	/* ��һ��mλ��bitset�����nλ����λ��䣬���ֽ���䣬��m, n������8�ı�����������PKCS7�ı�׼�����������bitset */
	extern dynamic_bitset<>
		bytes_paded_PKCS7(const dynamic_bitset<>& ori, size_t n);

	/* ��һ��*����*������bitsetȥ����䣨��λ��䣬���ֽ���䣩������PKCS7�ı�׼������ȥ�������bitset */
	dynamic_bitset<>
		bytes_unpaded_PKCS7(const dynamic_bitset<>& ori);
}
