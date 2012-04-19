#!/bin/sh

/scratchbox/login -d $HOME/lpsmagic dpkg-buildpackage

deb=`/scratchbox/login -d $HOME find -maxdepth 1 -name lpsmagic*.deb`
echo copying $deb from sbox $HOME to $HOME/lpsmagic
/scratchbox/login -d $HOME cp $deb $HOME/lpsmagic

