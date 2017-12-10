# !usr/bin/env python
# -*- coding:utf-8 -*-

import random
from math import gcd

# 100以内的素数，用于整除性检验
SMALL_PRIME = (3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97)

# Miller-Rabin素性测试次数
PRIMILITY_TEST_TIMES = 10


def genRandomBitNum(n):
    # 生成一个n-bits的随机奇数
    # 要确保产生的随机数为n-bits的奇数，最高位和最低位必须为1
    # 其余部分随机填充0或1即可
    # 所以只需要产生一个长度为n-2的比特串即可
    sub_bitset = ''
    for i in range(n - 2):
        if random.uniform(0, 1) >= 0.5:
            sub_bitset += '1'
        else:
            sub_bitset += '0'
    return int('0b1' + sub_bitset + '1', 2)


def MillerRabinTest(n, base):
    # Miller - Rabin素性测试

    # 将 n - 1 分解为 m * 2^k
    k = 1
    while (n - 1) % pow(2, k + 1) == 0:
        k += 1
    m = (n - 1) // pow(2, k)

    T = pow(base, m, n)
    if T == 1 or T == n - 1:
        return True

    for i in range(k - 1):
        T = pow(T, 2, n)
        if T == 1:
            return False
        elif T == n - 1:
            return True

    return False


def primalityTest(n, m):
    # 测试一个n是否为素数，使用整除性检验 + Miller - Rabin
    # 非素数通过测试的概率为(1 / 4) ^ m（即执行m次Miller - Rabin素性测试）

    # 用100以内的素数进行整除性检验，确保n不是一个明显的复合数
    for p in SMALL_PRIME:
        if n % p == 0:
            return False

    # 进行m次Miller - Rabin素性测试
    for base in range(2, m + 2):
        if not MillerRabinTest(n, base):
            return False

    return True


def genBigPrime(n, m):
    # 产生一个n-bits的大素数，返回非素数的概率为(1/4)^m
    while True:
        randomOddNum = genRandomBitNum(n)
        if primalityTest(randomOddNum, m):
            return randomOddNum


# Calculate the multiplicative inverse of the given number in the given domain
def calMulInverseNum(num, domain):
    r1, r2 = domain, num
    t1, t2 = 0, 1
    r, t = None, None
    while r2 != 0:
        q = r1 // r2
        r = r1 - q * r2
        t = t1 - q * t2
        r1 = r2
        r2 = r
        t1 = t2
        t2 = t
    if r1 == 1:
        if (t1 < 0):
            return t1 + domain
        return t1
    else:
        return domain + 1


def genKeys(bitnum):
    '''
    :param bitnum:
    :return: Private key and public key ((e, n) and (d, n))

    bit number of p and q is both bitnum
    bit number of n is bitnum * 2

    '''
    p = None
    q = None
    while p is q:
        p = genBigPrime(bitnum, PRIMILITY_TEST_TIMES)
        q = genBigPrime(bitnum, PRIMILITY_TEST_TIMES)

    n = p * q
    euler_n = (p - 1) * (q - 1)
    e = None
    e_prime = False
    while not e_prime:
        e = random.randint(2, euler_n - 1)
        if gcd(e, euler_n) == 1:
            e_prime = True

    d = calMulInverseNum(e, euler_n)

    return e, d, n, p, q


def str2int(str_):
    str_utf8byte = str_.encode()
    str_ba = bytearray(str_utf8byte)
    str_hex = str_ba.hex()
    str_int = int(str_hex, 16)
    return str_int


def valid_int2str(int_):
    int_hexstr = hex(int_)
    if len(int_hexstr) % 2 is not 0:
        int_hexstr = int_hexstr[:2] + '0' + int_hexstr[2:]
    int_hexbytes = [int(int_hexstr[i:i+2], 16) for i in range(2, len(int_hexstr), 2)]
    int_bytearray = bytearray(int_hexbytes)
    int_str = int_bytearray.decode()
    return int_str


def RSA_basic_Encry(P, public_key):
    # RSA加密，P是明文字符串，(e, n)是公钥
    # 返回密文数字
    e = public_key[0]
    n = public_key[1]
    P_int = str2int(P)
    # print('Encry P_int:', P_int)
    C_int = pow(P_int, e, n)
    return C_int


def RSA_basic_Decry(C_int, private_key):
    # RSA解密，C_int是密文数字，(d, n)是私钥
    # 返回明文字符串
    d = private_key[0]
    n = private_key[1]
    P_int = pow(C_int, d, n)
    # print('Decry P_int:', P_int)
    P_str = valid_int2str(P_int)
    return P_str


# Split too long str into sub-str
def RSA_partition(lstr, key_bitnum=1024):
    # UTF8编码下每个汉字占3 byte
    # 子字符串的最大byte数为key_bitnum/8
    # 考虑到兼容中文，每个子字符串的长度应不超过key_bitnum/24
    # 因此每个子字符串长度取key_bitnum/32 (方便整除)
    assert key_bitnum >= 32, 'The length of key is too short.'
    sub_str_len = key_bitnum // 32
    if len(lstr) <= sub_str_len:
        return lstr,                # return a tuple
    sub_strs = tuple([lstr[i:i + sub_str_len] for i in range(0, len(lstr), sub_str_len)])
    return sub_strs


# Combine the sub-str to a long str
def RSA_Combine(sub_strs):           # @param sub_strs type: tuple
    pass


def RSA_encry(plain_str, public_key):
    '''
    :param plain_str:
    :param public_key:
    :return: str
    '''
    # e, n = public_key
    key_bitnum = len(bin(public_key[1])) - 2
    sub_strs = RSA_partition(plain_str, key_bitnum)
    ret_intlist = []
    for sub_str in sub_strs:
        part_ret = RSA_basic_Encry(sub_str, public_key)
        ret_intlist.append(part_ret)

    # add 'p' at the end of each sub-str for splitting the cipher str when decrypting
    ret_strlist = [str(i)+'p' for i in ret_intlist]
    ret_strlist[-1] = ret_strlist[-1][:-1]            # delete the last added 'p'
    return ''.join(ret_strlist)


def RSA_decry(cipher_str, private_key):
    '''

    :param cipher_str:
    :param private_key:
    :return: str
    '''
    key_bitnum = len(bin(private_key[1])) - 2
    cipher_ints = list(map(int, cipher_str.split('p')))
    ret_strlist = []
    for ci in cipher_ints:
        part_ret = RSA_basic_Decry(ci, private_key)
        ret_strlist.append(part_ret)
    # ret_strlist = [valid_int2str(i) for i in ret_intlist]
    return ''.join(ret_strlist)


if __name__ == '__main__':
    s = '今天很不幸，被我们老大challenge了。他问Wo是否跑完了所有的CASE，是否知道在系统中中文字符占用3个字节。就我所知道的情况，中文字符只暂用两个字节，DEV也如是这样说。'
    key = genKeys(512)
    pub_key = key[0]
    pri_key = key[1]
    c = RSA_encry(s, pub_key)
    p = RSA_decry(c, pri_key)
    print(c)
    assert s == p
    print(p)