import numpy as np
from data import *
from utils import *
from ucidata import *

class GMM:
    def __init__(self, x, K, epochs=100, delta=1e-7):
        # x: data
        # K: number of clusters
        # epochs: 最大迭代次数
        # delta: 停止迭代的阈值
        self.delta = delta
        self.x = x
        self.K = K
        self.epochs = epochs
        self._sigma = np.array([0.1 * np.eye(self.x.shape[1])] * K)
        self._mu = x[np.random.choice(range(x.shape[0]), k, replace=False)]
        self._alpha = np.ones(k) * (1.0 / k)

    def E_step(self):
        self._gamma = np.zeros((self.x.shape[0], self.K))
        for j in range(self.x.shape[0]):
            now_sum = 0
            for k in range(self.K):
                self._gamma[j][k] = self._alpha[k] * phi_func(self.x[j], self._mu[k], self._sigma[k])
                now_sum += self._gamma[j][k]
            self._gamma[j] /= now_sum
    
    def M_step(self):
        _mu, _sigma, _alpha = np.zeros((self.K, self.x.shape[1])), np.array([0.1 * np.eye(self.x.shape[1])] * self.K), np.zeros((self.K))
        for k in range(self.K):
            sum_gamma, sum_gamma_x, sum_gamma_x_mu = 0, 0, np.zeros((self.x.shape[1], self.x.shape[1]))
            for j in range(self.x.shape[0]):
                sum_gamma += self._gamma[j][k]
                sum_gamma_x += self._gamma[j][k] * self.x[j]
                v = (self.x[j] - self._mu[k]).reshape(-1, 1)
                sum_gamma_x_mu += self._gamma[j][k] * np.dot(v, v.T)
            _mu[k] = sum_gamma_x / sum_gamma
            _sigma[k] = sum_gamma_x_mu / sum_gamma
            _alpha[k] = sum_gamma  / self.x.shape[0]
        self._mu, self._sigma, self._alpha = _mu, _sigma, _alpha

    def GMM(self):
        likelihood = 0
        for epoch in range(self.epochs):
            self.E_step()
            self.M_step()
            new_likelihood = log_likelihood(self.x, self._alpha, self._mu, self._sigma, self._gamma)
            if abs(new_likelihood - likelihood) < self.delta:
                break
            likelihood = new_likelihood
        return np.argmax(self._gamma, axis=1), epoch

# 画图
def show(dataset, y_pred, epoch):
    X, Y = dataset
    k = max(y for y in Y) + 1
    colors = ['r', 'g', 'y', 'b']
    for i in range(k):
        ix = np.where(y_pred == i)
        plt.plot(X[ix, 0], X[ix, 1], '.', color=colors[i])
    plt.title('epoch={}, acc={:.2f}'.format(epoch, get_acc(dataset[1], y_pred) * 100))
    plt.legend()
    plt.show()

if __name__ == '__main__':
    k, size, loc, cov = get_data_option(size=200)
    dataset = data_generator(k, size, loc, cov)
    y_pred, epoch = GMM(dataset[0], k).GMM()
    show(dataset, y_pred, epoch)

    file_path = './bezdekIris.data'
    dataset = uci_data_iris(file_path)
    y_pred, epoch = GMM(dataset[0], k).GMM()
    print('epoch={}, acc={:.2f}%'.format(epoch, get_acc(dataset[1], y_pred) * 100))