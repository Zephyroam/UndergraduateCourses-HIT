import numpy as np

def generate_swiss_roll(n_sample=2000, noise=0.1, theta=70 * np.pi / 180):
    t = 1.5 * np.pi * (1 + 2 * np.random.rand(1, n_sample))
    x = t * np.cos(t)
    y = np.random.rand(1, n_sample)
    z = t * np.sin(t)
    X = np.concatenate((x, y, z))
    X += noise * np.random.randn(3, n_sample)
    rotate = np.array([[np.cos(theta), 0, np.sin(theta)], [0, 1, 0], [-np.sin(theta), 0, np.cos(theta)]])
    return np.dot(rotate, X).T