from data import load_data
from core import check
from bnl import BNL
import time
import argparse


def SFS(data):
    data = [(x, sum(x)) for x in data]
    data.sort(key=lambda item: item[1], reverse=True)
    data = [x[0] for x in data]
    return BNL(data)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', type=str, default="data/qws/qws_normal.txt")
    args = parser.parse_args()
    data = load_data(args.d)
    tic = time.time()
    result = SFS(data)
    toc = time.time()
    assert check(result, data)
    print('SFS algorithm costs: {:.4}s'.format(toc - tic))