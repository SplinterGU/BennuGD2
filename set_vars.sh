#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
LD_LIBRARY_PATH=$DIR/build/linux-gnu/bin
export LD_LIBRARY_PATH
PATH=$DIR/build/linux-gnu/bin:$PATH
