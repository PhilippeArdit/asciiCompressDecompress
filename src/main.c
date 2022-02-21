/**
 * @file main.c
 * @author Philippe Ardit (philippe.ardit@free.fr)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 *
 * test compress :              pio run; ./.pio/build/native/program compress ./testFiles/uncompressed.txt tmp.txt ; diff tmp.txt ./testFiles/compressed.txt; rm tmp.txt
 *
 * test decompress :            pio run; ./.pio/build/native/program decompress < compressed.txt > tmp.txt ; diff tmp.txt uncompressed.txt; rm tmp.txt
 *
 */
#include <stdio.h>
#include <AsciiCompressDecompress.h>

int main(int argc, char *argv[])
{
    /*
        printf("Running %s with %i parameters\n", argv[0], argc - 1);
        for (int i = 1; i < argc; i++)
            printf("- parameter %i : %s\n", i, argv[i]);
    */

    if (argc == 1)
    {
        printf("Usage : \t%s (compress|decompress) [inputFile [outputFile]]\n\t\t%s (compress|decompress) < inputFile > outputFile\n", argv[0], argv[0]);
        return 0;
    }
    bool bCompress = true;
    int iArg = 0;
    char *arg;
    FILE *fpIn = stdin;
    FILE *fpOut = stdout;

    while (argc > iArg)
    {
        arg = argv[iArg];
        // printf("- parameter %i : %s\n", iArg, arg);

        // First argument if exists : "decompress" or any string <=> "compress"
        if (iArg == 1)
            bCompress = 0 != strcmp("decompress", arg);

        // Next argument : input file name
        if (iArg == 2)
        {
            if (NULL == (fpIn = fopen(arg, "r")))
            {
                perror("Error while opening the input file ");
                perror(arg);
                perror(" !\n");
                exit(EXIT_FAILURE);
            }
        }

        // Next argument : output file name
        if (iArg == 3)
        {
            if (NULL == (fpOut = fopen(arg, "w")))
            {
                perror("Error while opening the output file ");
                perror(arg);
                perror(" !\n");
                exit(EXIT_FAILURE);
            }
        }
        iArg++;
    }

    const size_t bufSize = 5000000; // a very big buffer for the first version
    char buffer[bufSize];
    char c;
    size_t nbInChar = 0;
    size_t iBuf = 0;
    size_t nbOutChar = 0;

    //   while ((c = (fpIn == NULL ? getchar() : fgetc(fpIn))) != EOF)
    while ((c = fgetc(fpIn)) != EOF)
    {
        nbInChar++;
        buffer[iBuf++] = c;
        if (iBuf == bufSize)
        {
            if (bCompress)
                nbOutChar += AsciiCompressTxt(buffer, iBuf, fpOut);
            else
                nbOutChar += AsciiDecompressTxt(buffer, iBuf, fpOut);
            iBuf = 0;
        }
    }
    if (iBuf)
    {
        if (bCompress)
            nbOutChar += AsciiCompressTxt(buffer, iBuf, fpOut);
        else
            nbOutChar += AsciiDecompressTxt(buffer, iBuf, fpOut);
    }
    return 0;
}