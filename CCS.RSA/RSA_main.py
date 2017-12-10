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
    while not (p is not q):
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

    d = pow(e, euler_n - 1, n)

    return (e, n), (d, n)


def str2int(s, encoding='utf_8'):
    str_bytesarray = bytearray(base64.encodebytes(s.encode(encoding=encoding)))
    str_hex = str_bytesarray.hex()
    return int(str_hex, 16)


'''
def int2str(i, encoding='utf_8'):
    int_hexstr = hex(i)
    if len(int_hexstr) % 2 is not 0:
        int_hexstr = int_hexstr[:2] + '0' + int_hexstr[2:]
    int_bytes = [int(int_hexstr[j:j+2], 16) for j in range(2, len(int_hexstr), 2)]
    str_bytesarray = bytearray(int_bytes)
    str_bytes = base64.decodebytes(str_bytesarray)
    ret = str_bytes.decode(encoding=encoding)
    return ret
'''

def int2base64(i):
    i_hexstr = hex(i)
    if len(i_hexstr) % 2 is not 0:
        int_hexstr = i_hexstr[:2] + '0' + i_hexstr[2:]
    return base64.decodebytes(bytearray.fromhex(i_hexstr[2:]))

def RSA_Encry(P, public_key):
    # RSA加密，P是明文字符串，(e, n)是公钥
    # 返回密文字符串
    e = public_key[0]
    n = public_key[1]
    P_int = str2int(P, encoding='utf_8')
    return int2base64(pow(P_int, e, n))


def RSA_Decry(C, private_key):
    # RSA解密，P是明文字符串，(d, n)是私钥
    # 返回明文字符串
    d = private_key[0]
    n = private_key[1]
    C_int = str2int(C)
    #return int2str(pow(C_int, d, n))


if __name__ == '__main__':
    pub_key, pri_key = genKeys(100)
    c = RSA_Encry('hello, 中文', pub_key)
    print(c.decode())