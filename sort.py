"""Same procedures and counters as the C implementation.

A move stores an Item, including a temporary local copy; swap = 3 moves.
"""
from dataclasses import dataclass

@dataclass
class Stats:
    comparisons: int = 0
    moves: int = 0

def less(a, b, stats):
    stats.comparisons += 1
    return a[0] < b[0]

def insertion_sort(a, stats):
    for i in range(1, len(a)):
        value = a[i]
        stats.moves += 1
        j = i
        while j and less(value, a[j-1], stats):
            a[j] = a[j-1]
            stats.moves += 1
            j -= 1
        a[j] = value
        stats.moves += 1

def merge_sort(a, stats):
    buffer = [None] * len(a)
    def recurse(lo, hi):
        if hi-lo < 2:
            return
        mid = lo+(hi-lo)//2
        recurse(lo, mid)
        recurse(mid, hi)
        i, j, k = lo, mid, lo
        while i < mid and j < hi:
            if less(a[j], a[i], stats):
                buffer[k] = a[j]
                j += 1
            else:
                buffer[k] = a[i]
                i += 1
            stats.moves += 1
            k += 1
        while i < mid:
            buffer[k] = a[i]
            stats.moves += 1
            i += 1
            k += 1
        while j < hi:
            buffer[k] = a[j]
            stats.moves += 1
            j += 1
            k += 1
        for k in range(lo, hi):
            a[k] = buffer[k]
            stats.moves += 1
    recurse(0, len(a))

def heap_sort(a, stats):
    def swap(i, j):
        a[i], a[j] = a[j], a[i]
        stats.moves += 3
    def sink(root, n):
        while root < n//2:
            child = 2*root+1
            if child+1 < n and less(a[child], a[child+1], stats):
                child += 1
            if not less(a[root], a[child], stats):
                break
            swap(root, child)
            root = child
    for i in range(len(a)//2-1, -1, -1):
        sink(i, len(a))
    for end in range(len(a), 1, -1):
        swap(0, end-1)
        sink(0, end-1)

ALGORITHMS = [("insertion", insertion_sort), ("merge", merge_sort), ("heap", heap_sort)]

def make_input(n, shape):
    state = 20260930+n
    result = []
    for i in range(n):
        if shape in ("random", "duplicates"):
            state ^= (state << 13) & 0xffffffff
            state ^= state >> 17
            state ^= (state << 5) & 0xffffffff
            key = state % (100000 if shape == "random" else 10)
        elif shape == "sorted":
            key = i
        elif shape == "reverse":
            key = n-i
        else:
            key = 7
        result.append((key, i))
    return result

def is_stable(a):
    return all(a[i-1][0] != a[i][0] or a[i-1][1] < a[i][1] for i in range(1, len(a)))
