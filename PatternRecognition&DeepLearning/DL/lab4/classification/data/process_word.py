import jieba
import pandas as pd
import re
import numpy as np
import matplotlib.pyplot as plt

# remove punctuation
def remove_punctuation(line):
    line = str(line)
    if line.strip() == '':
        return ''
    rule = re.compile(u"[^a-zA-Z0-9\u4E00-\u9FA5]")
    line = rule.sub('', line)
    return line

# get the stopwords
def stopwordslist(filepath):
    stopwords = [
        line.strip()
        for line in open(filepath, 'r', encoding='utf-8').readlines()
    ]
    return stopwords

# formalize the data to the max length
def formalize_data(x, max_length):
    if len(x) == 0:
        x = np.zeros((max_length))
    elif len(x) < max_length:
        # print('before:', x.shape, x)
        x = np.pad(x, (0, max_length - len(x)), 'constant')
        # print('after:', x.shape)
    elif len(x) > max_length:
        x = x[:max_length]
    return x

# get the word2idx and word_num
def get_word2idx(data, attr_new):
    wordlist = []
    for x in data[attr_new]:
        wordlist.extend(x.split())
    wordlist = list(set(wordlist))
    # print('wordList:', wordlist)
    word2idx = {w: i for i, w in enumerate(wordlist)}
    word_num = len(word2idx)
    return word2idx, word_num

# process the word
def process_word(data: pd.DataFrame, attr: str, max_length: int):
    print('Processing data...')
    attr_new = attr + '_processed'

    print('Removing punctuation...')
    data[attr_new] = data[attr].apply(remove_punctuation)

    print('Cutting words and removing stopwords...')
    stopwords = stopwordslist("./data/stopwords.txt")
    data[attr_new] = data[attr_new].apply(lambda x: " ".join(
        [w for w in list(jieba.cut(x)) if w not in stopwords]))

    print('Turning words into ids...')
    word2idx, word_num = get_word2idx(data, attr_new)
    data[attr + '_id'] = data[attr_new].apply(
        lambda x: np.array([word2idx[w] for w in x.split()]))

    plot_length(data)

    print('Formalizing data...')
    data[attr + '_id'] = data[attr + '_id'].apply(formalize_data,
                                                  max_length=max_length)

    return data, word_num


# output the length of the dataset
def plot_length(data):
    data_len = [len(x) for x in data['review' + '_id']]
    plt.hist(data_len, range=(0, 60))
    plt.show()