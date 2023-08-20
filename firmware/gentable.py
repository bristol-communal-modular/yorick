#!/usr/bin/env python

import sys
import math
import random


def sine():
    delta = 360.0 / 256.0

    for i in range(256):                                                                                                       
        angle = i * delta
        rad = math.radians(angle)
        s = math.sin(rad)
        o = round((s * 127))
        print("  {}, // {}".format(o, i))

def saw():
    for i in range(256):                                                                                                       
        o = i - 128
        print("  {}, // {}".format(o, i))

def square():
    delta = 360.0 / 256.0

    for i in range(256):                                                                                                       
        if i < 128:
            o = -128
        else:
            o = 127
        print("  {}, // {}".format(o, i))

def rand():
    for i in range(256):                                                                                                       
        o = round((random.random() * 256) - 128)
        print("  {}, // {}".format(o, i))


if __name__ == '__main__':
    if len(sys.argv) > 1:
        wavetype = sys.argv[1]
    else:
        wavetype = "sine"
    if wavetype == "sine":
        sine()
    elif wavetype == "saw":
        saw()
    elif wavetype == "square":
        square()
    elif wavetype == "rand":
        rand()
