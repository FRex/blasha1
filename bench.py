from subprocess import Popen, PIPE
import time
import sys
import os
import re


fname = 'hehe' if len(sys.argv) == 1 else sys.argv[1]
fsize = os.path.getsize(fname)

cmds = [
    ['rampin', '-0q'],
    ['openssl', 'sha1'],
    ['C:/Program Files/Git/usr/bin/sha1sum.exe'],
    ['C:/Program Files/Haskell Platform/8.6.5/msys/usr/bin/sha1sum.exe'],
    ['busybox', 'sha1sum'],
    ['./mysha1sum.exe'],
    ['python', 'sha1.py'],
]

print(f"{fname} is {fsize / 1024 ** 3:.03f} GiB")

times = []
outs = []
for c in cmds:
    start = time.time()
    job = Popen(c + [fname], errors='replace', stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
    jobout, joberr = job.communicate()
    if joberr:
        print(joberr, file=sys.stderr)
    elapsed = time.time() - start
    outs.append(jobout)
    what = ' '.join(c)
    times.append((elapsed, what))

# this re.sub is doing what was in test.sh done by: sed 's:SHA1(\(.*\))= \(.*\):\2 *\1:;s:  : *:'
outs = [re.sub(r'SHA1\((.*)\)= (.*)', r'\2 *\1', o.strip().replace('  ', ' *')) for o in outs if o.strip()]
outs = list(set(outs))

if len(outs) == 1:
    print(f"All hashes match: {outs[0]}")
else:
    print("HASHES DON'T MATCH!")
    print("\n".join(outs))

times = sorted(times, key=lambda x: x[0])
times = [(f"{fsize / (elapsed * 1024.0 ** 2):9.03f} MiB/s", what) for elapsed, what in times]
for speed, what in times:
    print(f"{speed} - {what}")
