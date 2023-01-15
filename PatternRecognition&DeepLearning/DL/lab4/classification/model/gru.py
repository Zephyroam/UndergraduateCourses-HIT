import torch
import torch.nn as nn
import torch.nn.functional as F


class GRU(nn.Module):

    def __init__(self, input_size, hidden_size, device):
        super(GRU, self).__init__()
        self.hidden_size = hidden_size
        self.device = device

        self.x2h = nn.Linear(input_size, 3 * hidden_size)
        self.h2h = nn.Linear(hidden_size, 3 * hidden_size)

    def _forward(self, x, h):
        x_t = self.x2h(x)
        h_t = self.h2h(h)

        x_reset, x_upd, x_new = x_t.chunk(3, 1)
        h_reset, h_upd, h_new = h_t.chunk(3, 1)

        reset_gate = torch.sigmoid(x_reset + h_reset)
        update_gate = torch.sigmoid(x_upd + h_upd)
        new_gate = torch.tanh(x_new + (reset_gate * h_new))

        return update_gate * h + (1 - update_gate) * new_gate

    def forward(self, input):
        output = []
        hidden = self.init_hidden(input.size(0))
        for t in range(input.size(1)):
            hidden = self._forward(input[:, t, :], hidden)
            output.append(hidden)
        return torch.stack(output, dim=1)

    def init_hidden(self, batch_size):
        return torch.zeros(batch_size, self.hidden_size).to(self.device)


class GRUClassifier(nn.Module):

    def __init__(self,
                 word_num,
                 embedding_size,
                 hidden_size=64,
                 max_length=64,
                 num_classes=10,
                 device='cuda'):
        super(GRUClassifier, self).__init__()
        self.embedding = nn.Embedding(word_num, embedding_size)
        self.gru = GRU(embedding_size, hidden_size, device)
        self.output_size = hidden_size * max_length
        self.fc = nn.Linear(self.output_size, num_classes)

    def forward(self, input):
        input = self.embedding(input)
        output = self.gru(input)
        output = self.fc(output.contiguous().view(-1, self.output_size))
        output = F.log_softmax(output, dim=1)
        return output