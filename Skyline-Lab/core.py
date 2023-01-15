from tqdm import tqdm
import random


def dominate(x, y):
    flag = False
    for i in range(len(x)):
        flag = flag or (x[i] > y[i])
        if x[i] < y[i]:
            return False
    return flag


def check(result, data):
    result = random.sample(result, min(len(result), 400))
    data = random.sample(data, min(len(data), 4000))
    for x in tqdm(result):
        for y in data:
            if y in result:
                if dominate(x, y) or dominate(x, y):
                    return False
            else:
                if dominate(y, x):
                    return False
    print('Correctness Checked!')
    return True