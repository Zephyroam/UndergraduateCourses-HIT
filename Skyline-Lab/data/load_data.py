def load_data(path):
    with open(path, 'r') as f:
        data = [x.strip() for x in f.readlines()]
    if ',' in data[0]:
        data = [tuple([float(item) for item in x.split(',')]) for x in data]
    else:
        data = [tuple([float(item) for item in x.split()]) for x in data]
    return data


if __name__ == '__main__':
    load_data('data/qws/qws_normal.txt')