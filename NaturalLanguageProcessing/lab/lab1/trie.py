# -*- coding: utf-8 -*-
from utils import *

class Node:
    def __init__(self, type='AVL'):
        self.value = False
        self.children = MyDict(type=type)
        

class Trie:
    def __init__(self, DICT_path, type='AVL'):
        self.DICT_path = DICT_path
        self.type = type
        self.root = Node(type=self.type)
        self.max_len = 0

    def insert(self, sequence):
        self.max_len = max(len(sequence), self.max_len)
        node_now = self.root
        for item in sequence:
            node_next = node_now.children[item]
            if node_next == None:
                node_now.children[item] = Node(type=self.type)
            node_now = node_now.children[item]
        node_now.value = True

    def search(self, sequence):
        node_now, result = self.root, None
        for i in range(len(sequence)):
            node_next = node_now.children[sequence[i]]
            if node_next == None:
                return result
            else:
                node_now = node_next
            if node_now.value:
                result = sequence[:i + 1]
        return result

    def build(self, reverse=False):
        with open(self.DICT_path, 'r', encoding='utf8') as f:
            for token in f.readlines():
                if reverse: token = token.strip()[::-1]
                else: token = token.strip()
                self.insert(token)
