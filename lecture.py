def new_hash(key, size):
    ret_val = 0
    for c in key:
        ret_val += ord(c)
    return ret_val % size

class GenericObj:
    pass

