import torch
import pandas as pd
import numpy as np
from torch.utils.data import TensorDataset, DataLoader
from data.process_word import process_word
import os

# path of data
path = './data/'
data_path = path + 'online_shopping_10_cats.csv'


# get tensor data
def get_tensor_data(data):
    data_X = torch.Tensor([x for x in data['review_id']]).long()
    data_y = torch.Tensor(data['cat_id'].values).long()
    return data_X, data_y


def online_shopping_10_cats(batch_size=64, max_length=32):
    print('Loading data...')
    # path of dump data
    data_dump_path = path + 'online_shopping_10_cats_{}.pt'.format(max_length)
    # if dump data exists, load it
    if os.path.exists(data_dump_path):
        print('Loading data from {}'.format(data_dump_path))
        train_dataset, val_dataset, test_dataset, word_num = torch.load(
            data_dump_path)
    else:
        print('Loading data from {}'.format(data_path))
        data = pd.read_csv(data_path)

        # factorize categorical data
        data['cat_id'] = data['cat'].factorize()[0]
        # process text
        data, word_num = process_word(data, 'review', max_length)

        # split data
        data_groups = list(data.groupby(lambda x: x % 5))
        data_train = pd.concat(
            [data_groups[1][1], data_groups[2][1], data_groups[3][1]])
        data_val = data_groups[4][1]
        data_test = data_groups[0][1]

        train_X, train_y = get_tensor_data(data_train)
        val_X, val_y = get_tensor_data(data_val)
        test_X, test_y = get_tensor_data(data_test)

        train_dataset = TensorDataset(train_X, train_y)
        val_dataset = TensorDataset(val_X, val_y)
        test_dataset = TensorDataset(test_X, test_y)

        # dump data
        torch.save((train_dataset, val_dataset, test_dataset, word_num),
                   data_dump_path)

    # get data loader
    train_loader = DataLoader(train_dataset,
                              batch_size=batch_size,
                              shuffle=True)
    val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False)
    test_loader = DataLoader(test_dataset,
                             batch_size=batch_size,
                             shuffle=False)
    print('Data loaded.')
    return train_loader, val_loader, test_loader, word_num
