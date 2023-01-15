from data import load_data
from core import dominate, check
import time
import argparse
from tqdm import trange


def BNL(data):
    result = [data[0]]
    for i in trange(1, len(data)):
        insert_cur = True
        j = 0
        while j < len(result):
            if dominate(data[i], result[j]):
                result.pop(j)
            elif dominate(result[j], data[i]):
                insert_cur = False
                break
            else:
                j = j + 1
        if insert_cur:
            result.append(data[i])
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', type=str, default="data/qws/qws_normal.txt")
    args = parser.parse_args()
    data = load_data(args.d)
    tic = time.time()
    result = BNL(data)
    toc = time.time()
    assert check(result, data)
    print('BNL algorithm costs: {:.4}s'.format(toc - tic))