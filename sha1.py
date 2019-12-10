from sys import argv, stderr
from hashlib import sha1

try:
    data = open(argv[1], 'rb').read()
    print(f"{sha1(data).hexdigest()} *{argv[1]}")
except FileNotFoundError as e:
    print(e, file=stderr)
