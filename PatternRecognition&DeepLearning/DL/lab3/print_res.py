from glob import glob
import re

path = './checkpoint/*'


def main():
    logs = []
    path_logs = glob(path)
    for path_log in path_logs:
        print(path_log)
        try:
            path_log = path_log + '/acc.txt'
            name = re.split('/|\\\\', path_log)[-2]
            with open(path_log, 'r') as f:
                lines = f.readlines()
                acc = [float(line.split(' ')[-1]) for line in lines]
            logs.append((name, acc))
        except:
            pass

    for log in logs:
        print(log[0])
        print('\t train: {}'.format(log[1][0]))
        print('\t val: {}'.format(log[1][1]))
        print()


if __name__ == '__main__':
    main()