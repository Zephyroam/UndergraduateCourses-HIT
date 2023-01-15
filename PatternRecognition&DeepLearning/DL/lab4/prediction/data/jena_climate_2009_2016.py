import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from torch.utils.data import TensorDataset, DataLoader
import os
import torch

data_path = './data/jena_climate_2009_2016.csv'
data_dump_path = './data/jena_climate_2009_2016.pt'

# generate X and Y
def create_X_Y(ts: np.array, lag=1, n_ahead=1, target_index=0, train=False):
    n_features = ts.shape[1]
    X, Y = [], []
    if train:
        for i in range(0, len(ts) - lag - n_ahead):
            X.append(ts[i:(i + lag)])
            Y.append(ts[(i + lag):(i + lag + n_ahead), target_index])
    else:
        for i in range(0, len(ts) - lag - n_ahead, lag + n_ahead):
            X.append(ts[i:(i + lag)])
            Y.append(ts[(i + lag):(i + lag + n_ahead), target_index])

    X, Y = np.array(X), np.array(Y)
    X = np.reshape(X, (X.shape[0], lag, n_features))
    return X, Y


def jena_climate_2009_2016(batch_size=64):
    print('Loading data...')
    if os.path.exists(data_dump_path):
        print('Loading data from {}'.format(data_dump_path))
        train_dataset, test_dataset = torch.load(
            data_dump_path)
    else:
        print('Loading data from {}'.format(data_path))
        data = pd.read_csv(data_path)

        cols = ['T (degC)']
        data = data[cols].values

        # Split data into train and test sets
        train_size = int(data.shape[0] * 0.75)
        train_data = data[:train_size, :]
        test_data = data[train_size:, :]

        # Split train data into X and Y
        train_X, train_y = create_X_Y(train_data,
                                      lag=5 * 24 * 6,
                                      n_ahead=1,
                                      target_index=0,
                                      train=True)
        test_X, test_y = create_X_Y(test_data,
                                    lag=5 * 24 * 6,
                                    n_ahead=2 * 24 * 6,
                                    target_index=0)

        # Create TensorDataset
        train_dataset = TensorDataset(torch.Tensor(train_X),
                                      torch.Tensor(train_y))
        test_dataset = TensorDataset(torch.Tensor(test_X),
                                     torch.Tensor(test_y))

        torch.save((train_dataset, test_dataset),
                   data_dump_path)

    # Create DataLoaders
    train_loader = DataLoader(train_dataset,
                              batch_size=batch_size,
                              shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=True)

    return train_loader, test_loader
