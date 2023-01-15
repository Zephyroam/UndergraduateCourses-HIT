import numpy as np
import matplotlib.pyplot as plt
from data import *

def transform(X, degree):
    """
    将样本自变量转为矩阵

    Args:
        X (list): 样本自变量
        degree (int): 维度

    Returns:
        list: 矩阵
    """
    X_transformed = np.zeros((X.shape[0], degree))
    for i in range(X.shape[0]):
        X_transformed[i][0] = 1
        for j in range(1, degree):
            X_transformed[i][j] = np.multiply(X[i], X_transformed[i][j - 1])
    return X_transformed

def calc_loss(X, W, T):
    """
    计算不带正则项的loss

    Args:
        X (list): X
        W (list): W
        T (list): T

    Returns:
        float: loss
    """
    loss = 0.5 * np.mean(np.transpose(X @ W - T) @ (X @ W - T))
    return loss

def calc_loss_normalized(X, W, T, lambda_):
    """
    计算带正则项的loss

    Args:
        X (list): X
        W (list): W
        T (list): T
        lambda_ (float): 正则项系数

    Returns:
        float: loss
    """
    loss = 0.5 * np.mean(np.transpose(X @ W - T) @ (X @ W - T) + lambda_ * np.transpose(W) @ W)
    return loss

def calc_gradient(X, W, T):
    """
    计算不带正则项的梯度

    Args:
        X (list): X
        W (list): W
        T (list): T

    Returns:
        list: 梯度
    """
    gradient = np.transpose(X) @ X @ W - np.transpose(X) @ T
    return gradient

def calc_gradient_normalized(X, W, T, lambda_):
    """
    计算带正则项的梯度

    Args:
        X (list): X
        W (list): W
        T (list): T
        lambda_ (float): 正则项系数

    Returns:
        list: 梯度
    """
    gradient = np.transpose(X) @ X @ W - np.transpose(X) @ T + lambda_ * W
    return gradient

def get_test_loss(W, data_test):
    """
    计算测试集上的loss（不带正则项）

    Args:
        W (list): W
        data_test (tuple): 测试样本

    Returns:
        float: loss
    """
    X = transform(data_test[0], W.shape[0])
    return calc_loss(X, W, data_test[1])

def get_test_loss_normalized(W, data_test, lambda_):
    """
    计算测试集上的loss（带正则项）

    Args:
        W (list): W
        data_test (tuple): 测试样本
        lambda_ (float): 正则项系数

    Returns:
        float: loss
    """
    X = transform(data_test[0], W.shape[0])
    return calc_loss_normalized(X, W, data_test[1], lambda_)

def early_stop(loss, loss_last, delta):
    """
    早停策略

    Args:
        loss (float): 当前的loss
        loss_last (float): 上一个loss
        delta (float): 早停策略参数

    Returns:
        bool: 是否停止
    """
    return abs(loss - loss_last) < delta
    

class lr_scheduler_MultiStep:
    def __init__(self, epochs, lr):
        """
        学习率下降策略

        Args:
            epochs (int): 总迭代轮数
            lr (float): 初始学习率
        """
        self.epochs = epochs
        self.lr = lr
        self.gamma = 0.1
        self.milestones = [int(epochs * 0.5), int(epochs * 0.75)]
    def step(self, epoch):
        """
        更新学习率

        Args:
            epoch (int): 当前迭代轮数

        Returns:
            float: 新的学习率
        """
        if epoch in self.milestones:
            self.lr *= self.gamma
        return self.lr
    