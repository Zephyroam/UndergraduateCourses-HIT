import numpy as np
import matplotlib.pyplot as plt
from data import *
from uci_data import *
from utils import *

class newton:
    def __init__(self, dataset, epochs=70000, delta=1e-7, lambda_=1e-3):
        """
        牛顿法初始化

        Args:
            dataset (tuple): 训练样本
            epochs (int, optional): 最大迭代次数. Defaults to 70000.
            delta (float), optional): 早停策略的参数值. Defaults to 1e-7.
            lambda_ (float), optional): 正则项系数. Defaults to 1e-3.
        """
        self.lambda_ = lambda_
        self.dataset = dataset
        self.X = transform(dataset[0])
        self.Y = dataset[1]
        self.epochs = epochs
        self.delta = delta

    def hessian(self, beta):
        """
        生成黑塞矩阵
        
        Args:
            beta (array): beta
        Returns:
            array: 黑塞矩阵
        """
        exp = sigmoid(self.X @ beta)
        H = -self.X.T @ self.X * (exp.T @ (exp - 1)) / self.X.shape[0]
        return H

    def newton(self):
        """
        牛顿法

        Returns:
            tuple: (beta, 总迭代次数)
        """
        beta = np.random.normal(size=(self.X.shape[1], 1))
        loss_last = loss = calc_loss(self.X, beta, self.Y, self.lambda_)
        last_epoch = self.epochs
        for epoch in range(self.epochs):
            beta = beta - np.linalg.pinv(self.hessian(beta)) @ calc_gradient(self.X, beta, self.Y, self.lambda_)
            loss = calc_loss(self.X, beta, self.Y, self.lambda_)
            if early_stop(loss, loss_last, self.delta):
                last_epoch = epoch
                break
            else: loss_last = loss
        return beta, last_epoch

# 画图
def draw(train_data, test_data):
    def draw_(dataset, beta, last_epoch, message):
        acc = calc_accuracy(dataset, beta)
        X_0, X_1 = get_Xs(dataset)
        plt.plot(X_0[:, 0], X_0[:, 1], '.', color='r', label="$y=0$")
        plt.plot(X_1[:, 0], X_1[:, 1], '.', color='b', label="$y=1$")
        plot(beta, 0, 3)
        plt.title('{}, acc={:.2f}, last epoch={}'.format(message, acc, last_epoch))
        plt.legend()

    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    beta, last_epoch = newton(train_data).newton()
    draw_(test_data, beta, last_epoch, 'with regularization')
    plt.subplot(1, 2, 2)
    beta, last_epoch = newton(train_data, lambda_=None).newton()
    draw_(test_data, beta, last_epoch, 'without regularization')
    
    plt.show()

# 画图
def draw_3d(train_data, test_data):
    def draw_(dataset, beta, last_epoch, message):
        acc = calc_accuracy(dataset, beta)
        fig = plt.figure(figsize=(7, 7))
        X_0, X_1 = get_Xs(dataset)
        ax = fig.add_subplot(111, projection='3d')
        ax.scatter(X_0[:, 0], X_0[:, 1], X_0[:, 2], c='r', marker='o')
        ax.scatter(X_1[:, 0], X_1[:, 1], X_1[:, 2], c='b', marker='^')
        plot_3d(beta, 50, 200, ax)
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        plt.title('{}, acc={:.2f}, last epoch={}'.format(message, acc, last_epoch))
        plt.legend()
        plt.show()

    beta, last_epoch = newton(train_data).newton()
    draw_(test_data, beta, last_epoch, 'with regularization')
    
    beta, last_epoch = newton(train_data, lambda_=None).newton()
    draw_(test_data, beta, last_epoch, 'without regularization')
    

if __name__ == "__main__":
    size = (100, 150)
    loc, cov = get_data_option()
    train_data, test_data = data_generator(size, loc, cov)
    draw(train_data, test_data)

    loc, cov = get_data_option(conv=0)
    train_data, test_data = data_generator(size, loc, cov)
    draw(train_data, test_data)

    train_data, test_data = get_uci_data_Skin_NonSkin()
    draw_3d(train_data, test_data)