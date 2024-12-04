from sys import argv, stderr
from hashlib import sha1, file_digest

try:
    f = open(argv[1], "rb")
    c = file_digest(f, sha1)
    print(f"{c.hexdigest()} *{argv[1]}")
except FileNotFoundError as e:
    print(e, file=stderr)
