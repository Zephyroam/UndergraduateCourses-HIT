import numpy as np
import matplotlib.pyplot as plt
from data import *
from utils import *
import math

class gradient_descent:
    def __init__(self, degree, data_train, lr=1e-2, epochs=70000, delta=1e-7, lambda_=1e-7):
        """
        梯度下降法初始化

        Args:
            degree (int)): 维度
            data_train (tuple): 训练样本
            lr (float), optional): 学习率. Defaults to 1e-2.
            epochs (int, optional): 最大迭代次数. Defaults to 70000.
            delta (float), optional): 早停策略的参数值. Defaults to 1e-7.
            lambda_ (float), optional): 正则项系数. Defaults to 1e-7.
        """
        self.degree = degree
        self.lambda_ = lambda_
        self.data_train = data_train
        self.lr = lr
        self.epochs = epochs
        self.delta = delta
        self.data_train_X_transformed = transform(data_train[0], degree)

    def gradient_descent(self):
        """
        梯度下降法

        Returns:
            tuple: (W, 总迭代次数)
        """
        X, T = self.data_train_X_transformed, self.data_train[1]
        W = np.random.normal(size=(X.shape[1]))
        lr_scheduler = lr_scheduler_MultiStep(self.epochs, self.lr)
        loss_last = loss = calc_loss_normalized(X, W, T, self.lambda_)
        last_epoch = self.epochs
        for epoch in range(self.epochs):
            W = W - self.lr * calc_gradient_normalized(X, W, T, self.lambda_)
            loss = calc_loss_normalized(X, W, T, self.lambda_)
            if early_stop(loss, loss_last, self.delta):
                last_epoch = epoch
                break
            else: loss_last = loss
            self.lr = lr_scheduler.step(epoch)
        return W, last_epoch

# 画图
def draw(data_train, data_test, lr=1e-2, epochs=70000, delta=1e-7, lambda_=1e-7, degrees=[2, 5, 7, 9]):
    plt.figure(figsize=(12, 8))
    for i, degree in enumerate(degrees):
        plt.subplot(2, 2, i + 1)
        gd = gradient_descent(degree, data_train, lr=lr, epochs=epochs, delta=delta, lambda_=lambda_)
        W, actual_epoch = gd.gradient_descent()
        data_test_X_transformed = transform(data_test[0], degree)
        plt.scatter(data_train[0], data_train[1], facecolor="none", edgecolor="b", label="Training Data")
        plt.plot(data_test[0], data_test[1], "g", label="$\sin(2\pi x)$")
        plt.plot(data_test[0], np.dot(data_test_X_transformed, W), "r", label="Gradient Descent Solution")
        plt.title("M={}, Actual Epoch={}".format(degree, actual_epoch))
        plt.legend()
    plt.show()

# 画图
def draw_loss_lr(data_train, data_test, degree=7, epochs=70000, delta=1e-7, lambda_=1e-7, lrs=[5e-5, 1e-4, 5e-4, 1e-3, 5e-3, 1e-2, 5e-2, 1e-1]):
    losses, actual_epochs = [], []
    plt.figure(figsize=(12, 6))
    for lr in lrs:
        gd = gradient_descent(degree, data_train, lr=lr, epochs=epochs, delta=delta, lambda_=lambda_)
        W, actual_epoch = gd.gradient_descent()
        loss = get_test_loss_normalized(W, data_test, lambda_)
        losses.append(loss)
        actual_epochs.append(actual_epoch)
    lrs = [math.log10(x) for x in lrs]
    plt.subplot(1, 2, 1)
    plt.plot(lrs, losses, "g")
    plt.xlabel("Learning Rate($\lg lr$)")
    plt.ylabel("Loss")
    plt.title("Loss")
    plt.legend()

    plt.subplot(1, 2, 2)
    plt.plot(lrs, actual_epochs, "g")
    plt.xlabel("Learning Rate($\lg lr$)")
    plt.ylabel("Actual Epoch")
    plt.title("Actual Epoch")
    plt.legend()
    plt.show()
    

if __name__ == "__main__":
    sample_num = 10
    data_train, data_test = training_data_generator(sample_num=sample_num), testing_data_generator()
    draw(data_train, data_test)
    draw_loss_lr(data_train, data_test)