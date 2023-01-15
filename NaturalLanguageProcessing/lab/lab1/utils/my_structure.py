from utils.self_balanced_tree import *
from utils.hash_map import *

class MyDict:
    def __init__(self, type='AVL'):
        '''
        type: 'AVL', 'HASH' or 'HASH1'
        '''
        if type == 'AVL':
            self.dic = MyAVL()
        elif type == 'HASH':
            self.dic = MyHash()
        elif type == 'HASH1':
            self.dic = MyHash1()
        else:
            raise Exception('type error')
    
    def __setitem__(self, key, value):
        self.dic[key] = value
    
    def __getitem__(self, key):
        return self.dic[key]
    
class MySet:
    def __init__(self):
        self.tree = AVLTree()
    
    def add(self, key):
        self.tree.insert(key, key)
    
    def toList(self):
        return self.tree.toList()