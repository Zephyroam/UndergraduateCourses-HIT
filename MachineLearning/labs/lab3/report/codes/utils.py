import numpy as np
from scipy.stats import multivariate_normal
import itertools

def phi_func(x, _mu, _sigma, delta=-1e7):
    res = multivariate_normal.pdf(x, mean=_mu, cov=_sigma)
    if abs(res) < delta:
        res += delta
    return res

def log_likelihood(x, _alpha, _mu, _sigma, _gamma):
    now_sum = 0
    for k in range(_mu.shape[0]):
        for j in range(x.shape[0]):
            now_sum += _gamma[j][k] * np.log(_alpha[k])
            now_sum += _gamma[j][k] * np.log(phi_func(x[j], _mu[k], _sigma[k]))
    return now_sum

def get_acc(ys, y_preds):
    # 获取准确率
    ks = list(set([x for x in ys]))
    news = list(itertools.permutations(ks))
    acc_best = 0
    for new in news:
        acc = 0
        for i in range(len(ys)):
            if ys[i] == new[y_preds[i]]:
                acc += 1
        acc /= len(ys)
        acc_best = max(acc_best, acc)
    return acc_best