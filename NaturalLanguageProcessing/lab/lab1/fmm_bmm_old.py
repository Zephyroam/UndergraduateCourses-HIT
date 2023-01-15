# -*- coding: utf-8 -*-

import time

from utils import *
from evaluation import *

class MM_OLD:
    def __init__(self, DICT_path, CONTENT_path):
        del_old_file(MM_FMM)
        del_old_file(MM_BMM)

        self.CONTENT_path = CONTENT_path
        self.DICT_path = DICT_path

    def search(self, sequence):
        for i in range(len(sequence))[::-1]:
            now_sequence = sequence[:i + 1]
            if now_sequence in self.dic:
                return now_sequence
        return None

    def MM(self, MM_type='F', timer=False):
        with open(self.DICT_path, 'r', encoding='utf8') as f:
            self.dic = [l.strip() for l in f.readlines()]
        self.max_len = max([len(w) for w in self.dic])
        if MM_type != 'F':
            self.dic = [w[::-1] for w in self.dic]
        f_mm = open(MM_FMM if MM_type == 'F' else MM_BMM, 'a', encoding='utf8')

        if timer:
            time_start = time.time()
        with open(self.CONTENT_path, 'r') as f:
            lines = [l.strip() for l in f.readlines()]
        for (line, index) in zip(lines, range(len(lines))):
            print('\r{}/{}'.format(index + 1, len(lines)))
            now_line, segs = line if MM_type == 'F' else line[::-1], []
            while len(now_line) > 0:
                result = self.search(now_line[:self.max_len])
                if result is not None:
                    segs.append(result)
                    now_line = now_line[len(result):]
                else:
                    segs.append(now_line[0])
                    now_line = now_line[1:]
            if MM_type != 'F':
                for i in range(len(segs)):
                    segs[i] = segs[i][::-1]
                segs = segs[::-1]
            segs = post_process_mm(segs)
            for seg in segs:
                f_mm.write(seg + '/ ')
            f_mm.write('\n')
        f_mm.close()
        if timer:
            time_end = time.time()
            return time_end - time_start

if __name__ == '__main__':
    mm = MM_OLD(MM_DICT, DATA1_TEST_CONTENT)
    
    with open(MM_TIMECOST, 'w', encoding='utf8') as f:
        f.write('Before Optimization:\n')
        time_FMM = mm.MM(MM_type='F', timer=True)
        f.write('\tFMM: time: {:.2f}, Acc: {}\n'.format(time_FMM, Evaluation(DATA1_TEST_POS, MM_FMM).get_accuracy()))
        time_BMM = mm.MM(MM_type='B', timer=True)
        f.write('\tBMM: time: {:.2f}, Acc: {}\n'.format(time_BMM, Evaluation(DATA1_TEST_POS, MM_BMM).get_accuracy()))
    