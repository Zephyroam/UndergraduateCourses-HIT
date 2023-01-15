import torch
import torch.nn as nn
import torch.optim as optim
from data import *
from model import *
from tqdm import tqdm
from sklearn.metrics import accuracy_score, recall_score, f1_score
import numpy as np
from utils import *


def get_metrics(preds, targets):
    "get metrics"
    acc = accuracy_score(targets, preds)
    recall = recall_score(targets, preds, average='macro')
    f1 = f1_score(targets, preds, average='macro')
    return acc, recall, f1


def train(model_name, model, train_loader, optimizer, epochs, criterion,
          lr_scheduler, test_loader, writer, cuda):
    metrics_best = (0, 0, 0)
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

        lr_scheduler.step()
        print('Epoch {}/{} Loss: {:.4f} '.format(epoch, epochs, Loss.avg))
        metrics = test(epoch, model, test_loader, criterion, writer, cuda)
        writer.add_scalar('TrainLoss', Loss.avg, epoch)
        if metrics[0] > metrics_best[0]:
            metrics_best = metrics
            torch.save(model.state_dict(),
                       './checkpoint/model_{}.pt'.format(model_name))
    print('\tBest accuracy: {:.4f}'.format(metrics_best[0]), end=' ')
    print('Best recall: {:.4f}'.format(metrics_best[1]), end=' ')
    print('Best f1: {:.4f}'.format(metrics_best[2]))


def test(epoch, model, test_loader, criterion, writer, cuda):
    model.eval()
    preds, targets = [], []
    with torch.no_grad():
        Loss = AverageMeter()
        for data, target in test_loader:
            if cuda:
                data, target = data.cuda(), target.cuda()
            output = model(data)
            loss = criterion(output, target)
            Loss.update(loss.item(), data.size(0))
            pred = output.data.max(1, keepdim=True)[1]
            preds.append(pred.cpu().numpy())
            targets.append(target.cpu().numpy())
    preds = np.concatenate(preds, axis=0)
    targets = np.concatenate(targets, axis=0)
    metrics = get_metrics(preds, targets)
    # save metrics
    writer.add_scalar('TestLoss', Loss.avg, epoch)
    writer.add_scalar('TestAccuracy', metrics[0], epoch)
    writer.add_scalar('TestRecall', metrics[1], epoch)
    writer.add_scalar('TestF1', metrics[2], epoch)
    print('Test result:')
    print('\tAccuracy: {:.4f}'.format(metrics[0]), end=' ')
    print('Recall: {:.4f}'.format(metrics[1]), end=' ')
    print('F1: {:.4f}'.format(metrics[2]))
    return metrics