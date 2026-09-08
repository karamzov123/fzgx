import os
import random

import avlz
import linecrypt


def test_linecrypt_roundtrip_all_lengths():
    rng = random.Random(1)
    for n in (0, 1, 3, 4, 5, 31, 32, 33, 100, 4097):
        data = bytes(rng.getrandbits(8) for _ in range(n))
        for region in ("INT", "JPN"):
            enc = linecrypt.crypt(data, region)
            assert len(enc) == n
            assert linecrypt.crypt(enc, region) == data
            if n >= 4:
                assert enc != data


def test_linecrypt_known_keystream_first_word():
    # s1 = key2*key1 + key0 (mod 2^32) for INT
    key0, key1, key2 = linecrypt.KEYS["INT"]
    s1 = (key2 * key1 + key0) & 0xFFFFFFFF
    enc = linecrypt.crypt(b"\0\0\0\0", "INT")
    assert int.from_bytes(enc, "big") == s1


def test_avlz_roundtrip_various():
    rng = random.Random(2)
    samples = [
        b"",
        b"a",
        b"abcabcabcabcabcabcabcabc" * 50,
        bytes(rng.getrandbits(8) for _ in range(5000)),
        (b"\0" * 6000) + b"end",
        bytes(range(256)) * 40,
    ]
    for data in samples:
        comp = avlz.compress(data)
        assert avlz.decompress(comp) == data
        # header fields
        assert int.from_bytes(comp[0:4], "little") == len(comp) - 8
        assert int.from_bytes(comp[4:8], "little") == len(data)


def test_avlz_rejects_truncated():
    comp = avlz.compress(b"hello hello hello hello")
    try:
        avlz.decompress(comp[:-3])
    except ValueError:
        return
    raise AssertionError("expected ValueError")
