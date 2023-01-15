from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def show_3d(x):
    ax = plt.gca(projection='3d')
    ax.scatter(x[:, 0], x[:, 1], x[:, 2], color="r", label='Origin Data')
    plt.legend()
    plt.show()

def show_2d(x):
    plt.scatter(x[:, 0], x[:, 1], facecolor='r', label='PCA Data')
    plt.legend()
    plt.show()

def psnr(source, target):
    rmse = np.sqrt(np.mean((source - target) ** 2))
    return 20 * np.log10(255.0 / rmse)