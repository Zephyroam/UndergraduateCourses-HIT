import numpy as np
import matplotlib.pyplot as plt
from data import *
from utils import *

class conjugate_gradient:
    def __init__(self, degree, data_train, epochs=20, delta=1e-7, lambda_=1e-7):
        """
        共轭梯度法初始化

        Args:
            degree (int)): 维度
            data_train (tuple): 训练样本
            epochs (int, optional): 最大迭代次数. Defaults to 20.
            delta (float), optional): 早停策略的参数值. Defaults to 1e-7.
            lambda_ (float), optional): 正则项系数. Defaults to 1e-7.
        """
        self.degree = degree
        self.lambda_ = lambda_
        self.data_train = data_train
        self.epochs = epochs
        self.delta = delta
        self.data_train_X_transformed = transform(data_train[0], degree)

    def conjugate_gradient(self):
        """
        共轭梯度法

        Returns:
            tuple: (W, 总迭代次数)
        """
        X, T = self.data_train_X_transformed, self.data_train[1]
        A = np.transpose(X) @ X - self.lambda_ * np.identity(len(np.transpose(X)))
        b = np.transpose(X) @ T
        x = np.random.normal(size=(A.shape[1]))
        r_0 = A @ x - b
        p = -r_0
        last_epoch = self.epochs
        for i in range(self.epochs):
            alpha = np.transpose(r_0) @ r_0 / (np.transpose(p) @ A @ p)
            x = x + alpha * p
            r = r_0 + alpha * A @ p
            if np.transpose(r_0) @ r_0 < self.delta:
                last_epoch = i
                break
            beta = np.transpose(r) @ r / (np.transpose(r_0) @ r_0)
            p = -r + beta * p
            r_0 = r
        return x, last_epoch
    
# 画图
def draw(data_train, data_test, epochs=20, delta=1e-7, lambda_=1e-7, degrees=[2, 5, 7, 9]):
    plt.figure(figsize=(12, 8))
    for i, degree in enumerate(degrees):
        plt.subplot(2, 2, i + 1)
        cg = conjugate_gradient(degree, data_train, epochs=epochs, delta=delta, lambda_=lambda_)
        W, actual_epoch = cg.conjugate_gradient()
        data_test_X_transformed = transform(data_test[0], degree)
        plt.scatter(data_train[0], data_train[1], facecolor="none", edgecolor="b", label="Training Data")
        plt.plot(data_test[0], data_test[1], "g", label="$\sin(2\pi x)$")
        plt.plot(data_test[0], np.dot(data_test_X_transformed, W), "r", label="Conjugate Gradient Solution")
        plt.title("M={}, Actual Epoch={}".format(degree, actual_epoch))
        plt.legend()
    plt.show()


if __name__ == "__main__":
    sample_num = 10
    data_train, data_test = training_data_generator(sample_num=sample_num), testing_data_generator()
    draw(data_train, data_test)