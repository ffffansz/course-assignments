# !usr/bin/env python
# -*- coding:utf-8 -*-

from copy import deepcopy
import timeit, time


def inference(factorList, orderListOfVE, evidence, queryVar, value):
    """
    :param factorList: list[Nodes]
    :param queryVars: list[var]
    :param orderListOfVE: list[var]
    :param evidence: dict[var: value]
    :return: dict[queryVar: value]
    """
    localFactors = deepcopy(factorList)  # avoiding the factorList being changed
    for e in evidence:
        for factor in localFactors:
            if e in factor.varList:
                factor.restrict(e, evidence[e])

    for var2elim in orderListOfVE:
        factorsIncludeVar = [factor for factor in localFactors if var2elim in factor.varList]
        assert len(factorsIncludeVar) is not 0, \
            'there is a variable to eliminate which is not a valid variable in this list of factors.'
        for i in range(1, len(factorsIncludeVar)):
            factorsIncludeVar[0].multiply(factorsIncludeVar[i])
        factorsIncludeVar[0].sumout(var2elim)
        # remove all factors in factorsIncludeVar
        localFactors = list(set(localFactors).difference(set(factorsIncludeVar)))
        localFactors.append(factorsIncludeVar[0])

    for remainFactor in localFactors:
        assert remainFactor.varList is [queryVar], 'VE error.'

    for i in range(1, len(localFactors)):
        localFactors[0].multiply(localFactors[i])

    return localFactors[0].cpt[((queryVar, value), )]


def printFactors(factorList):
    for factor in factorList:
        factor.printInfo()


def is2dictsMatch(d1, d2, commonVars):
    """If d1 and d2 hava same value of given common keys, return True"""
    for var in commonVars:
        assert var in d1.keys(), str(var) + 'is not a key of d1.'
        assert var in d2.keys(), str(var) + 'is not a key of d2.'
    for var in commonVars:
        if d1[var] is not d2[var]:
            return False
    return True


class CondiPrTable:
    @staticmethod
    def getInitializedCpt(table, vars):
        cpt = {}
        for row in table:
            k = ()   # add None for cpt with single var
            for i in range(len(row) - 1):
                k += ((vars[i], row[i]), )
            cpt[k] = row[-1]
        return cpt


class Node:
    def __init__(self, name, var_list):
        self.name = name
        self.varList = var_list
        self.cpt = {}

    def setCpt(self, cpt):
        self.cpt = cpt

    def printInfo(self):
        print('Name =', self.name)
        print(' vars', str(sorted(self.varList)))
        for k, v in self.cpt.items():
            print(' key:', dict(sorted(k, key=lambda x: x[0])), 'val:', str(v))

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
        self.varList = list(set(self.varList) | set(factor.varList))

    def sumout(self, var):
        """function that sums out a variable given a factor"""
        assert var in self.varList, 'the variable is not in the node\'s varlist.'
        ncpt = {}
        for asm, pr in self.cpt.items():
            d = dict(asm)
            del d[var]
            nk = tuple(d.items())
            if nk not in ncpt:
                ncpt[nk] = pr
            else:
                ncpt[nk] += pr
        self.cpt = ncpt
        self.varList.remove(var)

    def restrict(self, var, value):
        """function that restricts a variable to some value in a given factor"""
        assert var in self.varList, 'the variable is not in the node\'s varlist.'
        ncpt = {}
        for asm, pr in self.cpt.items():
            d = dict(asm)
            if d[var] is value:
                del d[var]
                nk = tuple(d.items())
                ncpt[nk] = pr
        self.cpt = ncpt
        self.varList.remove(var)


class BayesNet:
    def __init__(self, factorList):
        self.nodes = factorList
        self.name_fac_map = dict(tuple([(factor.name, factor) for factor in factorList]))
        self.graph = dict()
        self.initGraph_()

    def initGraph_(self):
        for node in self.nodes:
            self.graph[node.name] = list()
        for node in self.nodes:
            for parent in node.varList:
                if parent is not node.name:
                    self.graph[parent].append(node.name)
        for k in self.graph:
            self.graph[k] = tuple(self.graph[k])

    def getVEOrdering_(self, var2query):
        remainHyperEdges = [nodes.varList for nodes in self.nodes]   # get all hyperedges
        remainNodes = list(self.graph.keys())
        remainNodes.remove(var2query)
        VEOrderList = list()

        if len(remainHyperEdges) is 1:    # if only one hyper edge
            VEOrderList.extend(remainHyperEdges[0])
            VEOrderList.remove(var2query)
            return VEOrderList

        if len(remainNodes) is 1:         # only one node(the queried one)
            return VEOrderList            # return empty orderList

        # 对remainNodes进行迭代，直到最后只剩一个点，即要查询的点
        # 对remianHyperEdges进行迭代，找出下一个要消除的点，这个点满足的条件是，消除之后产生的新的超边的size最小
        while remainNodes:
            newGenHyperEdgeSize = dict()        # key: 某变量 val: 删除该变量后新产生的超边的size
            relatedNodes = dict()
            relatedHyperEdges = dict()
            for node in remainNodes:
                relatedNodes[node] = set()
                relatedHyperEdges[node] = list()
                for he in remainHyperEdges:
                    if node in he:
                        relatedNodes[node] = relatedNodes[node] | set(he)   # 新产生的超边是该变量所在的所有超边的结点集合的并集
                        relatedHyperEdges[node].append(he)
                # 遍历完所有剩余超边，得到与该结点相关的所有结点的集合S，则删除该结点得到的新的超边大小为len(S)-1
                newGenHyperEdgeSize[node] = len(relatedNodes[node]) - 1

            # 找出产生的新超边的size最小的node
            next2e = sorted(newGenHyperEdgeSize, key=lambda k: newGenHyperEdgeSize[k])[0]
            VEOrderList.append(next2e)

            # remove the node from remain nodes list
            remainNodes.remove(next2e)

            # remove related hyper edges
            for he in relatedHyperEdges[next2e]:
                remainHyperEdges.remove(he)

            # add new hyper edge got from removing the node
            remainHyperEdges.append(list(relatedNodes[next2e].difference(set(next2e))))

        return VEOrderList

    def inference(self, evidence, var2query, value):
        localFactors = deepcopy(self.nodes)  # avoiding the factorList being changed
        for e in evidence:
            for factor in localFactors:
                if e in factor.varList:
                    factor.restrict(e, evidence[e])

        orderListOfVE = self.getVEOrdering_(var2query)
        for var2elim in orderListOfVE:
            factorsIncludeVar = [factor for factor in localFactors if var2elim in factor.varList]
            if len(factorsIncludeVar) is 0:
                continue
            # assert len(factorsIncludeVar) is not 0, \
            #    'there is a variable to eliminate which is not a valid variable in this list of factors.'
            for i in range(1, len(factorsIncludeVar)):
                factorsIncludeVar[0].multiply(factorsIncludeVar[i])
            factorsIncludeVar[0].sumout(var2elim)
            # remove all factors in factorsIncludeVar
            localFactors = list(set(localFactors).difference(set(factorsIncludeVar)))
            localFactors.append(factorsIncludeVar[0])

        # for remainFactor in localFactors:
        #    assert remainFactor.varList is [var2query], 'VE error.'

        remainFactors = [factor for factor in localFactors if var2query in factor.varList]

        for i in range(1, len(remainFactors)):
            remainFactors[0].multiply(remainFactors[i])

        return remainFactors[0].cpt[((var2query, value), )]


if __name__ == '__main__':
    A = Node("A", ["A", "B", "E"])
    A.setCpt({(('A', 1), ('B', 1), ('E', 1)): 0.95,
              (('A', 0), ('B', 1), ('E', 1)): 0.05,
              (('A', 1), ('B', 1), ('E', 0)): 0.94,
              (('A', 0), ('B', 1), ('E', 0)): 0.06,
              (('A', 1), ('B', 0), ('E', 1)): 0.29,
              (('A', 0), ('B', 0), ('E', 1)): 0.71,
              (('A', 1), ('B', 0), ('E', 0)): 0.001,
              (('A', 0), ('B', 0), ('E', 0)): 0.999})
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
    B = Node("B", ["B"])
    B.setCpt({(('B', 0), ): 0.999,
              (('B', 1), ): 0.001})
    M.printInfo()
    J.printInfo()
    M.multiply(J)
    M.printInfo()
