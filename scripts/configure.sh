#!/bin/sh
dir="$HOME/lpsmagic/build"
/scratchbox/login mkdir -p $dir
/scratchbox/login -d $dir cmake ..
