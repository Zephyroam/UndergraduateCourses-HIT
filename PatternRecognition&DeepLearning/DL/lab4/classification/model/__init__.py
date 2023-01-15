from model.gru import *
from model.lstm import *
from model.rnn import *
from model.bilstm import *


def get_model(model):
    if model == 'RNN':
        return RNNClassifier
    elif model == 'GRU':
        return GRUClassifier
    elif model == 'LSTM':
        return LSTMClassifier
    elif model == 'BiLSTM':
        return BiLSTMClassifier
    raise ValueError('Model {} not supported'.format(model))