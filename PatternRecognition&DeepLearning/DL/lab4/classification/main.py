import torch
import torch.nn as nn
import torch.optim as optim
from data import *
from model import *
import argparse
from train import *
from utils import *
from torch.utils.tensorboard import SummaryWriter


def main(model_name, model, batch_size, epochs, lr, max_length, embedding_size,
         cuda):
    # Load data
    train_loader, val_loader, test_loader, word_num = online_shopping_10_cats(
        batch_size, max_length)
    # Build model
    model = model(word_num, embedding_size, max_length=max_length)
    if cuda:
        model = model.cuda()
    # get optimizer
    optimizer = optim.Adam(model.parameters(), lr=lr)
    # get loss function and lr scheduler
    criterion = nn.CrossEntropyLoss()
    if cuda:
        criterion = criterion.cuda()
    lr_scheduler = optim.lr_scheduler.StepLR(optimizer,
                                             step_size=epochs // 5,
                                             gamma=0.1)
    # train model
    train(model_name, model, train_loader, optimizer, epochs, criterion,
          lr_scheduler, test_loader, writer, cuda)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Lab4')
    parser.add_argument('--model',
                        type=str,
                        default='RNN',
                        choices=['RNN', 'GRU', 'LSTM', 'BiLSTM'],
                        help='model name')
    parser.add_argument('--batch-size',
                        type=int,
                        default=64,
                        help='input batch size for training (default: 64)')
    parser.add_argument('--epochs',
                        type=int,
                        default=50,
                        help='number of epochs to train (default: 50)')
    parser.add_argument('--lr',
                        type=float,
                        default=0.001,
                        help='learning rate (default: 0.001)')
    parser.add_argument('--max_length',
                        type=int,
                        default=32,
                        help='max length of sequence (default: 32)')
    parser.add_argument('--embedding_size',
                        type=int,
                        default=100,
                        help='embedding size (default: 100)')
    parser.add_argument('--no-cuda',
                        action='store_true',
                        default=False,
                        help='disables CUDA training')
    args = parser.parse_args()

    # Initialize Tensorboard
    writer = SummaryWriter('./log/model_{}'.format(args.model))

    # set cuda
    cuda = not args.no_cuda and torch.cuda.is_available()
    # set model type
    model = get_model(args.model)
    main(args.model, model, args.batch_size, args.epochs, args.lr,
         args.max_length, args.embedding_size, cuda)
