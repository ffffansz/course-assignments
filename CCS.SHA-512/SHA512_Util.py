# !usr/bin/env python
# -*- coding:utf-8 -*-

from copy import deepcopy


class SHA512_CONST:

    K = (
        0x428A2F98D728AE22, 0x7137449123EF65CD,
        0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
        0x3956C25BF348B538, 0x59F111F1B605D019,
        0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
        0xD807AA98A3030242, 0x12835B0145706FBE,
        0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
        0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1,
        0x9BDC06A725C71235, 0xC19BF174CF692694,
        0xE49B69C19EF14AD2, 0xEFBE4786384F25E3,
        0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
        0x2DE92C6F592B0275, 0x4A7484AA6EA6E483,
        0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
        0x983E5152EE66DFAB, 0xA831C66D2DB43210,
        0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
        0xC6E00BF33DA88FC2, 0xD5A79147930AA725,
        0x06CA6351E003826F, 0x142929670A0E6E70,
        0x27B70A8546D22FFC, 0x2E1B21385C26C926,
        0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
        0x650A73548BAF63DE, 0x766A0ABB3C77B2A8,
        0x81C2C92E47EDAEE6, 0x92722C851482353B,
        0xA2BFE8A14CF10364, 0xA81A664BBC423001,
        0xC24B8B70D0F89791, 0xC76C51A30654BE30,
        0xD192E819D6EF5218, 0xD69906245565A910,
        0xF40E35855771202A, 0x106AA07032BBD1B8,
        0x19A4C116B8D2D0C8, 0x1E376C085141AB53,
        0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
        0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB,
        0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
        0x748F82EE5DEFB2FC, 0x78A5636F43172F60,
        0x84C87814A1F0AB72, 0x8CC702081A6439EC,
        0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9,
        0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
        0xCA273ECEEA26619C, 0xD186B8C721C0C207,
        0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
        0x06F067AA72176FBA, 0x0A637DC5A2C898A6,
        0x113F9804BEF90DAE, 0x1B710B35131C471B,
        0x28DB77F523047D84, 0x32CAAB7B40C72493,
        0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
        0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A,
        0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817
        )

    IV = {
        'a': 0x6A09E667F3BCC908,
        'b': 0xBB67AE8584CAA73B,
        'c': 0x3C6EF372FE94F82B,
        'd': 0xA54FF53A5F1D36F1,
        'e': 0x510E527FADE682D1,
        'f': 0x9B05688C2B3E6C1F,
        'g': 0x1F83D9ABFB41BD6B,
        'h': 0x5BE0CD19137E2179}


class SHA512:
    def __init__(self, ctx):
        self.reg0 = deepcopy(SHA512_CONST.IV)
        self.reg1 = deepcopy(SHA512_CONST.IV)
        self.ctx = ctx
        self.padedBinCtx = self.pad_()
        self.summary = ''            # 16进制形式的字符串

    def sha(self):
        blockNum = len(self.padedBinCtx) // 1024
        blockIdx = 0
        print(blockNum)
        while blockIdx < blockNum:

            self.process_(blockIdx)
            for k in self.reg1:
                if self.reg1[k] + self.reg0[k] >= pow(2, 64):
                    #print('debug')
                    pass
                self.reg1[k] = (self.reg1[k] + self.reg0[k]) % pow(2, 64)
                self.reg0[k] = self.reg1[k]

            blockIdx += 1

        self.reg2summary_()

    def process_(self, blockIdx):
        # 处理第blockIdx个1024位的分组
        localBits = self.padedBinCtx[blockIdx*1024:(blockIdx+1)*1024]

        # --- 以下操作均按照int类型处理 --- #

        # 初始化W
        W = [int(localBits[j:j+64], 2) for j in range(0, 1024, 64)]    # 前16个
        for t in range(16, 80):
            factors = [littleSigma1(W[t-2]), W[t-7], littleSigma0(W[t-15]), W[t-16]]
            W.append(modSum64(factors))

        roundCnt = 0
        while roundCnt < 80:
            t1_factors = [
                self.reg1['h'],
                Ch(self.reg1['e'], self.reg1['f'], self.reg1['g']),
                bigSigma1(self.reg1['e']),
                W[roundCnt],
                SHA512_CONST.K[roundCnt]
            ]
            t1 = modSum64(t1_factors)

            t2_factors = [
                bigSigma0(self.reg1['a']),
                Maj(self.reg1['a'], self.reg1['b'], self.reg1['c'])
            ]
            t2 = modSum64(t2_factors)

            self.reg1['h'] = self.reg1['g']
            self.reg1['g'] = self.reg1['f']
            self.reg1['f'] = self.reg1['e']
            self.reg1['e'] = (self.reg1['d'] + t1) % pow(2, 64)
            self.reg1['d'] = self.reg1['c']
            self.reg1['c'] = self.reg1['b']
            self.reg1['b'] = self.reg1['a']
            self.reg1['a'] = (t1 + t2) % pow(2, 64)

            roundCnt += 1

    def pad_(self):
        binCtx = str2bin(self.ctx)
        ctxLen = len(binCtx)
        padLen = 896 - ctxLen % 1024
        padMsg = int2bin(pow(2, padLen-1))  # 一个1和padLen-1个0
        oriLenMsg = int2bin(ctxLen)  # 原始长度
        padedOriLenMsg = genZeroStr(128 - len(oriLenMsg)) + oriLenMsg
        return binCtx + (padMsg + padedOriLenMsg)

    def reg2summary_(self):
        def int264bin(x):
            x_short_bin = int2bin(x)
            x_bin = genZeroStr(64-int(len(x_short_bin))) + x_short_bin
            return x_bin

        self.summary += int264bin(self.reg1['a'])
        self.summary += int264bin(self.reg1['b'])
        self.summary += int264bin(self.reg1['c'])
        self.summary += int264bin(self.reg1['d'])
        self.summary += int264bin(self.reg1['e'])
        self.summary += int264bin(self.reg1['f'])
        self.summary += int264bin(self.reg1['g'])
        self.summary += int264bin(self.reg1['h'])

        self.summary = int(self.summary, 2)
        self.summary = hex(self.summary)[2:]


def int2bin(n):
    return bin(n)[2:]


def int2hex(n):
    return hex(n)[2:]


def genZeroStr(n):
    # 返回由n个0组成的字符串
    if n is 0:
        return ''
    return bin(pow(2, n))[3:]


def str2bin(str_):
    str_utf8byte = str_.encode()
    str_ba = bytearray(str_utf8byte)
    str_hex = str_ba.hex()
    str_int = int(str_hex, 16)
    str_bin = int2bin(str_int)

    # 如果str_bin的长度不是8的倍数，则意味着高位的0的被省略了，要补上
    if len(str_bin) % 8 is not 0:
        str_bin = genZeroStr(8-len(str_bin) % 8) + str_bin
    return str_bin


def ROTR(x, n):
    x_str = int2bin(x)
    if len(x_str) is not 64:
        x_str = genZeroStr(64 - len(x_str)) + x_str
    ret_str = x_str[len(x_str) - n:] + x_str[:len(x_str) - n]
    return int(ret_str, 2)


def SHR(x, n):
    x_str = int2bin(x)
    if len(x_str) is not 64:
        x_str = genZeroStr(64 - len(x_str)) + x_str
    ret_str = genZeroStr(n) + x_str[:len(x_str) - n]
    return int(ret_str, 2)


def modSum64(nums):
    # return the sum of a list of int (mod pow(2, 64))
    s = sum(nums)
    return s % pow(2, 64)


def bigSigma0(x):
    ret_int = ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39)
    return ret_int


def bigSigma1(x):
    ret_int = ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41)
    return ret_int


def littleSigma0(x):
    ret_int = ROTR(x, 1) ^ ROTR(x, 8) ^ SHR(x, 7)
    return ret_int


def littleSigma1(x):
    ret_int = ROTR(x, 19) ^ ROTR(x, 61) ^ SHR(x, 6)
    return ret_int


def Ch(e, f, g):
    ret_int = (e & f) ^ (~e & g)
    return ret_int


def Maj(a, b, c):
    ret_int = (a & b) ^ (a & c) ^ (b & c)
    return ret_int


if __name__ == '__main__':
    m = 'asfafa'
    m_sha512 = SHA512(m)
    m_sha512.sha()
    print(m_sha512.summary)
    # m = 0x00010000
    # print(int2bin(ROTR(m, 3))) pass
    # print(int2bin(SHR(m, 28)), len(int2bin(SHR(m, 28))))
    # print(int2bin(SHR(4, 2)), len(int2bin(SHR(4, 2))))

