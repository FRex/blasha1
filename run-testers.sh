#!/bin/bash

sha1sum hehe *.* smallfiles/* | ./tester.exe - | tail -n 1
sha1sum hehe *.* smallfiles/* | ./tester-native.exe - | tail -n 1
