from sys import argv, stderr
from hashlib import sha1

try:
    f = open(argv[1], 'rb')
    c = sha1()
    while True:
        data = f.read(64 * 1024 * 1024)
        if len(data) == 0:
            break
        c.update(data)
    print(f"{c.hexdigest()} *{argv[1]}")
except FileNotFoundError as e:
    print(e, file=stderr)
