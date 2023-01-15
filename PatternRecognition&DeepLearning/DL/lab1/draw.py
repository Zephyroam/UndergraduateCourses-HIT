import matplotlib.pyplot as plt
import torch
import numpy as np


def plot_loss():
    plt.figure(figsize=(16, 8))  # set figure size
    Losses, Accs = torch.load('./checkpoint/loss_acc_result.pt')
    x = range(0, len(Losses))
    plt.subplot(1, 2, 1)
    plt.plot(x, Losses)
    plt.title('Loss on training set')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.subplot(1, 2, 2)
    plt.plot(x, Accs)
    plt.title('Accuracy on test set')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.show()


if __name__ == "__main__":
    plot_loss()