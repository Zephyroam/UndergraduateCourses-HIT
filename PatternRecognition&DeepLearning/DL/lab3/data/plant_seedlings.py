import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from torch.utils.data import TensorDataset, DataLoader, random_split
from torchvision import transforms
import os
import torch
import cv2
from glob import glob
import math
from tqdm import tqdm
import re
from PIL import Image

data_path = './data/plant-seedlings-classification/'


def load_dataset(files, img_size, label, data_transforms=['hf', 'vf', 'r']):
    imgs = []
    labels = []
    test_ids = []

    transform = [transforms.ToTensor(), transforms.Resize((img_size, img_size))]
    if 'hf' in data_transforms:
        transform.append(transforms.RandomHorizontalFlip())
    if 'vf' in data_transforms:
        transform.append(transforms.RandomVerticalFlip())
    if 'r' in data_transforms:
        transform.append(transforms.RandomRotation(180))
    transform = transforms.Compose(transform)

    if label:
        for img in tqdm(files):
            imgs.append(transform(Image.open(img).convert('RGB')))
            labels.append(re.split('/|\\\\', img)[-2])
    else:
        for img in tqdm(files):
            imgs.append(transform(Image.open(img).convert('RGB')))
            test_ids.append(re.split('/|\\\\', img)[-1])

    imgs = torch.stack(imgs)

    if label:
        labels = np.array(labels)
        specie2idx = {
            specie: idx
            for idx, specie in enumerate(np.unique(labels))
        }
        idx2specie = {
            idx: specie
            for idx, specie in enumerate(np.unique(labels))
        }

        labels = np.array([specie2idx[specie] for specie in labels])
        labels = torch.from_numpy(labels).long()
        dataset = TensorDataset(imgs, labels)
        return dataset, idx2specie
    else:
        dataset = TensorDataset(imgs)
        return dataset, test_ids


def plant_seedlings(img_size=224, batch_size=256, data_transforms=['hf', 'vf', 'r']):
    print('Loading data...')
    print('data transforms:', data_transforms)
    data_dump_path = './data/plant-seedlings-classification_{}_{}.pt'.format(
        img_size, data_transforms)
    if os.path.exists(data_dump_path):
        print('Loading data from {}'.format(data_dump_path))
        train_data, val_data, test_data, test_ids, idx2specie = torch.load(
            data_dump_path)
    else:
        print('Loading data from {}'.format(data_path))
        path_train = data_path + 'train/*/*.png'
        path_test = data_path + 'test/*.png'
        files_train = glob(path_train)
        files_test = glob(path_test)

        dataset, idx2specie = load_dataset(files_train, img_size, True, data_transforms)
        train_data, val_data = random_split(
            dataset, [len(dataset) - len(dataset) // 10,
                      len(dataset) // 10])
        test_data, test_ids = load_dataset(files_test, img_size, False, data_transforms)

        torch.save((train_data, val_data, test_data, test_ids, idx2specie),
                   data_dump_path)

    # Create DataLoaders
    train_loader = DataLoader(train_data, batch_size=batch_size, shuffle=True)
    val_loader = DataLoader(val_data, batch_size=batch_size, shuffle=True)
    test_loader = DataLoader(test_data, batch_size=batch_size, shuffle=False)

    return train_loader, val_loader, test_loader, test_ids, idx2specie


if __name__ == '__main__':
    train_loader, val_loader, test_loader, test_ids, idx2specie = plant_seedlings(
    )
    print(idx2specie)