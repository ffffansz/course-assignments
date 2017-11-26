#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <boost/array.hpp>

using namespace boost;

namespace des {
	namespace table {
		const array<unsigned, 16> KEY_GENERATION_SHIFT_TABLE = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

		const array<unsigned, 64> INI_PERMUTATION_TABLE = { 58, 50, 42, 34, 26, 18, 10, 2,
			60, 52, 44, 36, 28, 20, 12, 4,
			62, 54, 46, 38, 30, 22, 14, 6,
			64, 56, 48, 40, 32, 24, 16, 8,
			57, 49, 41, 33, 25, 17,  9, 1,
			59, 51, 43, 35, 27, 19, 11, 3,
			61, 53, 45, 37, 29, 21, 13, 5,
			63, 55, 47, 39, 31, 23, 15, 7 };

		const array<unsigned, 64> FIN_PERMUTATION_TABLE = { 40, 8, 48, 16, 56, 24, 64, 32,
			39, 7, 47, 15, 55, 23, 63, 31,
			38, 6, 46, 14, 54, 22, 62, 30,
			37, 5, 45, 13, 53, 21, 61, 29,
			36, 4, 44, 12, 52, 20, 60, 28,
			35, 3, 43, 11, 51, 19, 59, 27,
			34, 2, 42, 10, 50, 18, 58, 26,
			33, 1, 41, 9,  49, 17, 57, 25 };

		const array<unsigned, 48> EXPANSION_TABLE = { 32,  1,  2,  3,  4,  5,
			4,  5,  6,  7,  8,  9,
			8,  9, 10, 11, 12, 13,
			12, 13, 14, 15, 16, 17,
			16, 17, 18, 19, 20, 21,
			20, 21, 22, 23, 24, 25,
			24, 25, 26, 27, 28, 29,
			28, 29, 30, 31, 32,  1 };

		const array<unsigned, 32> STRAIGHT_PERMUTATION_TABLE = { 16,  7, 20, 21, 29, 12, 28, 17,
			1, 15, 23, 26,  5, 18, 31, 10,
			2,  8, 24, 14, 32, 27,  3,  9,
			19, 13, 30,  6, 22, 11,  4, 25 };

		//array<array<array<unsigned, 16>, 4>, 8> SBOX = {
		const unsigned SBOX[8][4][16] = {
			//sbox 1
			{
				{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
				{ 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
				{ 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
				{ 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
			},

			//sbox 2
			{
				{ 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
				{ 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
				{ 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
				{ 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },
			},

			//sbox 3
			{
				{ 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
				{ 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
				{ 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
				{ 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
			},

			//sbox 4
			{
				{ 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
				{ 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
				{ 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
				{ 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
			},

			//sbox 5
			{
				{ 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
				{ 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
				{ 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
				{ 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
			},

			//sbox 6
			{
				{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
				{ 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
				{ 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
				{ 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
			},

			//sbox 7
			{
				{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
				{ 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
				{ 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
				{ 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
			},

			//sbox 8
			{
				{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
				{ 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
				{ 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
				{ 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
			}
		};

		const array<unsigned, 56> PARITY_DROP_TABLE = { 57, 49, 41, 33, 25, 17,  9, 1,
			58, 50, 42, 34, 26, 18, 10, 2,
			59, 51, 43, 35, 27, 19, 11, 3,
			60, 52, 44, 36, 63, 55, 47, 39,
			31, 23, 15,  7, 62, 54, 46, 38,
			30, 22, 14,  6, 61, 53, 45, 37,
			29, 21, 13,  5, 28, 20, 12, 4 };

		const array<unsigned, 48> KEY_COMPRESS_TABLE = { 14, 17, 11, 24,  1, 5,
			3, 28, 15,  6, 21, 10,
			23, 19, 12,  4, 26,  8,
			16,  7, 27, 20, 13,  2,
			41, 52, 31, 37, 47, 55,
			30, 40, 51, 45, 33, 48,
			44, 49, 39, 56, 34, 53,
			46, 42, 50, 36, 29, 32 };
	}

	namespace util {
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
	
	class KeyGen
	{
	public:

		std::vector<dynamic_bitset<>> roundKeys;   //ÿ�ֵ���Կ����Կ��СΪ48 bit��vector��СΪ16

		KeyGen(const dynamic_bitset<>& initKey);

		~KeyGen();
	private:
		dynamic_bitset<> initKey_;      //64 bit
		dynamic_bitset<> initKey56_;    //56 bit
		dynamic_bitset<> initKey56_L_;  //initKey56����벿��
		dynamic_bitset<> initKey56_R_;  //initKey56���Ұ벿��

		/* ����ָ��������key */
		dynamic_bitset<> genRoundKey_(unsigned round);
	};

	class DesBlock
	{
	public:
		DesBlock(const dynamic_bitset<>& initKey,
			dynamic_bitset<> plbits = dynamic_bitset<>(),
			dynamic_bitset<> cibits = dynamic_bitset<>());

		void encry();

		void decry();

		void setPlainBits(const dynamic_bitset<>& plbits);

		void setCipherBits(const dynamic_bitset<>& cibits);

		dynamic_bitset<> getPlainBits();

		dynamic_bitset<> getCipherBits();

		void clearPlainBits();

		void clearCipherBits();

		~DesBlock();
	private:
		dynamic_bitset<> plbits_;  //64 bit�����ı���
		dynamic_bitset<> cibits_;  //64 bit�����ı���
		KeyGen keygen_;            //�洢��ÿ�ֵ���Կ��ÿ����Կ��СΪ48 bit
		bool op;                   //����ָ����BlockҪ���еĲ�����op = 0 -> ���ܣ�op = 1 -> ����

								   /* DES������input 32 bit��return 32 bit*/
		dynamic_bitset<>
			desFunction_(const dynamic_bitset<>& bs, unsigned round);

		/* DES����������ּ��ܣ���return 32 bit , 32 bit */
		std::pair<dynamic_bitset<>, dynamic_bitset<>>
			mixer_(const dynamic_bitset<>& lbs, const dynamic_bitset<>& rbs, unsigned round);

		/* SBOX */
		dynamic_bitset<>
			sbox_(const dynamic_bitset<>& bs);
	};
}



