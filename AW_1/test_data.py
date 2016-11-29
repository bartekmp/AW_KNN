import random
import sys
K = int(sys.argv[1]) if len(sys.argv) > 1 else 4
learning_cnt = int(sys.argv[2]) if len(sys.argv) > 2 else 100
learning_data = []
for i in range(learning_cnt):
    x, y, z = random.randint(-10000, 10000), random.randint(-10000, 10000), random.randint(-10000, 10000)
    if x >= 0 and y >= 0:
        c = '1'
    if x < 0 and y >= 0:
        c = '2'
    if x < 0 and y < 0:
        c = '3'
    if x >= 0 and y < 0:
        c = '4'
    learning_data.append((c, x, y, z))

test_cnt = int(sys.argv[2]) if len(sys.argv) > 3 else 10
test_data = []
for i in range(test_cnt):
    x, y, z = random.randint(-10000, 10000), random.randint(-10000, 10000), random.randint(-10000, 10000)
    test_data.append((x, y, z))

with open('data', 'w') as f:
    f.write(str(K)+'\n')
    f.write(str(learning_cnt)+'\n')
    for i in learning_data:
        f.write(str(i[1]) + ' ' + str(i[2]) + ' ' +str(i[3]) + ' ' + i[0] + '\n')
    f.write(str(test_cnt)+'\n')
    for i in test_data:
        f.write(str(i[0]) + ' ' + str(i[1]) + ' ' + str(i[2]) + '\n')









