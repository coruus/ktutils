#!/usr/bin/env python
"""Generate a Clef constant with some rigidity."""
from __future__ import division, print_function

from binascii import hexlify

import sys
import struct

from rijndael import Rijndael

QQW = struct.Struct('QQQQ').unpack

def mbrpad(s):
    """Apply multi-byte-rate padding to input."""
    length = len(s)
    if length == 32:
        return s
    elif length > 31:
        raise Exception("Constant too long")
    s = bytearray(s + '\x00' * (32 - len(s)))
    s[length] ^= 0x80
    s[-1] ^= 0x01
    return bytes(s)

def mkk(s, key):
    cipher = Rijndael(mbrpad(key), block_size=32)
    block = cipher.encrypt(mbrpad(s))
    print(hexlify(block))
    print(','.join('0x{:02x}'.format(b) for b in bytearray(block)))
    print('0x{:016x}, 0x{:016x}, 0x{:016x}, 0x{:016x}'.format(*QQW(block)))
    return block

if __name__ == '__main__':
    mkk(sys.argv[2], sys.argv[1])
