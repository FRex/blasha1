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

In `testrun.txt` there's output of running `test.sh`, with interesting results:
`openssl sha1` is the fastest, `mysha1sum.exe` and `python sha1.py` are on par,
both taking 50% more time than `openssl sha1`, `sha1sum` is a bit faster than
them, but takes 25% more time than `openssl sha1`, and `busybox sha1sum` is the
slowest, and *very* slow, taking 2.25x the time my implementation and Python did.

These results will vary by OS, system, implementation, compilers used, etc. but
they are still curious, e.g. the fact that a simple C implementation in portable
pure C with `gcc -O2` can compete with GNU coreutils and OpenSSL binaries and
not lose by much.
