#!/bin/bash

if ![ ! -f hw.zip ]; then
  unzip hw.zip
fi
chmod u+x unpack.sh
make unpack


