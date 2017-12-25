# !usr/bin/env python
# -*- coding:utf-8 -*-

from BN_Util import *

if __name__ == '__main__':
    N1 = Node('N1', ['A', 'B'])
    N2 = Node('N2', ['B', 'C'])
    N3 = Node('N3', ['C', 'D'])

    N1.setCpt({(('A', 1), ('B', 1)): 0.9,
               (('A', 1), ('B', 0)): 0.1,
               (('A', 0), ('B', 1)): 0.4,
               (('A', 0), ('B', 0)): 0.6})
    N2.setCpt({(('B', 1), ('C', 1)): 0.7,
               (('B', 1), ('C', 0)): 0.3,
               (('B', 0), ('C', 1)): 0.8,
               (('B', 0), ('C', 0)): 0.2})
    N3.setCpt({(('C', 1), ('D', 1)): 0.3,
               (('C', 1), ('D', 0)): 0.7,
               (('C', 0), ('D', 1)): 0.2,
               (('C', 0), ('D', 0)): 0.8})
    N1.restrict('A', 1)
    N2.restrict('C', 1)
    N1.printInfo()
    N2.printInfo()
    N1.multiply(N2)
    N1.printInfo()