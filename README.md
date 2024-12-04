# blasha1

A portable pure C implementation of SHA1 and (partially) the sha1sum program.

**Not verified to always be correct and thus not suitable for any serious use.**

Done for fun, to learn and to comapre to other implementations, look at how the
compiler optimizes it or not, try optimize some parts out, etc. and to *maybe*
later use it in my other
small utils like [FRex/pixelsum](https://github.com/FRex/pixelsum) and others.

In `test.sh` there is a bit of `bash` and `sed` code to run a few different
SHA1 implementations (`openssl sha1` from `OpenSSL 1.1.1d  10 Sep 2019`, GNU
coreutils `sha1sum`, `busybox sha1sum` 64-bit Windows build from
[frippery.org](https://frippery.org), `hashlib.sha1` in Python 3.7.3, and this
implementation compiled with `realgcc.exe (Rev1, Built by MSYS2 project) 7.2.0`
with `-O2`) on a given file, and make sure their output is same format and
collapsed into one line, and display how long they each took.

It also runs `rampin -q0` first to make sure the file is in the cache so first
command isn't disavantaged by having to load the file coldly, and to be upper
limit for how fast anything can work (`rampin -q0` touches all 4K pages of a
memory mapped file so any hash that touches all bytes is at least slower than
it, plus it has to do extra work of hashing itself).

`rampin` is my Windows specific CLI utility to ensure files get into and stay
in disk cache: [FRex/rampin](https://github.com/FRex/rampin).

In `bench.py` there is a Python script that does a test similar to one done by
`test.sh` but prints the sorted hashing speeds (file size divided by time
taken, so it favors big files where startup) too. It also runs `rampin` first.

In `newbench.py` there is another Python script that prints min, max, average
and median of the commands too.

File `tester.c` is a program that tests various edge cases and block sizes.

Example run of `bench.py` and `newbench.py` on an already cached (hence `rampin` running at RAM
speed) big file:
```
$ python3 bench.py
hehe is 1.000 GiB
All hashes match: 2a492f15396a6768bcbca016993f4b4c8b0b5307 *hehe
 2756.392 MiB/s - rampin -0q
 1506.026 MiB/s - python sha1.py
 1204.939 MiB/s - openssl sha1
  875.751 MiB/s - ./native.exe
  752.454 MiB/s - ./mysha1sum.exe
  729.311 MiB/s - C:/Program Files/Git/usr/bin/sha1sum.exe
  624.007 MiB/s - C:/Program Files/Git/usr/bin/perl.exe C:/Program Files/Git/usr/bin/core_perl/shasum
  275.630 MiB/s - busybox sha1sum
```

```
$ python3 newbench.py
file: hehe, size: 1024.0 MiB, 7 repetitions, sorted by med
            cmd|           min|           max|           avg|           med
---------------|--------------|--------------|--------------|--------------
 python sha1.py|1533.972 MiB/s|1623.957 MiB/s|1596.982 MiB/s|1602.804 MiB/s
   openssl sha1|1244.188 MiB/s|1286.648 MiB/s|1271.724 MiB/s|1276.805 MiB/s
   ./native.exe| 886.479 MiB/s| 920.421 MiB/s| 906.993 MiB/s| 915.417 MiB/s
./mysha1sum.exe| 756.169 MiB/s| 789.744 MiB/s| 780.363 MiB/s| 785.974 MiB/s
        sha1sum| 742.695 MiB/s| 776.583 MiB/s| 763.897 MiB/s| 771.197 MiB/s
busybox sha1sum| 278.332 MiB/s| 287.190 MiB/s| 283.653 MiB/s| 284.513 MiB/s
```

NOTE: BusyBox has an option to optimize for space instead of speed and until recently
the option to enable performant [SHA1 was missing](https://bugs.busybox.net/show_bug.cgi?id=14391).
