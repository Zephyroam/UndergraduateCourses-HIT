import numpy as np
import os
from PIL import Image

from data import *
from utils import *


def pca(x, k, rebuild=False):
    mu = np.mean(x, axis=0)
    cov = np.cov(x, rowvar=False)
    values, vectors = np.linalg.eig(cov)
    index = np.argsort(values)[: -(k + 1): -1]
    vectors = vectors[:, index]
    x_pca = (x - mu).dot(vectors)
    if rebuild:
        x_pca = x_pca.dot(vectors.T) + mu
    return x_pca


def show(x, x_pca):
    show_3d(x)
    show_2d(x_pca)


def pca_my_data():
    x = generate_swiss_roll()
    x_pca = pca(x, 2)
    show(x, x_pca)


def read_face(file_path, figsize=(40, 40)):
    file_list = os.listdir(file_path)
    data = []
    for i in range(len(file_list)):
        path = os.path.join(file_path, file_list[i])
        L = Image.open(path).resize(figsize).convert('L')
        data.append(np.array(L))
    return np.array(data)

def pca_face(file_path, figsize=(40, 40)):
    x = read_face(file_path, figsize=figsize)
    print(x.shape)
    n_samples = x.shape[0]
    d_decreased = [30, 20, 10, 5, 4, 3, 2, 1]
    for i in range(n_samples):
        plt.subplot(3, 3, 1), plt.title('original pic'), plt.axis('off')
        plt.imshow(x[i], cmap='gray')
        for j in range(len(d_decreased)):
            plt.subplot(3, 3, j + 2), plt.axis('off')
            x_pca = pca(x[i], d_decreased[j], rebuild=True)
            plt.title('d={}, PSNR={:.2f}'.format(d_decreased[j], psnr(x[i], x_pca)))
            plt.imshow(np.real(x_pca.reshape(figsize)), cmap='gray')
        plt.show()

if __name__ == '__main__':
    # pca_my_data()
    file_path = "./face"
    pca_face(file_path)
    
