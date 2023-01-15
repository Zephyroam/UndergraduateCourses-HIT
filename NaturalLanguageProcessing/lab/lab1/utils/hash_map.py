from .utils import *

class MyHash:
    # refer to https://planetmath.org/goodhashtableprimes
    def __init__(self, length=1543):
        self.length = length
        self.items = [[]] * self.length
        self.hash = get_hash

    def _insert(self, key, value):
        index = self.hash(key) % self.length
        if self.items[index]:
            for item in self.items[index]:
                if key == item[0]:
                    return
        self.items[index].append((key, value))

    def _get(self, key):
        index = self.hash(key) % self.length
        if self.items[index]:
            for item in self.items[index]:
                if key == item[0]:
                    return item[1]
        return None

    def __setitem__(self, key, value):
        return self._insert(key, value)

    def __getitem__(self, key):  
        return self._get(key)



class MyHash1:
    def __init__(self, size=8):
        self._size = size
        self._used_slots = 0
        self._keys = [None] * self._size
        self._values = [None] * self._size
        self.hash = get_hash
        self._max_threshold = 0.70

    def _calc_new_pos(self, current_position):
        # refer to http://hg.python.org/cpython/file/52f68c95e025/Objects/dictobject.c#l69
        return ((5 * current_position) + 1) % self._size
        
    def _set_item(self, position, key, value):
        existing_key = self._keys[position]
        
        if existing_key is None or existing_key == key:
            self._keys[position] = key
            self._values[position] = value
            self._used_slots += 1
        else:
            new_position = self._calc_new_pos(position)
            self._set_item(new_position, key, value)
        
    def _resize(self):
        temp_keys = self._keys
        temp_values = self._values
        self._size = self._size * 4
        self._keys = [None] * self._size
        self._values = [None] * self._size
        self._used_slots = 0
        
        for (key, value) in zip(temp_keys, temp_values):
            if key is not None:
                hashvalue = self.hash(key)
                position = self._calc_pos(hashvalue)
                self._set_item(position, key, value)
        
    def _calc_pos(self, hashvalue):
        return hashvalue % self._size
        
    def __setitem__(self, key, value):
        if (self._used_slots / self._size) >= self._max_threshold:
            self._resize()
            
        position = self._calc_pos(self.hash(key))
        self._set_item(position, key, value)
        
    def _get_pos(self, position, key):
        tmp_key = self._keys[position]

        if tmp_key == None:
            return None
        if tmp_key != key:
            return self._get_pos(self._calc_new_pos(position), key) 
        else:
            return position
        
    def __getitem__(self, key):
        position = self._calc_pos(self.hash(key))
        position = self._get_pos(position, key)

        if position is None:
            return None
        return self._values[position]