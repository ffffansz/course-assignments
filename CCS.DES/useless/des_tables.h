#pragma once
#include <boost/array.hpp>

using namespace boost;

extern const array<unsigned, 16> KEY_GENERATION_SHIFT_TABLE;
extern const array<unsigned, 64> INI_PERMUTATION_TABLE;
extern const array<unsigned, 64> FIN_PERMUTATION_TABLE;
extern const array<unsigned, 48> EXPANSION_TABLE;
extern const array<unsigned, 32> STRAIGHT_PERMUTATION_TABLE;
extern const array<unsigned, 56> PARITY_DROP_TABLE;
extern const array<unsigned, 48> KEY_COMPRESS_TABLE;
extern const unsigned SBOX[8][4][16];
