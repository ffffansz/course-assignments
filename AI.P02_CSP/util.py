import copy

class Queue(object):
    def __init__(self):
        self.itemlist = []

    def push(self, item):
        self.itemlist.insert(0, item)

    def pop(self):
        return self.itemlist.pop()

    def size(self):
        return len(self.itemlist)

    def empty(self):
        return self.size() == 0

    def clear(self):
        while not self.itemlist:
            self.pop()

class Constraint(object):
    def __init__(self, contpos, goal, selfpos=None):
        self.pos = selfpos        # The constraint's position (for show the board)
        self.contpos = contpos    # A tuple of positions limited by the constraint
        self.goal = goal          # The goal sum

class Puzzle(object):
    def __init__(self, *, size=0, cont=None, dom=None):
        self.size = size    # The board's size
        self.cont = cont    # A list of constraints
        self.cells = set([pos for pos in self.cont.contpos])  # All positions in the game board
        self.cellsvalue = {}                                  # Value of each position
        for pos in self.cells:
            self.cellsvalue[pos] = 0
        self.solutionCnt = 0
        self.GACQueue = Queue()
        if dom is not None:
            self.dom = dom
        else:
            self.dom = {}
            for pos in self.cells:
                self.dom[pos] = [i for i in range(1, 10)]
        self.supports = {}        # A list of supports for each constraint
                                  # And each support is a dict including several var with its assignment

    def setCons(self, cont):
        self.cont = cont

    def setCellValue(self, pos, v):
        if pos in self.cells:
            self.cellsvalue[pos] = v
        else:
            print('Invalid position!')

    # Determine whether the board is filled
    def full(self):
        for v in self.cellsvalue.values():
            if v == 0:
                return False
        return True

    def print(self):
        pass

    def _isContStatisfied(self, cont):
        v = [self.cellsvalue[pos] for pos in cont.contpos]
        if len(v) != len(set(v)):
            return False
        return sum(v) == cont.goal

    # def _getDom(self, pos):
    #   return self.dom[pos]

    # Determine whether pos = v is already in the support list
    # If it exists, check whether the assignment of other variables is within their respective domains
    # If not, compute to find a support
    # If we can find a support, then add it to the support list and return True; otherwise, return False
    def _findSupport(self, cont, pos, v):
        # Try to find an existed support
        if cont in self.supports:
            for support in self.supports[cont]:
                if pos in support.keys() and support[pos] == v:
                    # Check other vars
                    for k, v in support.items():
                        if v not in self.dom[k]:
                            return False
                    return True

        # Compute to find a support
        # Actually, now we meet a mini CSP, and we try to solve it with GAC
        # The mini CSP has the following two constraints:
        # 1. pos = v
        # 2. original constraint, i.e. var cont
        anoCont = Constraint((pos, ), v)
        miniCSP = Puzzle(cont=[anoCont, cont], dom=self.dom)
        # TODO

    def _GACEnforce(self):
        while not self.GACQueue.empty():
            curcont = self.GACQueue.pop()
            for pos in curcont.contpos:
                for v in self.dom[pos]:
                    if not self._findSupport(curcont, pos, v):   # self._findSupport() return None
                        self.dom[pos].remove(v)
                        if not self.dom[pos]:                    # If curdom is empty, DWO occured
                            self.GACQueue.clear()
                            return False
                        else:
                            for cont in self.cont:
                                if pos in cont.contpos and cont not in self.GACQueue.itemlist:
                                    self.GACQueue.push(cont)
        return True

    def _pickAnUnassignedPos_MRV(self):
        domsize = {}
        for dom in self.dom.values():
            domsize[dom] = len(self.dom[dom])
        return min(domsize)  # Return the position with smallest domain

    # Use the GAC algorithm to solve the problem
    def solve_GAC(self, depth):
        if self.full():
            self.solutionCnt += 1
            self.print()
        unassignedPos = self._pickAnUnassignedPos_MRV()
        for v in self.dom[unassignedPos]:
            self.setCellValue(unassignedPos, v)
            dom_record = copy.deepcopy(self.dom)
            self.dom[unassignedPos] = [v]  # Prune all values of unassignedPos != v from curdom[unassignedPos]
            for con in self.cont:
                if unassignedPos in con.contpos:
                    self.GACQueue.push(con)
            if self._GACEnforce():
                self.solve_GAC(depth + 1)
            self.dom = dom_record                 # Restore all pruned values from self.dom
        self.setCellValue(unassignedPos, 0)       # Reset the changed value
        return

