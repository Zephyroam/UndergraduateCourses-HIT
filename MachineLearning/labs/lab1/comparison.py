import numpy as np
import matplotlib.pyplot as plt
from data import *
from utils import *
from analytical_solution import *
from conjugate_gradient import *
from gradient_descent import *

# 画图
def draw(data_train, data_test, lr=1e-2, epochs_gd=70000, epochs_cg=20, delta=1e-7, lambda_=1e-7, degrees=[5, 7, 9]):
    plt.figure(figsize=(20, 6))
    losses = []
    for i, degree in enumerate(degrees):
        plt.subplot(1, 3, i + 1)
        ana = analytical_solution(degree, lambda_, data_train)
        W_ana = ana.analytical_solution_normalized()
        loss_ana = ana.calc_loss_normalized()

        W_gd, _ = gradient_descent(degree, data_train, lr=lr, epochs=epochs_gd, delta=delta, lambda_=lambda_).gradient_descent()
        loss_gd = get_test_loss_normalized(W_gd, data_test, lambda_)

        W_cg, _ = conjugate_gradient(degree, data_train, epochs=epochs_cg, delta=delta, lambda_=lambda_).conjugate_gradient()
        loss_cg = get_test_loss_normalized(W_cg, data_test, lambda_)

        data_test_X_transformed = transform(data_test[0], degree)
        plt.scatter(data_train[0], data_train[1], facecolor="none", edgecolor="b", label="Training Data")
        plt.plot(data_test[0], data_test[1], label="$\sin(2\pi x)$")
        plt.plot(data_test[0], np.dot(data_test_X_transformed, W_ana), label="Analytical Solution")
        plt.plot(data_test[0], np.dot(data_test_X_transformed, W_gd), label="Gradient Descent Solution")
        plt.plot(data_test[0], np.dot(data_test_X_transformed, W_cg), label="Conjugate Gradient Solution")
        plt.title("M={}".format(degree))
        plt.legend()
        losses.append([loss_ana, loss_gd, loss_cg])
    plt.show()
    for i, degree in enumerate(degrees):
        plt.plot(['Analytical', 'Gradient Descent', 'Conjugate Gradient'], losses[i], label="M={}".format(degree))
    plt.title("Loss")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    sample_num = 10
    data_train, data_test = training_data_generator(sample_num=sample_num), testing_data_generator()
    draw(data_train, data_test)