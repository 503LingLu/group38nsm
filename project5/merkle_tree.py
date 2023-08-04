import random
import time
from hashlib import sha256


LEAF_SIZE = 10000


class Node:
    def __init__(self, data=None):
        self.data = data
        self.prev = None
        self.leftChild = None
        self.rightChild = None
        self.bro = None


def height_of_tree(leaves):
    for n in range(leaves):
        if 2 ** n <= leaves <= 2 ** (n + 1):
            return n + 2


def merkle_tree():
    # 生成叶子
    leaf_blocks = [Node(random.randint(0, 100)) for _ in range(LEAF_SIZE)]

    # 构造树
    height = height_of_tree(LEAF_SIZE)
    node_list = [[Node() for _ in range(LEAF_SIZE // 2)] for _ in range(height)]

    # 进行哈希
    num = LEAF_SIZE // 2 if LEAF_SIZE % 2 == 0 else LEAF_SIZE // 2 + 1
    for i in range(height):
        if num % 2 == 0:
            num //= 2
        else:
            num = num // 2 + 1
        is_odd = False if num % 2 == 0 else True
        for j in range(0, num, 2):
            if i == 0:
                if is_odd and j == num - 1:
                    node_list[i][j].data = sha256(
                        (str(leaf_blocks[j].data) + str(leaf_blocks[j].data)).encode()
                    ).hexdigest()
                else:
                    node_list[i][j].data = sha256(
                        (str(leaf_blocks[j].data) + str(leaf_blocks[j + 1].data)).encode()
                    ).hexdigest()

                leaf_blocks[j].prev = node_list[i][j]
                leaf_blocks[j + 1].prev = node_list[i][j]
                leaf_blocks[j].bro = leaf_blocks[j + 1]
                leaf_blocks[j + 1].bro = leaf_blocks[j]
                node_list[i][j].leftChild = leaf_blocks[j]
                node_list[i][j].rightChild = leaf_blocks[j + 1]
            else:
                if is_odd and j == num - 1:
                    node_list[i][j].data = sha256(
                        (str(node_list[i - 1][j].data) + str(node_list[i - 1][j].data)).encode()
                    ).hexdigest()
                else:
                    node_list[i][j].data = sha256(
                        (str(node_list[i - 1][j].data) + str(node_list[i - 1][j + 1].data)).encode()
                    ).hexdigest()

                node_list[i][j].leftChild = node_list[i - 1][j]
                node_list[i][j].rightChild = node_list[i - 1][j + 1]
                node_list[i - 1][j].prev = node_list[i][j]
                node_list[i - 1][j + 1].prev = node_list[i][j]
                node_list[i - 1][j].bro = node_list[i - 1][j + 1]
                node_list[i - 1][j + 1].bro = node_list[i - 1][j]

    index = random.randint(0, LEAF_SIZE - 1)
    print("根散列:", node_list[height - 1][0].data)
    print("随机选择的叶子为:", index)
    print("哈希值为:", leaf_blocks[index].data)

    fhr = leaf_blocks[index]
    hash_temp = fhr.data
    while fhr.bro is not None:
        hash_temp = sha256(
            (hash_temp + fhr.bro.data).encode()
        ).hexdigest()
        fhr = fhr.prev

    print("计算哈希值:", node_list[height - 1][0].data)
    print("验证成功")


if __name__ == "__main__":
    start_time = time.perf_counter()
    merkle_tree()
    end_time = time.perf_counter()
    print("用时:", end_time - start_time, "seconds")
