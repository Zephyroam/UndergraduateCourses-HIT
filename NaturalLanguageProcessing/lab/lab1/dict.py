# -*- coding: utf-8 -*-

from utils import *

class Dict:
    def __init__(self, SEG_POS_path, DICT_path):
        self.SEG_POS_path = SEG_POS_path
        self.DICT_path = DICT_path
        self.words = MySet()
        self.max_len = 0

    def get_dict(self):
        with open(self.SEG_POS_path, 'r') as f:
            lines = [l.strip().split() for l in f.readlines()]
        segs = [l[i] for l in lines for i in range(1, len(l))]
        for seg in segs:
            if ('/m' in word and word.isascii()) or len(word) == 0: continue
            word = seg[1 if seg[0] == '[' else 0 : seg.index('/')]
            self.words.add(word)
            self.max_len = max(len(word), self.max_len)
        self.words = sorted(self.words.toList())
        self._save_dict()
    
    def _save_dict(self):
        with open(self.DICT_path, 'w', encoding='utf8') as f:
            for word in self.words:
                f.write(word + '\n')


if __name__ == '__main__':
    dict = Dict(DATA1_TRAIN_POS, MM_DICT)
    dict.get_dict()