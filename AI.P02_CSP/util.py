class Queue(object):
    def __init__(self):
        self.list = []

    def push(self, item):
        self.list.insert(0, item)

    def pop(self):
        return self.list.pop()

    def size(self):
        return len(self.list)

    def empty(self):
        return self.size() == 0

    def clear(self):
        while not self.list:
            self.pop()

class Constraint(object):
    def __init__(self, pos, goal, conspos):
        self.pos = pos            # The constraint's position
        self.conspos = conspos    # A list of positions limited by the constraint
        self.goal = goal          # The goal sum

class Puzzle(object):
    def __init__(self, size, cons=None):
        self.size = size    # The board's size
        self.cons = cons    # A list of constraints
        self.cells = set([pos for pos in self.cons.conspos])  # A set of tuples
        self.cellsvalue = {}
        for pos in self.cells:
            self.cellsvalue[pos] = None
        self.solutionCnt = 0
        self.GACQueue = Queue()

    def setCons(self, cons):
        self.cons = cons

    def setCellValue(self, pos, v):
        if pos in self.cells:
            self.cellsvalue[pos] = v
        else:
            print('Invalid position!')

    # Determine whether the board is filled
    def full(self):
        for v in self.cellsvalue.values():
            if v is None:
                return False
        return True

    def print(self):
        pass

    def _getDom(self, pos):
        pass

    def _find(self, pos, v):
        pass

    def _GACEnforce(self):
        while not self.GACQueue.empty():
            curcon = self.GACQueue.pop()
            for pos in curcon.conspos:
                curdom = self._getDom(pos)
                for v in curdom:
                    if not self._find(pos, v):   # self._find() return None
                        curdom.remove(v)
                        if not curdom:           # If curdom is empty, DWO occured
                            self.GACQueue.clear()
                            return False
                        else:
                            for cons in self.cons:
                                if pos in cons.conspos and cons not in self.GACQueue.list:
                                    self.GACQueue.push(cons)
        return True


    def _pickAnUnassignedPos_MRV(self):
        pass

    # Use the GAC algorithm to solve the problem
    def solve_GAC(self, depth):
        if self.full():
            self.solutionCnt += 1
            self.print()
        unassignedPos = self._pickAnUnassignedPos_MRV()
        curdom = self._getDom(unassignedPos)
        for v in curdom:
            self.setCellValue(unassignedPos, v)
            # Prune all values of unassignedPos != v from curdom[unassignedPos]
            tmpdom = [v]
            for con in self.cons:
                if unassignedPos in self.cons.conspos:
                    self.GACQueue.push(con)
            if (self._GACEnforce()):
                self.solve_GAC(depth + 1)
        self.setCellValue(unassignedPos, None)
        return





