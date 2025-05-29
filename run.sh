#!/bin/bash

export LD_LIBRARY_PATH=$PWD/../target-root/usr/lib:$PWD/../target-root/usr/lib64

./bin/sls -c kl.conf
