#!/usr/bin/env python

import math

def main():
    delta = 360.0 / 256.0

    for i in range(256):                                                                                                       
        angle = i * delta
        rad = math.radians(angle)
        s = math.sin(rad)
        o = round((s * 127) + 128)
        print("  {}, // {}".format(o, i))


if __name__ == '__main__':
    main()
