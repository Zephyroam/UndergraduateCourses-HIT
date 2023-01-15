import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
from utils import *
import matplotlib.pyplot as plt
from torch.utils.tensorboard import SummaryWriter


def show(epoch, datas, preds, targets, writer: SummaryWriter):
    # plot the result
    sampled_ids = np.random.choice(len(datas), size=9, replace=False)
    fig = plt.figure(1, figsize=(15, 6))
    for i, idx in enumerate(sampled_ids):
        plt.subplot(3, 3, i + 1)
        data = datas[idx, :, 0].reshape(-1)
        pred = preds[idx].reshape(-1)
        target = targets[idx].reshape(-1)

        data_size = data.shape[0]
        pred_size = pred.shape[0]
        # plot the data
        plt.plot(range(data_size), data, color='blue')
        # plot the prediction
        plt.plot(range(data_size, data_size + pred_size),
                 pred,
                 label='pred',
                 color='red')
        # plot the target
        plt.plot(range(data_size, data_size + pred_size),
                 target,
                 label='target',
                 color='blue')
        plt.legend()
    plt.show()
    writer.add_figure('test_figure', fig, epoch)
