# -*- coding: utf-8 -*-
from utils import *

class Evaluation:
    def __init__(self, DATA_path, PRED_path):
        self.DATA = []
        with open(DATA_path, 'r') as f:
            lines = [l.strip() for l in f.readlines()]
        for l in lines:
            segs, now_segs = l.split(), []
            for seg in segs:
                if len(seg) == 0: continue
                word = seg[1 if seg[0] == '[' else 0 : seg.index('/')]
                now_segs.append(word)
            self.DATA.append(now_segs)
        
        # with open('./data/1998_std.txt', 'w', encoding='utf8') as f:
        #     for l in self.DATA:
        #         for x in l:
        #             f.write(x + '/ ')
        #         f.write('\n')
        
        self.PRED = []
        with open(PRED_path, 'r', encoding='utf8') as f:
            lines = [l.strip() for l in f.readlines()]
        for l in lines:
            segs, now_segs = l.split(), []
            for seg in segs:
                if len(seg) == 0: continue
                word = seg[0 : seg.index('/')]
                now_segs.append(word)
            self.PRED.append(now_segs)
    
    def to_region(self, segs):
        region, start = [], 0
        for seg in segs:
            end = start + len(seg)
            region.append((start, end))
            start = end
        return region

    def get_cap(self, region1, region2):
        cap = []
        for r1 in region1:
            for r2 in region2:
                if r1 == r2:
                    cap.append(r1)
                    break
        return cap

    def get_accuracy(self):
        assert len(self.DATA) == len(self.PRED)
        A_size, B_size, A_cap_B_size = 0, 0, 0
        for i in range(len(self.DATA)):
            if len(self.DATA[i]) == 0: continue
            A, B = self.to_region(self.DATA[i]), self.to_region(self.PRED[i])
            A_size += len(A)
            B_size += len(B)
            A_cap_B_size += len(self.get_cap(A, B))
        precision, recall = A_cap_B_size / B_size * 100, A_cap_B_size / A_size * 100
        return precision, recall, 2 * precision * recall / (precision + recall)
    
    def __str__(self):
        precision, recall, f = self.get_accuracy()
        return 'precision: %.2f%%, recall: %.2f%%, F: %.2f' % (precision, recall, f)
        
        

if __name__ == '__main__':
    print(str(Evaluation(DATA_TEST_POS, BI_SEG)))
    # with open(MM_SCORE, 'w', encoding='utf8') as f:
    #     f.write('FMM: ' + str(Evaluation(DATA1_SEG_POS, MM_FMM)) + '\n')
    #     f.write('BMM: ' + str(Evaluation(DATA1_SEG_POS, MM_BMM)) + '\n')
