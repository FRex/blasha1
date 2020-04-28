# blasha1

A portable pure C implementation of SHA1 and (partially) the sha1sum program.

**Not verified to always be correct and thus not suitable for any serious use.**

Done for fun, to learn and to comapre to other implementations, look at how the
compiler optimizes it or not, try optimize some parts out, etc. and to *maybe*
later turn it into a single-header-file style library and use it in my other
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

Example run of `bench.py` on an already cached (hence `rampin` running at RAM
speed) big file (with two different `sha1sum.exe` files):
```
$ python bench.py
hehe is 1.000 GiB
All hashes match: c6f8a50e6a3bb11fcc5695039362da3621f7c41b *hehe
 2377.326 MiB/s - rampin -0q
  609.069 MiB/s - openssl sha1
  484.734 MiB/s - ./mysha1sum.exe
  454.678 MiB/s - C:/Program Files/Haskell Platform/8.6.5/msys/usr/bin/sha1sum.exe
  426.518 MiB/s - C:/Program Files/Git/usr/bin/sha1sum.exe
  381.756 MiB/s - C:/Program Files/Git/usr/bin/perl.exe C:/Program Files/Git/usr/bin/core_perl/shasum
  374.671 MiB/s - python sha1.py
  175.153 MiB/s - busybox sha1sum
```
