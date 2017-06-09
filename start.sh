#!/bin/bash
# file: web-cli

./web-cli                           \
          --input-file sites.txt    \
          --verbose                 \
          -l 1                      \
          -s /home/favorart/data    \
          -t 2                      \
          --no-parent               \
          -r
