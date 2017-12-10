# !usr/bin/env python
# -*- coding:utf-8 -*-

from RSA_Utility import *
from math import gcd
import random
import base64

PRIMILITY_TEST_TIMES = 10


def genKeys(bitnum):
    # 生成RSA的公钥和私钥
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

    print((e * d) % euler_n)

    return (e, n), (d, n)


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

def RSA_Encry(P, public_key):
    # RSA加密，P是明文字符串，(e, n)是公钥
    # 返回密文字符串
    e = public_key[0]
    n = public_key[1]
    P_int = str2int(P)
    print('Encry P_int:', P_int)
    C_int = pow(P_int, e, n)
    return C_int

def RSA_Decry(C_int, private_key):
    #param C, type int
    # RSA解密，P是密文数字，(d, n)是私钥
    # 返回明文字符串
    d = private_key[0]
    n = private_key[1]
    P_int = pow(C_int, d, n)
    print('Decry P_int:', P_int)
    P_str = valid_int2str(P_int)
    return P_str


if __name__ == '__main__':
    pub_key, pri_key = genKeys(200)
    c = RSA_Encry('你好，我不好，heee', pub_key)
    print(c)
    p = RSA_Decry(c, pri_key)
    print(p)