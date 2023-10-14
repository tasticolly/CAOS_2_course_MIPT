#!/usr/bin/env bash

aarch64-linux-gnu-gcc main.c 3.S
qemu-aarch64 -L /usr/aarch64-linux-gnu/ ./a.out
