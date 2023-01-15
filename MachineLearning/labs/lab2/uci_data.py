import numpy as np
import matplotlib.pyplot as plt
from utils import *
from mpl_toolkits.mplot3d import Axes3D

def get_uci_data_Skin_NonSkin():
    """
    导入Uci数据

    Returns:
        tuple: 数据
    """
    file_path = './uci_data/Skin_NonSkin.txt'
    with open(file_path, 'r') as f:
        lines = [l[:-1].split('\t') for l in f.readlines()]
        np.random.shuffle(lines)
    X = np.array([[int(l[i]) for i in range(len(l) - 1)] for l in lines])
    Y = np.array([[int(l[i]) - 1 for i in range(len(l) - 1, len(l))] for l in lines])
    select_0, select_1, select = 0, 0, []
    for i in range(len(lines)):
        if select_0 < 1000 and Y[i][0] == 0:
            select.append(i)
            select_0 += 1
        elif select_1 < 1000 and Y[i][0] == 1:
            select.append(i)
            select_1 += 1
            
    X = X[select]
    Y = Y[select]
    train_num = int(X.shape[0] * 0.3)
    train_data = X[:train_num], Y[:train_num]
    test_data = X[train_num:], Y[train_num:]
    return train_data, test_data

# 画图
def show(dataset):
    titles = ['Train Data', 'Test Data']
    for i in range(2):
        fig = plt.figure(figsize=(7, 7))
        X_0, X_1 = get_Xs(dataset[i])
        ax = fig.add_subplot(111, projection='3d')
        ax.scatter(X_0[:, 0], X_0[:, 1], X_0[:, 2], c='r', marker='o')
        ax.scatter(X_1[:, 0], X_1[:, 1], X_1[:, 2], c='b', marker='^')
        plt.title('{}'.format(titles[i]))
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        plt.show()

if __name__ == '__main__':
    dataset = get_uci_data_Skin_NonSkin()
    show(dataset)