import torch
import torchvision
from torchvision import transforms
from PIL import Image
from tqdm import tqdm
import os
import matplotlib.pyplot as plt
import numpy as np


def caltech101(batch_size, img_size=(224, 224)):
    print('Loading Caltech101 dataset...')

    data_path = 'data/caltech101_{}.pt'.format(img_size[0])

    # Load the dataset
    if os.path.exists(data_path):
        print('Caltech101 dataset already exists. Loading...')
        train_data, val_data, test_data = torch.load(data_path)
    else:
        # Define transforms
        transform = transforms.Compose([
            transforms.Resize(img_size),
            transforms.ToTensor(),
            transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
        ])

        # Load the dataset
        data = torchvision.datasets.ImageFolder(root='./data/caltech-101')

        # Split the dataset
        data_by_class = [[] for _ in range(len(data.classes))]
        for x, y in data.samples:
            data_by_class[y].append(x)

        X_train, X_val, X_test, y_train, y_val, y_test = [], [], [], [], [], []
        for i, img in tqdm(enumerate(data_by_class)):
            n_train = int(len(img) * 0.8)
            n_val = int(len(img) * 0.1)
            for x in img[:n_train]:
                X_train.append(transform(Image.open(x).convert('RGB')))
                y_train.append(i)
            for x in img[n_train:n_train + n_val]:
                X_val.append(transform(Image.open(x).convert('RGB')))
                y_val.append(i)
            for x in img[n_train + n_val:]:
                X_test.append(transform(Image.open(x).convert('RGB')))
                y_test.append(i)

        # Save the dataset
        train_data = torch.utils.data.TensorDataset(torch.stack(X_train),
                                                    torch.LongTensor(y_train))
        val_data = torch.utils.data.TensorDataset(torch.stack(X_val),
                                                  torch.LongTensor(y_val))
        test_data = torch.utils.data.TensorDataset(torch.stack(X_test),
                                                   torch.LongTensor(y_test))
        torch.save((train_data, val_data, test_data), data_path)

    # Create the dataloaders
    train_loader = torch.utils.data.DataLoader(train_data,
                                               batch_size=batch_size,
                                               shuffle=True)
    val_loader = torch.utils.data.DataLoader(val_data,
                                             batch_size=batch_size,
                                             shuffle=False)
    test_loader = torch.utils.data.DataLoader(test_data,
                                              batch_size=batch_size,
                                              shuffle=False)

    return train_loader, val_loader, test_loader


def imgshow(img):
    img = img / 2 + 0.5
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))
    plt.show()


if __name__ == '__main__':
    train_loader, val_loader, test_loader = caltech101()
    x, y = next(iter(train_loader))
    out = torchvision.utils.make_grid(x)
    imgshow(out)