import random
import sys
K = 4 if len(sys.argv) <= 1 else int(sys.argv[1])
learning_cnt = 10 if len(sys.argv) <= 2 else int(sys.argv[2])
learning_data = []
for i in range(learning_cnt):
    x, y = random.randint(-10000, 10000), random.randint(-10000, 10000)
    if x >= 0 and y >= 0:
        c = '1'
    if x < 0 and y >= 0:
        c = '2'
    if x < 0 and y < 0:
        c = '3'
    if x >= 0 and y < 0:
        c = '4'
    learning_data.append((c, x, y))
    
test_cnt = 2 if len(sys.argv) <= 3 else int(sys.argv[3])
test_data = []
for i in range(test_cnt):
    x, y = random.randint(-10000, 10000), random.randint(-10000, 10000)
    test_data.append((x, y))

with open('data', 'w') as f:
    f.write(str(K)+'\n')
    f.write(str(learning_cnt)+'\n')
    for i in learning_data:
        f.write(str(i[1]) + ' ' + str(i[2]) + ' ' + i[0] + '\n')
    f.write(str(test_cnt)+'\n')
    for i in test_data:
        f.write(str(i[0]) + ' ' + str(i[1]) + '\n')

        