import numpy as np
import matplotlib.pyplot as plt

def uci_data_iris(file_path):
    with open(file_path, 'r') as f:
        label_map, label_map_counter = {}, 0
        lines = [l[:-1].split(',') for l in f.readlines()]
        for line in lines:
            x = line[-1]
            if x not in label_map.keys():
                label_map_counter += 1
                label_map[x] = label_map_counter
        np.random.shuffle(lines)
    X = np.array([[float(l[i]) for i in range(len(l) - 1)] for l in lines])
    Y = np.array([label_map[l[i]] - 1 for l in lines for i in range(len(l) - 1, len(l))])
    return X, Y

if __name__ == '__main__':
    file_path = './bezdekIris.data'
    dataset = uci_data_iris(file_path)
