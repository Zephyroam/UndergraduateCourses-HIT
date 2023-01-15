import argparse
import re
import os, glob, datetime, time
import numpy as np
import torch
import torch.nn as nn
from torch.nn.modules.loss import _Loss
import torch.nn.init as init
from torch.utils.data import DataLoader
import torch.optim as optim
from torch.optim.lr_scheduler import MultiStepLR
from data_generator import DenoisingDataset
from src.data_generator import datagenerator

parser = argparse.ArgumentParser(description='PyTorch DnCNN')
parser.add_argument('--batch_size', default=128, type=int, help='batch size')
parser.add_argument('--train_data', default='data/Train400', type=str, help='path of train data')
parser.add_argument('--sigma', default=25, type=int, help='noise level')
parser.add_argument('--epoch', default=50, type=int, help='number of train epoches')
parser.add_argument('--lr', default=1e-3, type=float, help='initial learning rate for Adam')
args = parser.parse_args()

batch_size = args.batch_size
cuda = torch.cuda.is_available()
n_epoch = args.epoch
sigma = args.sigma

save_dir = 'models/sigma' + str(sigma) + '_1'
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

if not os.path.exists(save_dir):
    os.mkdir(save_dir)


class DnCNN(nn.Module):
    def __init__(self, image_channels=1):
        super(DnCNN, self).__init__()

        layers = []
        layers.append(nn.Conv2d(in_channels=image_channels, out_channels=64, kernel_size=(3, 3), padding=1, bias=True))
        layers.append(nn.ReLU(inplace=True))
        for i in range(15):
            layers.append(nn.Conv2d(in_channels=64, out_channels=64, kernel_size=(3, 3), padding=1, bias=False))
            layers.append(nn.BatchNorm2d(64, eps=0.0001, momentum = 0.95))
            layers.append(nn.ReLU(inplace=True))
        layers.append(nn.Conv2d(in_channels=64, out_channels=image_channels, kernel_size=(3, 3), padding=1, bias=False))
        self.net = nn.Sequential(*layers)
        self._initialize_weights()

    def forward(self, x):
        y = x
        pred_noise = self.net(x)
        denoised_img = y - pred_noise
        return denoised_img

    def _initialize_weights(self):
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                init.orthogonal_(m.weight)
                if m.bias is not None:
                    init.constant_(m.bias, 0)
            elif isinstance(m, nn.BatchNorm2d):
                init.constant_(m.weight, 1)
                init.constant_(m.bias, 0)


class sum_squared_error(_Loss):
    def __init__(self, size_average=None, reduce=None, reduction='sum'):
        super(sum_squared_error, self).__init__(size_average, reduce, reduction)

    def forward(self, input, target):
        # return torch.sum(torch.pow(input-target,2), (0,1,2,3)).div_(2)
        return torch.nn.functional.mse_loss(input, target, size_average=None, reduce=None, reduction='sum').div_(2)


def recover_training():
    '''
    从上次训练的记录点恢复
    '''
    file_list = glob.glob(save_dir + '/model_*.pth')
    if file_list:
        epochs_exist = []
        for file_ in file_list:
            result = re.findall(".*model_(.*).pth.*", file_)
            epochs_exist.append(int(result[0]))
        initial_epoch = max(epochs_exist)
    else:
        initial_epoch = 0
    return initial_epoch


def log(*args, **kwargs):
     print(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S:"), *args, **kwargs)


def train():

    for epoch in range(initial_epoch, n_epoch):

        print('Learning rate = ' + str(optimizer.state_dict()['param_groups'][0]['lr']))
        data = datagenerator(data_dir=args.train_data)
        data = data.astype('float32') / 255.0
        data = torch.from_numpy(data.transpose((0, 3, 1, 2)))  # tensor of the clean patches, NXCXHXW
        dataset = DenoisingDataset(data, sigma)
        dataloader = DataLoader(dataset=dataset, num_workers=4, drop_last=True, batch_size=batch_size, shuffle=True)

        tic = time.time()
        batch_cnt = 0
        cnt = 0
        batch_loss = 0.
        total_loss = 0.
        for batch in dataloader:
            batch_cnt += 1
            cnt += 1

            optimizer.zero_grad()

            noise_img, ground = batch
            noise_img, ground = noise_img.to(device), ground.to(device)

            denoised_img = model(noise_img)
            loss = criterion(denoised_img, ground)
            batch_loss += loss.item()
            total_loss += loss.item()
            loss.backward()
            optimizer.step()

            if batch_cnt == 10:
                log_str = '[epoch %d, batch %d] loss = %4.4f' % (epoch + 1, cnt, batch_loss / batch_size / batch_cnt)
                print(log_str)
                f = open('log.txt', 'a')
                f.write(log_str + '\n')
                f.close()

                batch_loss = 0
                batch_cnt = 0

        toc = time.time()
        time_cost = toc - tic

        log_str_ = '[epoch %d] loss = %.4f, time = %.2fs' % (epoch + 1, total_loss / cnt / batch_size, time_cost)
        print(log_str_)
        f = open('log.txt', 'a')
        f.write(log_str_ + '\n')
        f.close()

        torch.save(model, save_dir + '/model_%03d.pth' % (epoch + 1))
        scheduler.step()


if __name__ == '__main__':
    model = DnCNN()

    initial_epoch = recover_training()  # load the last model in matconvnet style
    if initial_epoch > 0:
        print('resuming by loading epoch %03d' % initial_epoch)
        # model.load_state_dict(torch.load(os.path.join(save_dir, 'model_%03d.pth' % initial_epoch)))
        model = torch.load(os.path.join(save_dir, 'model_%03d.pth' % initial_epoch))
    model.train()
    model = model.to(device)

    criterion = sum_squared_error()
    optimizer = optim.Adam(model.parameters(), lr=args.lr)
    scheduler = MultiStepLR(optimizer, milestones=[30, 60, 90], gamma=0.2)

    for i in range(initial_epoch):
        scheduler.step()

    train()
