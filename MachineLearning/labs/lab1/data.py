import numpy as np
import matplotlib.pyplot as plt
from utils import *

def training_data_generator(x_range=(0.0, 1.0), sample_num=10, scale=0.2):
    """
    生成训练样本

    Args:
        x_range (tuple, optional): X的范围. Defaults to (0.0, 1.0).
        sample_num (int, optional): 训练样本数量. Defaults to 10.
        scale (float, optional): 噪声标准差. Defaults to 0.2.

    Returns:
        tuple: 训练样本(X, Y)
    """

    X = np.linspace(x_range[0], x_range[1], sample_num)
    Y = np.sin(2 * np.pi * X) + np.random.normal(scale=scale, size=X.shape)
    return X, Y

def testing_data_generator(x_range=(0.0, 1.0), sample_num=1000):
    """
    生成测试样本

    Args:
        x_range (tuple, optional): X的范围. Defaults to (0.0, 1.0).
        sample_num (int, optional): 测试样本数量. Defaults to 1000.

    Returns:
        tuple: 测试样本(X, Y)
    """

    X = np.linspace(x_range[0], x_range[1], sample_num)
    Y = np.sin(2 * np.pi * X)
    return X, Y

# 画图
def show(data_trian, data_test):
    plt.scatter(data_trian[0], data_trian[1], facecolor="none",  edgecolor="b",  s=50, label="training data")
    plt.plot(data_test[0], data_test[1], c="g", label="$\sin(2\pi x)$")
    plt.ylabel("y", size=20)
    plt.xlabel("x", size=20)
    plt.legend()
    plt.show()

if __name__ == '__main__':
    data_train, data_test = training_data_generator(), testing_data_generator()
    show(data_train, data_test)