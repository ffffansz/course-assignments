def foo(l):
    l.extend([4, ])
    return l

l = [1, 2, 3]
lc = foo(l)
print('lc =', lc)
print('l =', l)

# Result: vc = 7, v = 5
# 可变对象会被更改，不可变对象不会被更改
# 不可变：int, str, float, tuple
# 可变: list, dict