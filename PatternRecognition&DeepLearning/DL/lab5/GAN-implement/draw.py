import matplotlib.pyplot as plt
import numpy as np
import torch

x_min, x_max, y_min, y_max = -0.5, 1.5, 0, 1

# draw scatter
def draw_scatter(true_imgs, gen_imgs):
    plt.scatter(true_imgs[:, 0], true_imgs[:, 1], c='r', s=10)
    plt.scatter(gen_imgs[:, 0], gen_imgs[:, 1], c='b', s=10)

# draw the background
def draw_background(model_D):
    plt.xlim(x_min, x_max)
    plt.ylim(y_min, y_max)
    xline = np.linspace(x_min, x_max, 100)
    yline = np.linspace(y_min, y_max, 100)
    bg = np.array([(x, y) for x in xline for y in yline])
    color = model_D(torch.Tensor(bg).cuda()).cpu()
    color = (color - color.min()) / (color.max() - color.min())
    # turn color into 0 or 1
    color = torch.tensor(np.array([0 if x[0] < 0.5 else 1
                                   for x in color])).reshape(-1, 1)

    sc = plt.scatter(bg[:, 0],
                     bg[:, 1],
                     c=np.squeeze(color),
                     cmap=plt.cm.get_cmap('gray'))
    plt.colorbar(sc)
