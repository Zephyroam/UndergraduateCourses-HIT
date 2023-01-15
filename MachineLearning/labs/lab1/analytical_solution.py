import numpy as np
import matplotlib.pyplot as plt
from data import *
from utils import *
import math

class analytical_solution:
    def __init__(self, degree, lambda_, data_train):
        """
        求解析解初始化

        Args:
            degree (int): 维度
            lambda_ (float): 正则项系数
            data_train (tuple)): 训练样本
        """
        self.degree = degree
        self.lambda_ = lambda_
        self.data_train = data_train
        self.data_train_X_transformed = transform(data_train[0], degree)

    def analytical_solution(self):
        """
        求不带正则项的解析解

        Returns:
            list: W
        """
        self.analytical_solution_W = np.linalg.pinv(self.data_train_X_transformed) @ self.data_train[1]
        return self.analytical_solution_W

    def analytical_solution_normalized(self):
        """
        求带正则项的解析解

        Returns:
            list: W
        """
        data_train_X_t_t = np.transpose(self.data_train_X_transformed)
        self.analytical_solution_normalized_W = np.linalg.pinv(data_train_X_t_t @ self.data_train_X_transformed + self.lambda_* np.identity(len(data_train_X_t_t))) @ data_train_X_t_t @ self.data_train[1]
        return self.analytical_solution_normalized_W
    
    def calc_loss(self):
        """
        计算不带正则项的loss

        Returns:
            float: loss
        """
        return calc_loss(self.data_train_X_transformed, self.analytical_solution_W, self.data_train[1])

    def calc_loss_normalized(self):
        """
        计算带正则项的loss

        Returns:
            float: loss
        """
        return calc_loss(self.data_train_X_transformed, self.analytical_solution_normalized_W, self.data_train[1])

    
# 画图
def draw(data_train, data_test, lambda_=1e-7, normalized=False, degrees=[2, 5, 7, 9]):
    plt.figure(figsize=(12, 8))
    for i, degree in enumerate(degrees):
        plt.subplot(2, 2, i + 1)

        ana = analytical_solution(degree, lambda_, data_train)
        if normalized:
            W = ana.analytical_solution_normalized()
        else:
            W = ana.analytical_solution()
        data_test_X_transformed = transform(data_test[0], degree)
        plt.scatter(data_train[0], data_train[1], facecolor="none", edgecolor="b", label="Training Data")
        plt.plot(data_test[0], data_test[1], "g", label="$\sin(2\pi x)$")
        plt.plot(data_test[0], np.dot(data_test_X_transformed, W), "r", label="Analytical Solution")
        plt.title("M={}".format(degree))
        plt.legend()
    plt.show()

# 画图
def draw_loss(data_train, data_test, lambda_=1e-7, normalized=False, degrees=range(1, 15)):
    losses = []
    for degree in degrees:
        ana = analytical_solution(degree, lambda_, data_train)
        if normalized:
            W = ana.analytical_solution_normalized()
            loss = get_test_loss_normalized(W, data_test, lambda_)
        else:
            W = ana.analytical_solution()
            loss = get_test_loss(W, data_test)
        losses.append(loss)
    plt.plot(degrees, losses, "g", label="Loss")
    plt.title("Loss")
    plt.legend()
    plt.show()

# 画图
def draw_lambda(data_train, data_test, lambda_exp=(0, 30), normalized=True, degree=7):
    losses = []
    lambdas = [10 ** (-x) for x in range(lambda_exp[1], lambda_exp[0], -1)]
    for lambda_ in lambdas:
        ana = analytical_solution(degree, lambda_, data_train)
        if normalized:
            W = ana.analytical_solution_normalized()
            loss = get_test_loss_normalized(W, data_test, lambda_)
        else:
            W = ana.analytical_solution()
            loss = get_test_loss(W, data_test)
        losses.append(loss)
    lambdas = [math.log10(x) for x in lambdas]
    plt.plot(lambdas, losses, "g", label="Loss")
    plt.title("Loss")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    sample_num = 10
    data_train, data_test = training_data_generator(sample_num=sample_num), testing_data_generator()
    draw(data_train, data_test, normalized=False)
    draw_loss(data_train, data_test, normalized=False)
    draw(data_train, data_test, normalized=True)
    draw_loss(data_train, data_test, normalized=True)
    draw_lambda(data_train, data_test)