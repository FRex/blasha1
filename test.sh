#!/bin/bash

fname='hehe'
if [ $# -eq 1 ] ; then
    fname="$1"
fi

function echoer() { echo >/dev/stderr "$@"; }

(
    rampin -0q "$fname" || echoer

    echoer -n "openssl sha1 $fname: "
    time openssl sha1 "$fname" | sed 's:SHA1(\(.*\))= \(.*\):\2 *\1:'
    echoer

    echoer -n "sha1sum $fname: "
    time sha1sum "$fname"
    echoer

    echoer -n "busybox sha1sum $fname: "
    time busybox sha1sum "$fname" | sed 's:  : *:'
    echoer

    echoer -n "./b.exe $fname: "
    time ./b.exe "$fname"
    echoer

    echoer -n "python sha1.py $fname: "
    time python sha1.py "$fname"
    echoer
) | dos2unix | uniq -c
