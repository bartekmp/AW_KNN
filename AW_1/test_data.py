import random
import sys
K = 4 if len(sys.argv) <= 1 else int(sys.argv[1])
dim = 2 if len(sys.argv) <= 2 else int(sys.argv[2])
learning_cnt = 10 if len(sys.argv) <= 3 else int(sys.argv[3])
learning_data = []
for i in range(learning_cnt):
    xi = []
    for j in range(dim):
        xi.append(random.randint(-10000, 10000))
    if xi[0] >= 0 and xi[1] >= 0:
        c = '1'
    if xi[0] < 0 and xi[1] >= 0:
        c = '2'
    if xi[0] < 0 and xi[1] < 0:
        c = '3'
    if xi[0] >= 0 and xi[1] < 0:
        c = '4'
    learning_data.append((c, xi))
    
test_cnt = 2 if len(sys.argv) <= 4 else int(sys.argv[4])
test_data = []
for i in range(test_cnt):
    xj = []
    for j in range(dim):
        xj.append(random.randint(-10000, 10000))
    test_data.append((xj))

with open('data', 'w') as f:
    f.write(str(K)+'\n')
    f.write(str(dim)+'\n')
    f.write(str(learning_cnt)+'\n')
    for i in learning_data:
        line = ''
        for j in i[1]:
            line += ' ' + str(j)
        f.write(i[0] + line + '\n')
    f.write(str(test_cnt)+'\n')
    for i in test_data:
        line = ''
        for j in i:
            line += str(j) + ' '
        f.write(line + '\n')

        