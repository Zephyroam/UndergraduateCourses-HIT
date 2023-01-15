import argparse
import os, time
import numpy as np
import torch.nn as nn
import torch.nn.init as init
import torch
from skimage.measure import compare_psnr, compare_ssim
from skimage.io import imread, imsave


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--set_dir', default='data/Test', type=str, help='directory of test dataset')
    parser.add_argument('--set_names', default=['Set68', 'Set12'], help='directory of test dataset')
    parser.add_argument('--sigma', default=25, type=int, help='noise level')
    parser.add_argument('--model_dir', default='models/sigma25', help='directory of the model')
    parser.add_argument('--model_name', default='model_050.pth', type=str, help='the model name')
    parser.add_argument('--result_dir', default='results', type=str, help='directory of test dataset')
    parser.add_argument('--save_result', default=0, type=int, help='save the denoised image, 1 or 0')
    return parser.parse_args()


def save_result(result, path):
    path = path if path.find('.') != -1 else path+'.png'
    ext = os.path.splitext(path)[-1]
    if ext in ('.txt', '.dlm'):
        np.savetxt(path, result, fmt='%2.4f')
    else:
        imsave(path, np.clip(result, 0, 1))


def show(x, title=None, cbar=False, figsize=None):
    import matplotlib.pyplot as plt
    plt.figure(figsize=figsize)
    plt.imshow(x, interpolation='nearest', cmap='gray')
    if title:
        plt.title(title)
    if cbar:
        plt.colorbar()
    plt.show()


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


def test():

    for set_cur in args.set_names:

        if not os.path.exists(os.path.join(args.result_dir, set_cur)):
            os.mkdir(os.path.join(args.result_dir, set_cur))
        psnr_list = []
        ssim_list = []

        for im in os.listdir(os.path.join(args.set_dir, set_cur)):
            if im.endswith(".jpg") or im.endswith(".bmp") or im.endswith(".png"):

                x = np.array(imread(os.path.join(args.set_dir, set_cur, im)), dtype=np.float32)/255.0
                np.random.seed(seed=0)  # for reproducibility
                y = x + np.random.normal(0, args.sigma/255.0, x.shape)  # Add Gaussian noise without clipping
                y = y.astype(np.float32)
                y_ = torch.from_numpy(y).view(1, -1, y.shape[0], y.shape[1])

                torch.cuda.synchronize()
                start_time = time.time()
                y_ = y_.cuda()
                x_ = model(y_)  # inference
                x_ = x_.view(y.shape[0], y.shape[1])
                x_ = x_.cpu()
                x_ = x_.detach().numpy().astype(np.float32)
                torch.cuda.synchronize()
                elapsed_time = time.time() - start_time
                print('%10s : %10s : %2.4f second' % (set_cur, im, elapsed_time))

                psnr_x_ = compare_psnr(x, x_)
                ssim_x_ = compare_ssim(x, x_)
                if args.save_result:
                    name, ext = os.path.splitext(im)
                    show(np.hstack((y, x_)))  # show the image
                    save_result(x_, path=os.path.join(args.result_dir, set_cur, name+'_dncnn'+ext))  # save the denoised image
                psnr_list.append(psnr_x_)
                ssim_list.append(ssim_x_)
        psnr_avg = np.mean(psnr_list)
        ssim_avg = np.mean(ssim_list)
        psnr_list.append(psnr_avg)
        ssim_list.append(ssim_avg)
        if args.save_result:
            save_result(np.hstack((psnr_list, ssim_list)), path=os.path.join(args.result_dir, set_cur, 'results.txt'))
        print('[Dataset: %s] PSNR = %.2fdB, SSIM = %.4f' % (set_cur, psnr_avg, ssim_avg))


if __name__ == '__main__':

    args = parse_args()

    if not os.path.exists(os.path.join(args.model_dir, args.model_name)):
        model = torch.load(os.path.join(args.model_dir, 'model.pth'))
    else:
        model = torch.load(os.path.join(args.model_dir, args.model_name))

    model.eval()

    if torch.cuda.is_available():
        model = model.cuda()

    if not os.path.exists(args.result_dir):
        os.mkdir(args.result_dir)

    test()
