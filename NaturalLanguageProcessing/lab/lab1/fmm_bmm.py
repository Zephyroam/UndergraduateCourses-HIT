# -*- coding: utf-8 -*-

import time

from utils import *
from trie import *
from evaluation import *

class MM:
    def __init__(self, DICT_path, CONTENT_path):
        del_old_file(MM_FMM)
        del_old_file(MM_BMM)

        self.CONTENT_path = CONTENT_path
        self.DICT_path = DICT_path
        
    def MM(self, MM_type='F', dict_type='AVL', timer=False):
        self.trie = Trie(self.DICT_path, type=dict_type)
        self.trie.build(reverse=True if MM_type != 'F' else False)
        f_mm = open(MM_FMM if MM_type == 'F' else MM_BMM, 'a', encoding='utf8')

        if timer:
            time_start = time.time()
        with open(self.CONTENT_path, 'r') as f:
            lines = [l.strip() for l in f.readlines()]
        for line in lines:
            now_line, segs = line if MM_type == 'F' else line[::-1], []
            while len(now_line) > 0:
                result = self.trie.search(now_line[:self.trie.max_len])
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
    '''
    type: 'AVL', 'HASH' or 'HASH1'
    '''
    f_timecost = open(MM_TIMECOST, 'a', encoding='utf8')
    f_timecost.write('After Optimization:\n')
    
    mm = MM(MM_DICT, DATA1_TEST_CONTENT)
    time_FMM = mm.MM(MM_type='F', dict_type='AVL', timer=True)
    time_BMM = mm.MM(MM_type='B', dict_type='AVL', timer=True)
    f_timecost.write('using AVL:\n')
    f_timecost.write('\tFMM: time: {:.2f}'.format(time_FMM))
    f_timecost.write('\tBMM: time: {:.2f}'.format(time_BMM))
    
    mm = MM(MM_DICT, DATA1_TEST_CONTENT)
    time_FMM = mm.MM(MM_type='F', dict_type='HASH', timer=True)
    time_BMM = mm.MM(MM_type='B', dict_type='HASH', timer=True)
    f_timecost.write('using HASH:\n')
    f_timecost.write('\tFMM: time: {:.2f}'.format(time_FMM))
    f_timecost.write('\tBMM: time: {:.2f}'.format(time_BMM))

    mm = MM(MM_DICT, DATA1_TEST_CONTENT)
    time_FMM = mm.MM(MM_type='F', dict_type='HASH1', timer=True)
    time_BMM = mm.MM(MM_type='B', dict_type='HASH1', timer=True)
    f_timecost.write('using HASH1:\n')
    f_timecost.write('\tFMM: time: {:.2f}'.format(time_FMM))
    f_timecost.write('\tBMM: time: {:.2f}'.format(time_BMM))

    