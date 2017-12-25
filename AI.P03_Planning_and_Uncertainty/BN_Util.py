# !usr/bin/env python
# -*- coding:utf-8 -*-

from copy import deepcopy
import timeit, time


def inference(factorList, queryVars, orderListOfVE, evidence):
    """
    :param factorList: list[Nodes]
    :param queryVars: list[str]
    :param orderListOfVE: list[str]
    :param evidence: dict[strs: values]
    :return: dict[queryVars: values]
    """
    pass


def printFactors(factorList):
    for factor in factorList:
        factor.printInfo()


def enumAssignmentComb(vars, vardoms):
    """Return the enum of combination of variables' assignment with their domains, as a list of dict"""
    '''
    @:param vars: iterable object (tuple or list)
    @:param vardoms: dict, e.g. {'a': set(1, 2, 3), 'b': set(4, 5, 6)}
    '''
    if len(vars) is 1:
        ret = []
        for v in vardoms[vars[0]]:
            ret.append({vars[0]: v})
        return ret

    subret = enumAssignmentComb(vars[1:], vardoms)
    ret = []
    for v in vardoms[vars[0]]:
        for e in subret:
            e[vars[0]] = v
            ret.append(deepcopy(e))

    return ret


def is2tuplesMatch(t1, t2, commonVarPositions):
    """If t1 and t2 have same element at every pair of positions, return True"""
    # @:param commonVarPositions: e.g. {'a': (5, 2)} means we need t1[5] == t2[2]
    for pos in commonVarPositions.values():
        if t1[pos[0]] is not t2[pos[1]]:
            return False
    return True

def is2dictsMatch(d1, d2, commonVars):
    """If d1 and d2 hava same value of given common keys, return True"""
    for var in commonVars:
        assert var in d1.keys(), str(var) + 'is not a key of d1.'
        assert var in d2.keys(), str(var) + 'is not a key of d2.'
    for var in commonVars:
        if d1[var] is not d2[var]:
            return False
    return True

def removeElementFromTuple(object, *, index=None, element=None):
    """Return a tuple which is same as t except removing object[index]"""
    if index is not None and element is not None:
        if object.index(element) is not index:
            return
        else:
            return object[:index] + object[index+1:]

    if index is not None:
        if index >= len(object):
            return
        else:
            return object[:index] + object[index+1:]

    if element is not None:
        if element not in object:
            return
        else:
            eidx = object.index(element)
            return object[:eidx] + object[eidx + 1:]


def removeit(t, *, index=None, element=None):
    l = list(t)
    if element is not None:
        l.remove(element)
        return tuple(l)
    else:
        l.remove(l[index])
        return tuple(l)

class Node:
    def __init__(self, name, var_list):
        self.name = name
        self.varList = var_list
        self.cpt = {}

    def setCpt(self, cpt):
        self.cpt = cpt

    def printInfo(self):
        print('Name =', self.name)
        print(' vars', str(self.varList))

        for k in self.cpt:
            print(' key:', k, 'val:', str(self.cpt[k]))

        print()

    def multiply(self, factor):
        """function that multiplies with another factor"""
        comvars = tuple(set(self.varList).intersection(set(factor.varList)))  # get common variables
        assert len(comvars) is not 0, 'the two factors have no common variable.'
        ncpt = {}  # new cpt
        for asm1 in self.cpt.keys():
            d1 = dict(asm1)   # convert to dict type
            for asm2 in factor.cpt.keys():
                d2 = dict(asm2)
                if is2dictsMatch(d1, d2, comvars):
                    nk = tuple(set(asm1) | set(asm2))   # The complement of the intersection
                    ncpt[nk] = self.cpt[asm1] * factor.cpt[asm2]
        self.cpt = ncpt
        for var in comvars:
            self.varList.remove(var)

    def sumout(self, var):
        """function that sums out a variable given a factor"""
        assert var in self.varList, 'the variable is not in the node\'s varlist.'

    def restrict(self, var, value):
        """function that restricts a variable to some value in a given factor"""
        assert var in self.varList, 'the variable is not in the node\'s varlist.'


def genRanTuple():
    import random
    size = random.randint(500, 1000)
    ret = []
    for i in range(size):
        ret.append(random.randint(1000, 50000))
    return tuple(ret)


A = Node("A", ["A", "B", "E"])
A.setCpt({(('A', 1), ('B', 1), ('C', 1)): 0.95,
          (('A', 0), ('B', 1), ('C', 1)): 0.05,
          (('A', 1), ('B', 1), ('C', 0)): 0.94,
          (('A', 0), ('B', 1), ('C', 0)): 0.06,
          (('A', 1), ('B', 0), ('C', 1)): 0.29,
          (('A', 0), ('B', 0), ('C', 1)): 0.71,
          (('A', 1), ('B', 0), ('C', 0)): 0.001,
          (('A', 0), ('B', 0), ('C', 0)): 0.999})
J = Node("J", ["J", "A"])
J.setCpt({(('J', 1), ('A', 1)): 0.9,
          (('J', 0), ('A', 1)): 0.1,
          (('J', 1), ('A', 0)): 0.05,
          (('J', 0), ('A', 0)): 0.95})
M = Node("M", ["M", "A"])
M.setCpt({(('M', 1), ('A', 1)): 0.7,
          (('M', 0), ('A', 1)): 0.3,
          (('M', 1), ('A', 0)): 0.01,
          (('M', 0), ('A', 0)): 0.99})
M.printInfo()
J.printInfo()
M.multiply(J)
M.printInfo()
