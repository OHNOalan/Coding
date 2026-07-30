#!/bin/bash
g++-15 -I$(g++-15 -print-file-name=plugin)/include \
       -I$(brew --prefix)/include \
       -fPIC -shared \
       -fno-rtti \
       -undefined dynamic_lookup \
       -o my_plugin.so my_plugin.c

g++-15 -fplugin=./my_plugin.so test.c -o test_app 