import torch
import torch.nn as nn
import torch.nn.functional as F
from tqdm import tqdm, trange
class LSTM(nn.Module):

    def __init__(self, input_size, hidden_size, device):
        super(LSTM, self).__init__()
        self.hidden_size = hidden_size
        self.device = device

        self.x2h = nn.Linear(input_size, 4 * hidden_size)
        self.h2h = nn.Linear(hidden_size, 4 * hidden_size)

    def _forward(self, x, h):
        h, cx = h

        gates = self.x2h(x) + self.h2h(h)

        input_gate, forget_gate, cell_gate, output_gate = gates.chunk(4, 1)

        i_t = torch.sigmoid(input_gate)
        f_t = torch.sigmoid(forget_gate)
        g_t = torch.tanh(cell_gate)
        o_t = torch.sigmoid(output_gate)

        cy = cx * f_t + i_t * g_t

        hy = o_t * torch.tanh(cy)

        return (hy, cy)

    def forward(self, input):
        output = []
        hidden = self.init_hidden(input.size(0))
        hidden = (hidden, hidden)
        for t in range(input.size(1)):
            hidden = self._forward(input[:, t, :], hidden)
            output.append(hidden[0])
        return torch.stack(output, dim=1)

    def init_hidden(self, batch_size):
        return torch.zeros(batch_size, self.hidden_size).to(self.device)


class LSTMRegressor(nn.Module):

    def __init__(self,
                 input_size,
                 target_size,
                 length,
                 hidden_size=32,
                 device='cuda'):
        super(LSTMRegressor, self).__init__()
        self.output_size = hidden_size * length
        self.lstm = LSTM(input_size, hidden_size, device=device)
        self.fc1 = nn.Linear(self.output_size, 2048)
        self.relu = torch.nn.ReLU()
        self.fc2 = torch.nn.Linear(2048, 256)
        self.fc3 = torch.nn.Linear(256, target_size)
        assert input_size == 1
        assert target_size == 1

    def _forward(self, input):
        output = self.lstm(input)
        output = F.relu(
            self.fc1(output.contiguous().view(-1, self.output_size)))
        output = F.relu(self.fc2(output))
        output = self.fc3(output)
        return output

    def forward(self, input, true_target_size=1):
        outputs = None
        ite = range(true_target_size) if true_target_size == 1 else trange(true_target_size)
        for i in ite:
            output = self._forward(input)
            if outputs is None:
                outputs = output
            else:
                outputs = torch.cat((outputs, output), dim=1)
            input = input[:, 1:, :]
            input = torch.cat((input, output.unsqueeze(1)), dim=1)
        return outputs
