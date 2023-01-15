import os


class AverageMeter(object):
    """Computes and stores the average and current value"""

    def __init__(self):
        self.reset()

    def reset(self):
        self.val = 0
        self.avg = 0
        self.sum = 0
        self.count = 0

    def update(self, val, n=1):
        self.val = val
        self.sum += val * n
        self.count += n
        self.avg = self.sum / self.count


def ini_path(path):
    if not os.path.exists(path):
        os.makedirs(path)
    else:
        for root, dirs, files in os.walk(path):
            for file in files:
                os.remove(os.path.join(root, file))