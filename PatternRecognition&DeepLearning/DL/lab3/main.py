import torch
import torch.nn as nn
import torch.optim as optim
import os
from data import *
import models
from tqdm import tqdm
import argparse
from train import *

def get_optimizer(optim_name, model, lr):
    if optim_name == 'adam':
        return optim.Adam(model.parameters(), lr=lr)
    elif optim_name == 'sgd':
        return optim.SGD(model.parameters(), lr=lr, momentum=0.9)
    elif optim_name == 'rmsprop':
        return optim.RMSprop(model.parameters(), lr=lr)
    elif optim_name == 'adagrad':
        return optim.Adagrad(model.parameters(), lr=lr)
    else:
        raise ValueError('Unknown optimizer')

def main(model, optim_name, batch_size, epochs, lr, cuda, only_test, res_path, img_size, data_transforms):
    # load data
    train_loader, val_loader, test_loader, test_ids, idx2specie = plant_seedlings(
        img_size=img_size, batch_size=batch_size, data_transforms=data_transforms)
    if cuda:
        model = model.cuda()
    # load optimizer and criterion
    optimizer = get_optimizer(optim_name, model, lr)
    criterion = nn.CrossEntropyLoss()
    if cuda:
        criterion = criterion.cuda()
    lr_scheduler = optim.lr_scheduler.StepLR(optimizer,
                                             step_size=epochs // 5,
                                             gamma=0.1)
    # train
    if not only_test:
        train(model, train_loader, optimizer, epochs, criterion, lr_scheduler,
              val_loader, res_path, cuda)
    # predict
    model.load_state_dict(torch.load(res_path + 'model.pth'))
    results = predict(model, test_loader, cuda)
    with open(res_path + 'results.csv', 'w') as f:
        f.write('file,species\n')
        for i, result in enumerate(results):
            f.write(str(test_ids[i]) + ',' + idx2specie[result.item()] + '\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Lab3')
    parser.add_argument('--model',
                        type=str,
                        default='resnet20',
                        help='model name',
                        choices=[
                            'resnet18',
                            'resnet34',
                            'resnet18_se',
                            'resnet34_se',
                            'resnet20',
                            'resnet32',
                            'densenet',
                            'vgg11',
                            'vgg13',
                            'vgg16',
                            'vgg19',
                            'vgg11_b',
                            'vgg13_b',
                            'vgg16_b',
                            'vgg19_b',
                        ])
    parser.add_argument('--optim',
                        type=str,
                        default='adam',
                        help='optimizer name',
                        choices=['adam', 'sgd', 'adagrad', 'rmsprop'])
    parser.add_argument('--batch-size',
                        type=int,
                        default=256,
                        help='input batch size for training (default: 256)')
    parser.add_argument('--epochs',
                        type=int,
                        default=40,
                        help='number of epochs to train (default: 40)')
    parser.add_argument('--lr',
                        type=float,
                        default=0.001,
                        help='learning rate (default: 0.001)')
    parser.add_argument('--no-cuda',
                        action='store_true',
                        default=False,
                        help='disables CUDA training')
    parser.add_argument('--only-test',
                        action='store_true',
                        default=False,
                        help='only test')
    parser.add_argument('--img-size',
                        type=int,
                        default=60,
                        help='input image size (default: 60)')
    parser.add_argument('--transform', 
                        type=str,
                        nargs='*',
                        default=['hf', 'vf', 'r'],
                        help='data transform')

    args = parser.parse_args()
    set_seed(2022)

    res_path = './checkpoint/model_{}_bs{}_ep{}_img{}_optim_{}_transform_{}/'.format(
        args.model, args.batch_size, args.epochs, args.img_size, args.optim, args.transform)
    if not os.path.exists(res_path):
        os.makedirs(res_path)
    # args.model + '_' + str(args.batch_size)

    model = models.__dict__[args.model](12)

    # set cuda
    cuda = not args.no_cuda and torch.cuda.is_available()
    main(model, args.optim, args.batch_size, args.epochs, args.lr, cuda, args.only_test,
         res_path, args.img_size, args.transform)
