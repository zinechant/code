#!/bin/python3

import math
import os
import random
import re
import sys

#
# Complete the 'closestStraightCity' function below.
#
# The function is expected to return a STRING_ARRAY.
# The function accepts following parameters:
#  1. STRING_ARRAY c
#  2. INTEGER_ARRAY x
#  3. INTEGER_ARRAY y
#  4. STRING_ARRAY q
#

MAX_DIST = 2000000000
import bisect
def closest(arr, val):
    ans = (MAX_DIST + 10, "NONE")
    x = bisect.bisect_right(arr, (val, ""))
    if x > 0:
        ans = (arr[x][0] - arr[x-1][0], arr[x-1][1])
    if x < len(arr) - 1:
        alt = (arr[x+1][0] - arr[x][0], arr[x+1][1])
        if alt < ans:
            ans = alt
    return ans

def closestStraightCity(c, x, y, q):
    xs = {}
    ys = {}
    cities = {}
    for i in range(len(c)):
        cities[c[i]] = (x[i], y[i])
        if x[i] not in xs:
            xs[x[i]] = []
        if y[i] not in ys:
            ys[y[i]] = []
            
        xs[x[i]].append((y[i], c[i]))
        ys[y[i]].append((x[i], c[i]))
    
    for x in xs:
        xs[x] = sorted(xs[x])
    for y in ys:
        ys[y] = sorted(ys[y])

    ans = []
    for city in q:
        if city not in cities:
            ans.append("NONE")
            continue
        x, y = cities[city]
        res = closest(xs[x], y)
        alt = closest(ys[y], x)
        if alt < res:
            res = alt
        ans.append(res[1])
    return ans

if __name__ == '__main__':
    c = ["fastcity", "bigbanana", "xyz"]
    x = [23, 23, 23]
    y = [1, 10, 20]
    q = ["fastcity", "bigbanana", "xyz"]
    print(closestStraightCity(c, x, y, q))
    # fptr = open(os.environ['OUTPUT_PATH'], 'w')

    # c_count = int(input().strip())

    # c = []

    # for _ in range(c_count):
    #     c_item = input()
    #     c.append(c_item)

    # x_count = int(input().strip())

    # x = []

    # for _ in range(x_count):
    #     x_item = int(input().strip())
    #     x.append(x_item)

    # y_count = int(input().strip())

    # y = []

    # for _ in range(y_count):
    #     y_item = int(input().strip())
    #     y.append(y_item)

    # q_count = int(input().strip())

    # q = []

    # for _ in range(q_count):
    #     q_item = input()
    #     q.append(q_item)

    # result = closestStraightCity(c, x, y, q)

    # fptr.write('\n'.join(result))
    # fptr.write('\n')

    # fptr.close()
