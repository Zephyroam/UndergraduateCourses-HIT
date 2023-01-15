import torch
import torch.nn as nn
import torch.optim as optim
from data import *
from model import *
from tqdm import tqdm
import numpy as np
import matplotlib.pyplot as plt
import torch.autograd as autograd
import random
from draw import *
from utils import *

sample_interval = 10


def get_optimizer(model, optimizer, lr):
    """Return optimizer"""
    if optimizer == 'Adam':
        return optim.Adam(model.parameters(), lr=lr, betas=(0.5, 0.999))
    elif optimizer == 'RMSprop':
        return optim.RMSprop(model.parameters(), lr=lr)
    elif optimizer == 'SGD':
        return optim.SGD(model.parameters(), lr=lr)
    elif optimizer == None:
        if isinstance(model, GAN_D) or isinstance(model, GAN_G):
            return get_optimizer(model, 'Adam', lr)
        else:
            return get_optimizer(model, 'RMSprop', lr)
    else:
        raise ValueError('Unknown optimizer {}'.format(optimizer))


def train_GAN(model_G, model_D, data_loader, lr, optimizer, epochs, n_noise,
              cuda):
    ini_path('./images/GAN/{}/'.format(optimizer))
    optimizer_D = get_optimizer(model_D, optimizer, lr)
    optimizer_G = get_optimizer(model_G, optimizer, lr)
    # Loss function
    adversarial_loss = nn.BCELoss()
    for epoch in range(epochs):
        Loss_G = AverageMeter()
        Loss_D = AverageMeter()
        for i, true_imgs in enumerate(data_loader):
            true_imgs = true_imgs[0]

            # Adversarial ground truths, Sample noise as generator input
            valid = torch.ones((true_imgs.size(0), 1))
            fake = torch.zeros((true_imgs.size(0), 1))
            noise = torch.randn([true_imgs.size(0), n_noise])

            if cuda:
                true_imgs = true_imgs.cuda()
                valid = valid.cuda()
                fake = fake.cuda()
                noise = noise.cuda()

            # Train the discriminator
            gen_imgs = model_G(noise).detach()
            optimizer_D.zero_grad()
            real_loss = adversarial_loss(model_D(true_imgs), valid)
            fake_loss = adversarial_loss(model_D(gen_imgs), fake)
            loss_D = (real_loss + fake_loss) / 2
            Loss_D.update(loss_D.item(), gen_imgs.size(0))
            loss_D.backward()
            optimizer_D.step()

            # Train the generator
            gen_imgs = model_G(noise)
            optimizer_G.zero_grad()
            loss_G = adversarial_loss(model_D(gen_imgs), valid)
            Loss_G.update(loss_G.item(), gen_imgs.size(0))
            loss_G.backward()
            optimizer_G.step()

        print("[Epoch %d/%d] [D loss: %f] [G loss: %f]" %
              (epoch, epochs, Loss_D.avg, Loss_G.avg))

        if (epoch + 1) % sample_interval == 0:
            file_name = './images/GAN/{}/'.format(optimizer) + str(epoch +
                                                                   1) + '.png'
            test(model_G, model_D, data_loader, n_noise, file_name)
    torch.save(model_G.state_dict(),
               './snapshot/GAN/{}/G.pth'.format(optimizer))
    torch.save(model_D.state_dict(),
               './snapshot/GAN/{}/D.pth'.format(optimizer))


def train_WGAN(model_G,
               model_D,
               data_loader,
               lr,
               optimizer,
               epochs,
               n_noise=2,
               clamp=0.01,
               cuda=True):
    ini_path('./images/WGAN/{}/'.format(optimizer))
    optimizer_D = get_optimizer(model_D, optimizer, lr)
    optimizer_G = get_optimizer(model_G, optimizer, lr)
    for epoch in range(epochs):
        Loss_G = AverageMeter()
        Loss_D = AverageMeter()
        for i, true_imgs in enumerate(data_loader):
            true_imgs = true_imgs[0]

            # Sample noise as generator input
            noise = torch.randn([true_imgs.size(0), n_noise])

            if cuda:
                true_imgs = true_imgs.cuda()
                noise = noise.cuda()

            # Train the discriminator
            gen_imgs = model_G(noise).detach()
            optimizer_D.zero_grad()
            loss_D = -torch.mean(model_D(true_imgs)) + torch.mean(
                model_D(gen_imgs))
            Loss_D.update(loss_D.item(), gen_imgs.size(0))
            loss_D.backward()
            optimizer_D.step()

            # Clip weights of discriminator
            for p in model_D.parameters():
                p.data.clamp_(-clamp, clamp)

            # Train the generator
            gen_imgs = model_G(noise)
            optimizer_G.zero_grad()
            loss_G = -torch.mean(model_D(gen_imgs))
            Loss_G.update(loss_G.item(), gen_imgs.size(0))
            loss_G.backward()
            optimizer_G.step()

        print("[Epoch %d/%d] [D loss: %f] [G loss: %f]" %
              (epoch, epochs, Loss_D.avg, Loss_G.avg))

        if (epoch + 1) % sample_interval == 0:
            file_name = './images/WGAN/{}/'.format(optimizer) + str(epoch +
                                                                    1) + '.png'
            test(model_G, model_D, data_loader, n_noise, file_name)
    torch.save(model_G.state_dict(),
               './snapshot/WGAN/{}/G.pth'.format(optimizer))
    torch.save(model_D.state_dict(),
               './snapshot/WGAN/{}/D.pth'.format(optimizer))


def compute_gradient_penalty(D, real_samples, fake_samples, cuda):
    """Calculates the gradient penalty loss for WGAN GP"""
    alpha = random.random()
    interpolates = (alpha * real_samples +
                    ((1 - alpha) * fake_samples)).requires_grad_(True)
    d_interpolates = D(interpolates)
    fake = torch.ones(d_interpolates.size())
    if cuda:
        fake = fake.cuda()
    gradients = autograd.grad(
        outputs=d_interpolates,
        inputs=interpolates,
        grad_outputs=fake,
        create_graph=True,
        retain_graph=True,
        only_inputs=True,
    )[0]
    gradients = gradients.view(gradients.size(0), -1)
    gradient_penalty = ((gradients.norm(2, dim=1) - 1)**2).mean()
    return gradient_penalty


def train_WGAN_GP(model_G,
                  model_D,
                  data_loader,
                  lr,
                  optimizer,
                  epochs,
                  n_noise=2,
                  cuda=True):
    ini_path('./images/WGAN-GP/{}/'.format(optimizer))
    optimizer_D = get_optimizer(model_D, optimizer, lr)
    optimizer_G = get_optimizer(model_G, optimizer, lr)
    lambda_gp = 0.001
    for epoch in range(epochs):
        Loss_G = AverageMeter()
        Loss_D = AverageMeter()
        for i, true_imgs in enumerate(data_loader):
            true_imgs = true_imgs[0]

            # Sample noise as generator input
            noise = torch.randn([true_imgs.size(0), n_noise])

            if cuda:
                true_imgs = true_imgs.cuda()
                noise = noise.cuda()

            # Train the discriminator
            gen_imgs = model_G(noise)
            optimizer_D.zero_grad()
            # Gradient penalty
            gradient_penalty = compute_gradient_penalty(
                model_D, true_imgs.data, gen_imgs.data, cuda)
            loss_D = -torch.mean(model_D(true_imgs)) + torch.mean(
                model_D(gen_imgs)) + lambda_gp * gradient_penalty
            Loss_D.update(loss_D.item(), gen_imgs.size(0))
            loss_D.backward()
            optimizer_D.step()

            # Train the generator
            gen_imgs = model_G(noise)
            optimizer_G.zero_grad()
            loss_G = -torch.mean(model_D(gen_imgs))
            Loss_G.update(loss_G.item(), gen_imgs.size(0))
            loss_G.backward()
            optimizer_G.step()

        print("[Epoch %d/%d] [D loss: %f] [G loss: %f]" %
              (epoch, epochs, Loss_D.avg, Loss_G.avg))

        if (epoch + 1) % sample_interval == 0:
            file_name = './images/WGAN-GP/{}/'.format(optimizer) + str(
                epoch + 1) + '.png'
            test(model_G, model_D, data_loader, n_noise, file_name)
    torch.save(model_G.state_dict(),
               './snapshot/WGAN-GP/{}/G.pth'.format(optimizer))
    torch.save(model_D.state_dict(),
               './snapshot/WGAN-GP/{}/D.pth'.format(optimizer))


def test(model_G, model_D, data_loader, n_noise, file_name):
    with torch.no_grad():
        plt.clf()
        true_imgs = data_loader.dataset.tensors[0]
        gen_imgs = torch.randn([8192, n_noise])
        gen_imgs = gen_imgs.cuda()
        gen_imgs = model_G(gen_imgs).cpu()

        draw_background(model_D)
        draw_scatter(true_imgs, gen_imgs)

        plt.savefig(file_name, bbox_inches='tight', dpi=300, pad_inches=0.0)
        # plt.show()