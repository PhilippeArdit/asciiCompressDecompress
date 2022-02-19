#! /usr/bin/python
#
# python ./pythonVersion/asciiDecompress.py < ./testFiles/compressed.txt > ./testFiles/uncompressed.txt
# cat ./testFiles/compressed.txt | python ./pythonVersion/asciiDecompress.py > ./testFiles/uncompressed.txt
#
from asciiCompressDecompress import decompressTxt
import sys

if len(sys.argv) == 1:
    # echo "my string" | python asciiDecompress.py
    message = ''.join(sys.stdin.readlines())
else:
    # python asciiDecompress.py "my string"
    message = ' '.join(sys.argv[1:len(sys.argv)])

print(decompressTxt(message), end='')
