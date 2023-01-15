class AVLNode:
    def __init__(self, data=None, extra_data=None):
        self.data = data
        self.extra_data = extra_data
        self.left = None
        self.right = None
        self.height = 0

class AVLTree:
    def __init__(self):
        self.root = None

    def _LL(self, node):
        temp = node.left
        node.left = temp.right
        temp.right = node
        node.height = max(self._getHeight(node.right), self._getHeight(node.left)) + 1
        temp.height = max(self._getHeight(temp.right), self._getHeight(temp.left)) + 1
        return temp

    def _RR(self, node):
        temp = node.right
        node.right = temp.left
        temp.left = node
        node.height = max(self._getHeight(node.right), self._getHeight(node.left)) + 1
        temp.height = max(self._getHeight(temp.right), self._getHeight(temp.left)) + 1
        return temp

    def _LR(self, node):
        node.left = self._RR(node.left)
        return self._LL(node)

    def _RL(self, node):
        node.right = self._LL(node.right)
        return self._RR(node)

    def _getHeight(self, node):
        if node == None:
            return -1
        return node.height

    def _insert(self, data, extra_data, node):
        if node == None:
            temp = AVLNode(data=data, extra_data=extra_data)
            return temp
        if data == node.data:
            return node
        if data < node.data:
            node.left = self._insert(data, extra_data, node.left)
            if self._getHeight(node.left) - self._getHeight(node.right) >= 2:
                if data < node.left.data: node = self._LL(node)
                else: node = self._LR(node)
        else:
            node.right = self._insert(data, extra_data, node.right)
            if self._getHeight(node.right) - self._getHeight(node.left) >= 2:
                if data > node.right.data: node = self._RR(node)
                else: node = self._RL(node)

        node.height = max(self._getHeight(node.left), self._getHeight(node.right)) + 1
        return node

    def insert(self, data, extra_data):
        self.root = self._insert(data, extra_data, self.root)
        return self.root

    def _get_extra_data(self, data, node):
        if node == None:
            return None
        if data == node.data:
            return node.extra_data
        if data < node.data:
            return self._get_extra_data(data, node.left)
        else:
            return self._get_extra_data(data, node.right)

    def get_extra_data(self, data):
        return self._get_extra_data(data, self.root)
    
    def _toList(self, node):
        result = []
        if node != None:
            result.append(node.extra_data)
            result += self._toList(node.left)
            result += self._toList(node.right)
        return result
    
    def toList(self):
        return self._toList(self.root)

class MyAVL:
    def __init__(self):
        self.tree = AVLTree()
    
    def __setitem__(self, key, value):
        self.tree.insert(key, value)
    
    def __getitem__(self, key):
        return self.tree.get_extra_data(key)
    