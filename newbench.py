from subprocess import Popen, PIPE
from collections import defaultdict
import statistics
import time
import sys
import os

fname = "hehe" if len(sys.argv) == 1 else sys.argv[1]
fsize = os.path.getsize(fname)

repetitions = 7

cmds = [
    # ['rampin', '-0q'],
    ["openssl", "sha1"],
    # ['C:/Program Files/Git/usr/bin/sha1sum.exe'],
    ["sha1sum"],
    # ['C:/Program Files/Haskell Platform/8.6.5/msys/usr/bin/sha1sum.exe'],
    # ['C:/Program Files/Git/usr/bin/perl.exe', 'C:/Program Files/Git/usr/bin/core_perl/shasum'],
    ["busybox", "sha1sum"],
    ["./mysha1sum.exe"],
    ["./native.exe"],
    ["python", "sha1.py"],
]

cmds = cmds * repetitions
times = defaultdict(list)
for c in cmds:
    start = time.time()
    job = Popen(
        c + [fname],
        errors="replace",
        stdin=PIPE,
        stdout=PIPE,
        stderr=PIPE,
        universal_newlines=True,
    )
    jobout, joberr = job.communicate()
    elapsed = time.time() - start
    times[" ".join(c)].append(elapsed)

rows = []
for i, v in times.items():
    v = sorted(v)
    rows.append((i, v[-1], v[0], sum(v) / len(v), statistics.median(v)))

headers = ("cmd", "min", "max", "avg", "med")
widths = len(headers) * [0]
sorby = "med"
rows = sorted(rows, key=lambda x: x[headers.index(sorby)])
rows = [
    ((r[0],) + tuple(f"{fsize / (x * 1024 ** 2):.3f} MiB/s" for x in r[1:]))
    for r in rows
]
for r in [headers] + rows:
    for i, v in enumerate(r):
        widths[i] = max(widths[i], len(str(v)))

print(
    f"file: {fname}, size: {fsize / 1024 ** 2} MiB, {repetitions} repetitions, sorted by {sorby}"
)
print("|".join(str(v).rjust(w) for w, v in zip(widths, headers)))
print("|".join("-" * w for w in widths))
for r in rows:
    print("|".join(str(v).rjust(w) for w, v in zip(widths, r)))
