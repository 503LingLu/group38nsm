class MPT:
    def __init__(self):
        self.prefix = 0
        self.shared_nibbles = ""
        self.next_node = [None]*16
        self.value = 0
        self.leaf = False

easy_mpt = MPT()

def setup():
    global easy_mpt
    easy_mpt.prefix = 0
    easy_mpt.shared_nibbles = "a7"
    easy_mpt.next_node[1] = MPT()
    easy_mpt.next_node[1].prefix = 2
    easy_mpt.next_node[1].leaf = True
    easy_mpt.next_node[1].shared_nibbles = "1355"
    easy_mpt.next_node[1].value = 45
    easy_mpt.next_node[15] = MPT()
    easy_mpt.next_node[15].prefix = 2
    easy_mpt.next_node[15].leaf = True
    easy_mpt.next_node[15].shared_nibbles = "9365"
    easy_mpt.next_node[15].value = 2
    easy_mpt.next_node[7] = MPT()
    temp = easy_mpt.next_node[7]
    temp.prefix = 0
    temp.shared_nibbles = "d3"
    temp.next_node[3] = MPT()
    temp.next_node[3].prefix = 3
    temp.next_node[3].leaf = True
    temp.next_node[3].shared_nibbles = "7"
    temp.next_node[3].value = 1
    temp.next_node[9] = MPT()
    temp.next_node[9].prefix = 3
    temp.next_node[9].leaf = True
    temp.next_node[9].shared_nibbles = "7"
    temp.next_node[9].value = 5

def to_int(c):
    if ('A' <= c <= 'Z'):
        return ord(c) - ord('A') + 10
    elif ('a' <= c <= 'z'):
        return ord(c) - ord('a') + 10
    elif ('0' <= c <= '9'):
        return ord(c) - ord('0')

def node_verify(m):
    global easy_mpt
    mlength = len(m)
    temp = easy_mpt
    str_ = ""
    while not temp.leaf and mlength > 0:
        str_ = str_ + temp.shared_nibbles
        if m[0:len(str_)] == str_:
            p = m[len(str_):len(str_)+1]
            str_ += p
            temp = temp.next_node[to_int(p)]
        else:
            return False
    mlength -= len(str_)
    if mlength != len(temp.shared_nibbles):
        return False
    str_ = str_ + temp.shared_nibbles
    if m[0:len(str_)] == str_:
        return True
    else:
        return False

def main():
    setup()
    print("a7f9365")
    if node_verify("a7f9365"):
        print("success")
    else:
        print("")

if __name__ == "__main__":
    main()
