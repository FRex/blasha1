#!/bin/bash

fname='hehe'
if [ $# -eq 1 ] ; then
    fname="$1"
fi

function echoer() { echo >/dev/stderr "$@"; }

cmds=(
    'rampin -0q'
    'openssl sha1'
    'sha1sum'
    'busybox sha1sum'
    './mysha1sum.exe'
    'python sha1.py'
)

du -h "$fname"
echo

for cmd in "${cmds[@]}"; do
    echoer -n "$cmd $fname: "
    time $cmd "$fname" #intentionally no "" around $cmd for cmds with params!
    echoer
done | sed 's:SHA1(\(.*\))= \(.*\):\2 *\1:;s:  : *:' | dos2unix | uniq -c
