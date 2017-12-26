# !usr/bin/env python
# -*- coding:utf-8 -*-

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