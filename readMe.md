
# ASCII Fold - Plain and simple text compression

## Inspired from [CodeLobe.com](http://codelobe.com/)/[tools](http://codelobe.com/tools)/[ascii-compress](http://codelobe.com/tools/ascii-compress)

Initially wrote in JavaScript by the first author (sysop@codelobe.com) to compress lengthy JSON data & WebGL shader, I converted this code in Python an C languages.

Even if it only works on text compatible with 7 bit ASCII, it is a small & light-weight code. The output is plain ASCII text, and can be stored anywhere or transfered thru any network, as well as BLE serial protocol.

## Use case

My final objective was to compensate for the slowness of data transfers via BLE (2kb/s maximum): an embedded system stores data on microSD card in ASCII/CSV format; when it is idle (while battery is charging), it compresses these files which will be transferred via BLE in the end; the recipient decompresses and uses this data...

## Repository content

This repository is a Visual Studio Code / Platformio project.

### The ***testFiles*** folder contains

- uncompressed.txt : a sample CSV file
- comressed.txt : the compressed version

### The **javascriptVersion** folder contains

- [asciiCompressDecompress_original.html](https://philippeardit.github.io/asciiCompressDecompress/javascriptVersion/asciiCompressDecompress_original.html), just a extraxtion of the original javascript code
- [asciiCompressDecompress.html](https://philippeardit.github.io/asciiCompressDecompress/javascriptVersion/asciiCompressDecompress.html), where I've extracted and reviewed the javascript code for a better rewriting in another language.

### The **pythonVersion** folder

contains the same code written in Python (tested with python3 on MacOS).

Test : the following command lines must be silent: the goal is to compress or decompress and compare the result with what is expected.

#### Test Python compress

    python3 ./pythonVersion/asciiCompress.py < ./testFiles/uncompressed.txt > tmp.txt; diff tmp.txt ./testFiles/compressed.txt; rm tmp.txt

#### Test Python decompress

    python3 ./pythonVersion/asciiDecompress.py < ./testFiles/compressed.txt > tmp.txt; diff tmp.txt ./testFiles/uncompressed.txt; rm tmp.txt

### The **include** folder contains the same code written in C

A sample C program is available too (src/main.c)

Once inside platformio, "pio run" will compile and generate .pio/build/native/program for your platform.

Test : the following command lines must be silent: the goal is to compress or decompress and compare the result with what is expected.

#### Test C compress

    ./.pio/build/native/program compress < ./testFiles/uncompressed.txt > tmp.txt; diff tmp.txt ./testFiles/compressed.txt; rm tmp.txt
or

    ./.pio/build/native/program compress ./testFiles/uncompressed.txt > tmp.txt ; diff tmp.txt ./testFiles/compressed.txt; rm tmp.txt
***but not yet***

    cat ./testFiles/uncompressed.txt | ./.pio/build/native/program compress > tmp.txt ; diff tmp.txt ./testFiles/compressed.txt; rm tmp.txt

#### Test C decompress

    ./.pio/build/native/program decompress < ./testFiles/compressed.txt > tmp.txt; diff tmp.txt ./testFiles/uncompressed.txt; rm tmp.txt
or

    ./.pio/build/native/program decompress ./testFiles/compressed.txt > tmp.txt ; diff tmp.txt ./testFiles/uncompressed.txt; rm tmp.txt
***but not yet***

    cat ./testFiles/compressed.txt | ./.pio/build/native/program decompress  > tmp.txt ; diff tmp.txt ./testFiles/uncompressed.txt; rm tmp.txt
