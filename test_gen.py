from sortedcontainers import SortedList
from random import randint
import random

n = 10
k = 20
mx = 1000
st = SortedList()
siz = n
operator = ['I', 'R', 'S', 'L']

print(n, k)
a = []
for i in range(n):
    x = randint(1, mx)
    a.append(x)
    st.add(x)

print(' '.join(str(x) for x in a))

for i in range(k):
    op = operator[int(randint(0, 15) / 4)]
    if op == 'I':
        x = randint(1, mx)
        st.add(x)
        siz += 1
        print(op, x)
    elif op == 'R':
        if st:
            x = st[randint(0, len(st)-1)]
            st.remove(x)
            siz -= 1
            print(op, x)
        else:
            i -= 1
            continue
    elif op == 'S':
        if not st:
            i -= 1
            continue
        v = randint(1, len(st))
        print(op, v)
    elif op == 'L':
        x = random.choice(st)
        print(op, x)
