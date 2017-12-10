# !usr/bin/env python
# -*- coding:utf-8 -*-

import random

# 100以内的素数，用于整除性检验
small_prime = (3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97)


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

    for p in small_prime:
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
