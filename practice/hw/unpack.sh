#!/bin/bash

if ![ ! -f hw.zip ]; then
  unzip hw.zip
fi
chmod u+z unpack.sh
make unpack


