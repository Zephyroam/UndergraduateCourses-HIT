import numpy as np
from torch.utils.data import TensorDataset, DataLoader
import random
import os
import torch
import scipy.io

data_path = './data/points.mat'


def points(batch_size=64):
    print('Loading data...')
    data = scipy.io.loadmat('data/points.mat')['xx']
    data = np.array(data)
    np.random.shuffle(data)

    # Create TensorDataset
    dataset = TensorDataset(torch.Tensor(data))

    # Create DataLoaders
    data_loader = DataLoader(dataset, batch_size=batch_size, shuffle=True)

    print('Data loaded.')
    return data_loader


if __name__ == '__main__':
    points()