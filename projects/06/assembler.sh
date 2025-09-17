#!/bin/bash
# This program exists solely to pipe data into the LOLCODE program as I wasn't
# able to find a built in way to do so, though I code be wrong

# Require a file to be supplied
if [ -z "$1" ]; then
  echo "Usage: ./assembler.sh filename.asm"
  exit 1
fi

echo "$1" | lci assembler.lol