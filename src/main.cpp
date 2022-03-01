/**
 * @file main.c
 * @author Philippe Ardit (philippe.ardit@free.fr)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 * test compress :              pio run; ./.pio/build/native/program compress ./testFiles/uncompressed.txt > tmp.txt ; diff tmp.txt ./testFiles/compressed.txt; rm tmp.txt
 * test decompress :            pio run; ./.pio/build/native/program decompress ./testFiles/compressed.txt > tmp.txt ; diff tmp.txt ./testFiles/uncompressed.txt; rm tmp.txt
 *
 */
#include <stdio.h>
#include <AsciiCompressDecompress.hpp>
#include <sys/time.h>

// from https://youngmok.com/precise-elapsed-time-measurement-in-c-program/
double getTimeElapsed(struct timeval end, struct timeval start)
{
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.00;
}

int main(int argc, char *argv[])
{
    /*
        printf("Running %s with %i parameters\n", argv[0], argc - 1);
        for (int i = 1; i < argc; i++)
            printf("- parameter %i : %s\n", i, argv[i]);
    */

    bool bCompress = true;
    int iArg = 0;
    char *arg;
    FILE *fpIn = stdin;
    FILE *fpOut = stdout;
    size_t nbOutChar = 0;
#ifdef DEBUG
    fpIn = fopen("./testFiles/uncompressed.txt", "r");
#else
    if (argc == 1)
    {
        printf("Usage : \t%s (compress|decompress) [inputFile [outputFile]]\n\t\t%s (compress|decompress) < inputFile > outputFile\n", argv[0], argv[0]);
        return 0;
    }
    while (argc > iArg)
    {
        arg = argv[iArg];

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
#endif // DEBUG
       /*    struct timeval tvalBefore, tvalAfter;
           gettimeofday(&tvalBefore, NULL);
       */
    if (bCompress)
        nbOutChar += asciiCompressDecompress.compressFile(fpIn, fpOut);
    else
        nbOutChar += asciiCompressDecompress.decompressFile(fpIn, fpOut);
    /*
    gettimeofday(&tvalAfter, NULL);
    double time_elapsed = getTimeElapsed(tvalAfter, tvalBefore);

     printf("time_elapsed: %lf seconds\n", time_elapsed);
    */
    return 0;
}
