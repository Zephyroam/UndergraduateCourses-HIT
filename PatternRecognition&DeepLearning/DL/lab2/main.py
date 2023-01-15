import torch
import torch.nn as nn
import torch.optim as optim
import data.caltech101 as caltech101
import model.alexnet as alexnet
from tqdm import tqdm
import argparse
from utils import AverageMeter
from torch.utils.tensorboard import SummaryWriter


def train(model, train_loader, optimizer, epochs, criterion, lr_scheduler,
          test_loader, cuda):
    # Set model to training mode
    model.train()
    # Initialize accuracy
    acc_best = 0
    # Iterate over epochs
    for epoch in range(epochs):
        # Initialize loss
        Loss = AverageMeter()
        # Iterate over batches
        for data, target in tqdm(train_loader):
            # Move data to GPU
            if cuda:
                data, target = data.cuda(), target.cuda()
            # Zero out gradients
            optimizer.zero_grad()
            # Forward pass
            output = model(data)
            # Calculate loss
            loss = criterion(output, target)
            # Record loss
            Loss.update(loss.item(), data.size(0))
            # Backward pass
            loss.backward()
            # Update weights
            optimizer.step()
        # Update learning rate
        lr_scheduler.step()
        print('Epoch {}/{} Loss: {:.4f} '.format(epoch, epochs, Loss.avg))
        writer.add_scalar('Train Loss', Loss.avg, epoch)
        # Evaluate model
        acc = test(epoch, model, test_loader, criterion, cuda)
        # Save model if accuracy is best
        if acc > acc_best:
            acc_best = acc
            torch.save(model.state_dict(), './checkpoint/model.pt')
    print('Best accuracy: {:.2f}%'.format(acc_best * 100))


def test(epoch, model, test_loader, criterion, cuda):
    # Set model to evaluation mode
    model.eval()
    # Initialize accuracy
    correct = 0
    with torch.no_grad():
        Loss = AverageMeter()
        for data, target in test_loader:
            # Move data to GPU
            if cuda:
                data, target = data.cuda(), target.cuda()
            # Forward pass
            output = model(data)
            # Calculate loss
            loss = criterion(output, target)
            # Record loss
            Loss.update(loss.item(), data.size(0))
            # Get predictions
            pred = output.data.max(1, keepdim=True)[1]
            correct += pred.eq(target.data.view_as(pred)).cpu().sum().item()
    writer.add_scalar('Test Loss', Loss.avg, epoch)
    writer.add_scalar('Test Acc', correct / len(test_loader.dataset), epoch)
    print('Test set: Accuracy: {}/{} ({:.2f}%)'.format(
        correct, len(test_loader.dataset),
        100. * correct / len(test_loader.dataset)))
    return correct / len(test_loader.dataset)


def main(batch_size, epochs, lr, img_size, cuda):
    # Get data loaders
    train_loader, val_loader, test_loader = caltech101.caltech101(
        batch_size, img_size)
    # Initialize model
    model = alexnet.AlexNet(101)
    if cuda:
        model = model.cuda()

    # Initialize optimizer
    optimizer = optim.Adam(model.parameters(), lr=lr)
    # Initialize learning rate scheduler
    criterion = nn.CrossEntropyLoss()
    if cuda:
        criterion = criterion.cuda()
    lr_scheduler = optim.lr_scheduler.StepLR(optimizer,
                                             step_size=epochs // 5,
                                             gamma=0.1)
    # Train model
    train(model, train_loader, optimizer, epochs, criterion, lr_scheduler,
          test_loader, cuda)


if __name__ == '__main__':

    # Parse arguments
    parser = argparse.ArgumentParser(description='Lab2')
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
    parser.add_argument('--img-size',
                        type=int,
                        default=64,
                        help='size of input image (default: 64)')
    parser.add_argument('--no-cuda',
                        action='store_true',
                        default=False,
                        help='disables CUDA training')
    args = parser.parse_args()

    # Initialize Tensorboard
    writer = SummaryWriter('./log/img_size_{}'.format(args.img_size))

    # Run
    cuda = not args.no_cuda and torch.cuda.is_available()
    img_size = (args.img_size, args.img_size)
    main(args.batch_size, args.epochs, args.lr, img_size, cuda)
