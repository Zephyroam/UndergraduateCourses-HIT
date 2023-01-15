import torch
import torch.nn as nn
import torch.optim as optim
from data import *
from model import *
from tqdm import tqdm
import argparse
from train import *
from torch.utils.tensorboard import SummaryWriter


def main(batch_size, epochs, lr, cuda):
    # load data
    train_loader, test_loader = jena_climate_2009_2016(batch_size)
    # load model
    model = LSTMRegressor(input_size=1, target_size=1, length=120 * 6)
    if cuda:
        model = model.cuda()
    # load optimizer and criterion
    optimizer = optim.Adam(model.parameters(), lr=lr)
    criterion = nn.MSELoss()
    if cuda:
        criterion = criterion.cuda()
    # train
    train(model, train_loader, optimizer, epochs, criterion, test_loader,
          writer, cuda)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Lab4')
    parser.add_argument('--batch-size',
                        type=int,
                        default=256,
                        help='input batch size for training (default: 256)')
    parser.add_argument('--epochs',
                        type=int,
                        default=50,
                        help='number of epochs to train (default: 50)')
    parser.add_argument('--lr',
                        type=float,
                        default=0.001,
                        help='learning rate (default: 0.001)')
    parser.add_argument('--no-cuda',
                        action='store_true',
                        default=False,
                        help='disables CUDA training')
    args = parser.parse_args()

    # Initialize Tensorboard
    writer = SummaryWriter('./log')

    # set cuda
    cuda = not args.no_cuda and torch.cuda.is_available()
    main(args.batch_size, args.epochs, args.lr, cuda)
