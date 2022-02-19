#! /usr/bin/python
#
# python ./pythonVersion/asciiCompress.py < ./testFiles/uncompressed.txt > testFiles/compressed.txt
# cat ./testFiles/uncompressed.txt | python ./pythonVersion/asciiCompress.py > testFiles/compressed.txt
#
from asciiCompressDecompress import compressTxt
import sys

if len(sys.argv) == 1:
    # echo "my string" | python asciiCompress.py
    message = ''.join(sys.stdin.readlines())
else:
    # python asciiCompress.py "my string"
    message = ' '.join(sys.argv[1:len(sys.argv)])

print(compressTxt(message), end='')
