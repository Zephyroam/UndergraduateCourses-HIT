import torch
import torch.nn as nn
from data import *
from model import *
import argparse
from train import *


def main(model, batch_size, epochs, lr, optimizer, n_noise, clamp, cuda):
    # load data
    data_loader = points(batch_size)
    # load model and train
    if model == 'GAN':
        model_G = GAN_G(n_noise)
        model_D = GAN_D()
        if cuda:
            model_G, model_D = model_G.cuda(), model_D.cuda()
        train_GAN(model_G, model_D, data_loader, lr, optimizer, epochs,
                  n_noise, cuda)
    elif model == 'WGAN':
        model_G = WGAN_G(n_noise)
        model_D = WGAN_D()
        if cuda:
            model_G, model_D = model_G.cuda(), model_D.cuda()
        train_WGAN(model_G, model_D, data_loader, lr, optimizer, epochs,
                   n_noise, clamp, cuda)
    elif model == 'WGAN-GP':
        model_G = WGAN_G(n_noise)
        model_D = WGAN_D()
        if cuda:
            model_G, model_D = model_G.cuda(), model_D.cuda()
        train_WGAN_GP(model_G, model_D, data_loader, lr, optimizer, epochs,
                      n_noise, clamp)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Lab5')
    parser.add_argument('--model',
                        type=str,
                        default='GAN',
                        choices=['GAN', 'WGAN', 'WGAN-GP'],
                        help='model name')
    parser.add_argument('--batch-size',
                        type=int,
                        default=1024,
                        help='input batch size for training (default: 1024)')
    parser.add_argument('--epochs',
                        type=int,
                        default=300,
                        help='number of epochs to train (default: 300)')
    parser.add_argument('--lr',
                        type=float,
                        default=0.001,
                        help='learning rate (default: 0.001)')
    parser.add_argument('--optim',
                        type=str,
                        default=None,
                        choices=['SGD', 'Adam', 'RMSprop'],
                        help='optimizer (default: None)')
    parser.add_argument('--n-noise',
                        type=int,
                        default=3,
                        help='number of noise (default: 3)')
    parser.add_argument('--clamp',
                        type=float,
                        default=0.1,
                        help='clamp value (default: 0.1)')
    parser.add_argument('--no-cuda',
                        action='store_true',
                        default=False,
                        help='disables CUDA training')
    args = parser.parse_args()

    # set cuda
    cuda = not args.no_cuda and torch.cuda.is_available()
    main(args.model, args.batch_size, args.epochs, args.lr, args.optim,
         args.n_noise, args.clamp, cuda)
