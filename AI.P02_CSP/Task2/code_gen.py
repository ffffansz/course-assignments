size = int(input('size: '))
print('%% example %d' % size)
print('solve(ex%d, Ret):-\nset_prolog_flag(answer_write_options, [max_depth(0)]),' % size)
print('Board = [', end='')
for i in range(size):
    for j in range(size):
        print('E%d%d' % (i+1, j+1), end='')
        if i+1 == size and j+1 == size:
            pass
        else:
            print(', ', end='')
    print()
print('],')

for i in range(size):
    print('R %d = [' % i+1, end='')
    for j in range(size):
        print('E%d%d' % (i+1, j+1), end='')
        if j+1 == size:
            pass
        else:
            print(', ')
    print('],')

for j in range(size):
    print('C%d = [' % j+1, end='')
    for i in range(size):
        print('E%d%d' % (i+1, j+1), end='')
        if i+1 == size:
            pass
        else:
            print(', ')
    print('],')