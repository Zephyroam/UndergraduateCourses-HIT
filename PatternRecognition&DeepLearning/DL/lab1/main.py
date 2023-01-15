import torch
import torch.nn as nn
import torch.optim as optim
import data.mnist as mnist
import model.mlp as mlp
from tqdm import tqdm
import argparse
from utils import AverageMeter


def train(model, train_loader, optimizer, epochs, criterion, lr_scheduler,
          test_loader):
    model.train()
    acc_best = 0
    Losses = []
    Accs = []
    for epoch in range(epochs):
        losses = AverageMeter()
        for data, target in tqdm(train_loader):
            data, target = data.cuda(), target.cuda()
            data, target = data.view(-1, 784), target.view(-1)
            optimizer.zero_grad()
            output = model(data)
            loss = criterion(output, target)
            losses.update(loss.item())
            loss.backward()
            optimizer.step()

        lr_scheduler.step()
        print('Epoch {}/{} Loss: {:.4f} '.format(epoch, epochs, losses.avg))
        Losses.append(losses.avg)
        acc = test(model, test_loader)
        Accs.append(acc)
        if acc > acc_best:
            acc_best = acc
            torch.save(model.state_dict(), './checkpoint/model.pt')
    print('Best accuracy: {:.2f}%'.format(acc_best * 100))
    torch.save((Losses, Accs), './checkpoint/loss_acc_result.pt')


def test(model, test_loader):
    model.eval()
    correct = 0
    with torch.no_grad():
        for data, target in test_loader:
            data, target = data.cuda(), target.cuda()
            data, target = data.view(-1, 784), target.view(-1)
            output = model(data)
            pred = output.data.max(1, keepdim=True)[1]
            correct += pred.eq(target.data.view_as(pred)).cpu().sum()
    print('\tTest set: Accuracy: {}/{} ({:.2f}%)'.format(
        correct, len(test_loader.dataset),
        100. * correct / len(test_loader.dataset)))
    return correct / len(test_loader.dataset)


def main(batch_size, epochs, lr, hidden_size):
    train_loader, test_loader = mnist.mnist(batch_size)
    model = mlp.MLP(784, hidden_size, 10).cuda()
    optimizer = optim.SGD(model.parameters(), lr=lr)
    criterion = nn.CrossEntropyLoss().cuda()
    lr_scheduler = optim.lr_scheduler.StepLR(optimizer,
                                             step_size=epochs // 5,
                                             gamma=0.1)

    train(model, train_loader, optimizer, epochs, criterion, lr_scheduler,
          test_loader)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Lab1')
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
                        default=0.01,
                        help='learning rate (default: 0.01)')
    parser.add_argument('--hidden-size',
                        type=int,
                        default=128,
                        help='hidden size (default: 128)')
    args = parser.parse_args()
    batch_size = args.batch_size
    epochs = args.epochs
    lr = args.lr
    hidden_size = args.hidden_size
    main(batch_size, epochs, lr, hidden_size)
