#!/bin/sh

/scratchbox/login -d $HOME apt-get update
/scratchbox/login -d $HOME apt-get install cmake
/scratchbox/login -d $HOME apt-get -f install
/scratchbox/login -d $HOME apt-get install cmake
/scratchbox/login -d $HOME apt-get -f install

