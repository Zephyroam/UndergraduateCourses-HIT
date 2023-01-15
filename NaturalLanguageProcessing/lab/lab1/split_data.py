# -*- coding: utf-8 -*-
import random
import numpy as np

from utils import *

def split_data():
    random.seed(202111)
    with open(DATA1_SEG_POS, 'r') as f:
        lines_pos = np.array([l.strip() for l in f.readlines()])
    with open(DATA1_CONTENT, 'r') as f:
        lines_content = np.array([l.strip() for l in f.readlines()])
    
    assert len(lines_content) == len(lines_pos)
    new_index = list(range(len(lines_pos)))
    random.shuffle(new_index)
    lines_pos = lines_pos[new_index]
    lines_content = lines_content[new_index]
    trian_size = len(lines_content) // 10 * 9
    TRAIN_POS, TEST_POS = lines_pos[:trian_size], lines_pos[trian_size:]
    _, TEST_CONTENT = lines_content[:trian_size], lines_content[trian_size:]
    
    with open(DATA1_TRAIN_POS, 'w') as f:
        for l in TRAIN_POS:
            f.write(l + '\n')
    with open(DATA1_TEST_POS, 'w') as f:
        for l in TEST_POS:
            f.write(l + '\n')
    with open(DATA1_TEST_CONTENT, 'w') as f:
        for l in TEST_CONTENT:
            f.write(l + '\n')