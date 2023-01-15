import torch
import torch.nn as nn
import torch.optim as optim
from data import *
from model import *
from tqdm import tqdm
from sklearn.metrics import median_absolute_error, mean_absolute_error
import numpy as np
from utils import *
from show import *


def get_metrics(preds, targets):
    "get mean absolute error and median absolute error"
    meanae = mean_absolute_error(preds, targets)
    medianae = median_absolute_error(preds, targets)
    return meanae, medianae


def train(model, train_loader, optimizer,
          epochs, criterion, test_loader, writer, cuda):
    metrics_best = (1e9, 1e9)
    for epoch in range(epochs):
        Loss = AverageMeter()
        model.train()
        for data, target in tqdm(train_loader):
            if cuda:
                data, target = data.cuda(), target.cuda()
            optimizer.zero_grad()
            output = model(data)
            loss = criterion(output, target)
            Loss.update(loss.item(), data.size(0))
            loss.backward()
            optimizer.step()

        print('Epoch {}/{} Loss: {:.4f} '.format(epoch, epochs, Loss.avg))
        metrics, datas, preds, targets = test(epoch, model, test_loader, criterion,
                                              writer, cuda)
        writer.add_scalar('TrainLoss', Loss.avg, epoch)
        if metrics[0] < metrics_best[0]:
            metrics_best = metrics
            torch.save(model.state_dict(),
                       './checkpoint/model.pt')
            # torch.save((datas, preds, targets), './checkpoint/datas_preds_targets.pt')
    # print the best result
    print('Best result:')
    print('\tMEANAE: {:.4f}'.format(metrics_best[0]), end=' ')
    print('MEDIANAE: {:.4f}'.format(metrics_best[1]))


def test(epoch, model, test_loader, criterion, writer,
         cuda):
    model.eval()
    datas, preds, targets = [], [], []
    with torch.no_grad():
        Loss = AverageMeter()
        for data, target in test_loader:
            if cuda:
                data, target = data.cuda(), target.cuda()
            pred = model(data, target.shape[1])
            loss = criterion(pred, target)
            Loss.update(loss.item(), data.size(0))
            preds.append(pred.cpu().numpy())
            datas.append(data.cpu().numpy())
            targets.append(target.cpu().numpy())
    
    datas = np.concatenate(datas, axis=0)
    preds = np.concatenate(preds, axis=0)
    targets = np.concatenate(targets, axis=0)
    metrics = get_metrics(preds, targets)
    writer.add_scalar('TestLoss', Loss.avg, epoch)
    writer.add_scalar('TestMEANAE', metrics[0], epoch)
    writer.add_scalar('TestMEDIANAE', metrics[1], epoch)
    # print the result
    print('Test result:')
    print('\tMEANAE: {:.4f}'.format(metrics[0]), end=' ')
    print('MEDIANAE: {:.4f}'.format(metrics[1]))
    # print(datas.shape, preds.shape)
    # plot the result
    show(epoch, datas, preds, targets, writer)
    return metrics, datas, preds, targets