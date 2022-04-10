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
speed) big file (with two different `sha1sum.exe` files):
```
$ python bench.py
hehe is 1.000 GiB
All hashes match: c6f8a50e6a3bb11fcc5695039362da3621f7c41b *hehe
 2406.069 MiB/s - rampin -0q
  600.912 MiB/s - openssl sha1
  511.919 MiB/s - ./native.exe
  472.199 MiB/s - ./mysha1sum.exe
  434.433 MiB/s - C:/Program Files/Haskell Platform/8.6.5/msys/usr/bin/sha1sum.exe
  431.084 MiB/s - C:/Program Files/Git/usr/bin/sha1sum.exe
  363.862 MiB/s - C:/Program Files/Git/usr/bin/perl.exe C:/Program Files/Git/usr/bin/core_perl/shasum
  360.976 MiB/s - python sha1.py
  169.359 MiB/s - busybox sha1sum
```

```
$ python newbench.py
file: hehe, size: 1024.0 MiB, 7 repetitions, sorted by med
            cmd|          min|          max|          avg|          med
---------------|-------------|-------------|-------------|-------------
   openssl sha1|623.534 MiB/s|611.294 MiB/s|617.858 MiB/s|618.576 MiB/s
   ./native.exe|545.441 MiB/s|535.286 MiB/s|540.261 MiB/s|540.515 MiB/s
./mysha1sum.exe|498.638 MiB/s|489.805 MiB/s|494.812 MiB/s|495.960 MiB/s
        sha1sum|448.375 MiB/s|433.114 MiB/s|442.559 MiB/s|444.187 MiB/s
 python sha1.py|381.382 MiB/s|366.942 MiB/s|375.433 MiB/s|378.010 MiB/s
```

On a VitualBox Linux VM (Fedora 31) where `sha1sum` and Python's `hashlib.sha1`
both use OpenSSL's `libcrypto.so`:
```
[ff@localhost blasha1]$ python bench.py
hehe is 1.000 GiB
All hashes match: 65b448bb43478646c0072b62e420b2cc8b46e014 *hehe
  699.686 MiB/s - sha1sum
  649.414 MiB/s - openssl sha1
  544.726 MiB/s - python sha1.py
  453.495 MiB/s - ./native.exe
  429.585 MiB/s - ./mysha1sum.exe
  201.637 MiB/s - busybox sha1sum
```

NOTE: BusyBox has an option to optimize for space instead of speed and until recently
the option to enable performant [SHA1 was missing](https://bugs.busybox.net/show_bug.cgi?id=14391).
