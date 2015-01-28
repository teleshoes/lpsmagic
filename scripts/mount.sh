#!/bin/sh
dir=$HOME/Code/lpsmagic
mnt=/scratchbox/users/$USER/home/$USER/lpsmagic

sudo /scratchbox/sbin/sbox_ctl start
sudo /scratchbox/sbin/sbox_sync

mkdir -p $mnt
sudo mount -o bind $dir $mnt
