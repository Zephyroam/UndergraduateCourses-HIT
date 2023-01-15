import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np


class WGAN_G(nn.Module):

    def __init__(self, n_noise=2, img_shape=(2, )):
        super(WGAN_G, self).__init__()

        self.img_shape = img_shape

        self.model = nn.Sequential(
            nn.Linear(n_noise, 128),
            nn.ReLU(True),
            nn.Linear(128, 256),
            nn.ReLU(True),
            nn.Linear(256, 512),
            nn.ReLU(True),
            nn.Linear(512, int(np.prod(img_shape))),)

    def forward(self, z):
        img = self.model(z)
        img = img.view(img.shape[0], *self.img_shape)
        return img


class WGAN_D(nn.Module):

    def __init__(self, img_shape=(2, )):
        super(WGAN_D, self).__init__()

        self.model = nn.Sequential(
            nn.Linear(int(np.prod(img_shape)), 512),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Linear(512, 256),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Linear(256, 1),
        )

    def forward(self, img):
        img_flat = img.view(img.shape[0], -1)
        validity = self.model(img_flat)
        return validity
