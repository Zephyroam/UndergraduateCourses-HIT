import numpy as np
import matplotlib.pyplot as plt


def data_generator(k, size, loc, cov):
    # 生成数据
    X = np.zeros((k * size, loc.shape[1]))
    Y = np.zeros((k * size), dtype=np.int)
    for i in range(k):
        X[i * size:(i + 1) * size] = np.random.multivariate_normal(loc[i], cov[i], size=(size))
        Y[i * size:(i + 1) * size] = i
    shuffle_ix = np.random.permutation(X.shape[0])
    X, Y = X[shuffle_ix], Y[shuffle_ix]
    return X, Y

def get_data_option(k=3, size=150):
    loc = np.array([[2, 7], [6, 2], [8, 7]])
    cov = np.array([[[1, 0], [0, 2]], [[2, 0], [0, 1]], [[1, 0], [0, 1]]])
    return k, size, loc, cov

# 画图
def show(dataset):
    X, Y = dataset
    k = max(y for y in Y) + 1
    print(k)
    colors = ['r', 'g', 'y', 'b']
    for i in range(k):
        ix = np.where(Y == i)
        plt.plot(X[ix, 0], X[ix, 1], '.', color=colors[i])
    plt.title('Dataset')
    plt.legend()
    plt.show()

if __name__ == '__main__':
    k, size, loc, cov = get_data_option()
    dataset = data_generator(k, size, loc, cov)
    show(dataset)
