/**
 * @file AsciiCompressDecompress.h
 *
 * Copyright (c) 2022 Philippe Ardit
 * Author : Philippe Ardit
 * Create Time : 02/2022
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#ifndef _AsciiCompressDecompress_hpp
#define _AsciiCompressDecompress_hpp

/**
 * @brief Inspired from ASCII Fold - Plain Text Compression - CodeLobe.com
 * http://codelobe.com/tools/ascii-compress, compress and decompress ASCII texts
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

class AsciiCompressDecompress
{
private:
    size_t txtBufSize, txtBufEosPos, txtBufStartPos, nbFullBuff, indexInputText,
        Gsm, nbrOutputChars, nbrVirtualChars, lenInputText;
    unsigned int Gsf;
    bool bIsCompressProcess;
    char *txtBuf;
    FILE *_outPutStream;

    bool init(FILE *inputFile, FILE *outPutStream)
    {
        txtBufSize = 9000;
        txtBufEosPos = 0;
        txtBufStartPos = 0;
        nbFullBuff = 0;
        indexInputText = 0;
        Gsm = 0;
        Gsf = 0;
        bIsCompressProcess = false;
        nbrOutputChars = 0;
        nbrVirtualChars = 0;
        nbrOutputChars = 0;
        txtBuf = (char *)calloc(txtBufSize, sizeof(char));
        _outPutStream = outPutStream;

        fseek(inputFile, 0, SEEK_END);
        lenInputText = ftell(inputFile);
        fseek(inputFile, 0, SEEK_SET);

        return txtBuf != NULL;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // String buffer to store the latest characters of :
    // - substring while compression
    // - output string while decompression
    //      The decompress algorythm may read back up to 8935 position
    //      to copy some repeated substring at the end
    //      Nb : no shift procesus here (too long) but a start and end pos index

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void addCharToTxtBuff(const char c)
    {
        txtBuf[txtBufEosPos] = c;
        txtBufEosPos++;
        if (txtBufEosPos == txtBufSize)
        {
            nbFullBuff++;     // we filled one full buffer more
            txtBufEosPos = 0; // we go back to the buffer start
        }
        txtBuf[txtBufEosPos] = '\0';
        if (nbFullBuff)
            txtBufStartPos = txtBufEosPos + 1;
        if (txtBufStartPos == txtBufSize)
            txtBufStartPos = 0;
        nbrVirtualChars++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    char readCharFromTxtBuff(size_t iVirtualPos)
    {
        /*
        Step 1 : buffer not full
            012345678901234567890123456789  ->  txtBufSize = 40
                v                           ->  txtBufEosPos = 4
            1234                            ->  outputTextLength = 4
            ^                               ->  txtBufStartPos = 0
            iVirtualPos = 3 -> iPos = iVirtualPos % txtBufSize = 3 % 40 =  3

        Step 2 : buffer full one time
            012345678901234567890123456789  ->  txtBufSize = 40
                v                           ->  txtBufEosPos = 4
            .....6789012345678901234567890
            1234                            ->  outputTextLength = 44
                 ^                          ->  txtBufStartPos = 5
            iVirtualPos = 13 -> iPos = 13 -> "4" : iPos = iVirtualPos % txtBufSize = 13 % 40 = 13
            iVirtualPos = 43 -> iPos =  3 -> "4" : iPos = iVirtualPos % txtBufSize = 43 % 40 =  3

        Step 3 : buffer full two times
            012345678901234567890123456789  ->  txtBufSize = 40
                v                           ->  txtBufEosPos = 4
            ..............................
            .....6789012345678901234567890
            1234                            ->  outputTextLength = 84
                 ^                          ->  txtBufStartPos = 5
            iVirtualPos = 83 -> iPos =  3 -> "4" : iPos = iVirtualPos % txtBufSize = 83 % 40 =  3
            iVirtualPos = 53 -> iPos = 13 -> "4" : iPos = iVirtualPos % txtBufSize = 53 % 40 = 13

        */
        // if (nbrVirtualChars - iVirtualPos > txtBufSize)
        //     return '\0'; // We should never be here !

        size_t iPos = iVirtualPos % txtBufSize;
        return txtBuf[iPos];
    }

    void getNextCharFromTxtBuff(size_t iVirtualPos, char **p)
    {
        if (*p == 0)
            *p = txtBuf + (iVirtualPos % txtBufSize);
        else
            getNextCharFromTxtBuff(p);
    }

    void getNextCharFromTxtBuff(char **p)
    {
        (*p)++;
        if (*p == txtBuf + txtBufSize)
            *p = txtBuf;
    }

    void end()
    {
        free(txtBuf);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void addCharToOutputText(char c)
    {
        if (!bIsCompressProcess)
            addCharToTxtBuff(c);
        putc(c, _outPutStream);
        nbrOutputChars++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void fsa(size_t sab, FILE *inputFile)
    {
        unsigned int sasl = 0;
        while (0 < sab)
        {
            sab -= 1;
            addCharToTxtBuff(fgetc(inputFile));
            sasl = readCharFromTxtBuff(indexInputText) & 127;
            indexInputText += 1;

            if ((32 > sasl && 9 != sasl && 10 != sasl) || (127 == sasl))
            {
                addCharToOutputText('`');
                sasl = (sasl + 34) & 127;
            }
            else
            {
                if (96 == sasl)
                    addCharToOutputText('`');
            }
            addCharToOutputText(sasl);
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    void fsb(unsigned int sbsh, FILE *inputFile)
    {
        unsigned int sbsl = 0;
        indexInputText += sbsh;
        for (size_t i = 0; i < sbsh; i++)
            addCharToTxtBuff(fgetc(inputFile));

        Gsm -= sbsh;
        addCharToOutputText('`');
        Gsf -= 5;
        sbsh -= 5;
        sbsh += 66;
        if (96 <= sbsh)
            sbsh += 1;
        addCharToOutputText(sbsh);
        sbsl = Gsf % 94;
        Gsf = (Gsf - sbsl) / 94;
        addCharToOutputText(sbsl + 33);
        addCharToOutputText(Gsf + 33);
    }

public:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    size_t compressFile(FILE *inputFile, FILE *outPutStream)
    {
        if (!init(inputFile, outPutStream))
            return 0;
        bIsCompressProcess = true;
        unsigned int sh = 0;
        int sp2;
        size_t i, n, jMax;
        char *p02, *p1, *p2;

        while (indexInputText < lenInputText)
        {
            if (5 > Gsm)
                Gsm = 5;
            if (indexInputText + Gsm > lenInputText)
            {
                if (sh)
                {
                    fsb(sh, inputFile);
                    sh = 0;
                }
                fsa(lenInputText - indexInputText, inputFile);
            }
            if (indexInputText < lenInputText)
            {
                n = 0;
                if (8840 < indexInputText)
                    n = indexInputText - 8840;

                while (nbrVirtualChars < indexInputText + Gsm && nbrVirtualChars < lenInputText)
                    addCharToTxtBuff(fgetc(inputFile));

                sp2 = -1;
                p02 = 0;
                p1 = 0;
                getNextCharFromTxtBuff(indexInputText, &p02);
                jMax = indexInputText - n;
                if (jMax + 1 - Gsm < jMax)
                    jMax = jMax - Gsm + 1;
                for (size_t j = 0; j < jMax; j++)
                {
                    getNextCharFromTxtBuff(n + j, &p1);
                    if (*p1 == *p02)
                    {
                        p2 = p02;
                        i = 0;
                        while (i < Gsm && *p1 == *p2)
                        {
                            i++;
                            getNextCharFromTxtBuff(&p1);
                            getNextCharFromTxtBuff(&p2);
                        }
                        if (i == Gsm)
                            sp2 = j;
                        p1 = 0;
                    }
                }

                if (0 <= sp2)
                {
                    Gsf = indexInputText - (n + sp2);
                    sh = Gsm;
                    Gsm += 1;
                    if (64 <= sh)
                    {
                        if (sh)
                        {
                            fsb(sh, inputFile);
                            sh = 0;
                        }
                    }
                }
                else
                {
                    if (sh)
                    {
                        fsb(sh, inputFile);
                        sh = 0;
                    }
                    else
                        fsa(1, inputFile);
                }
            }
        }
        end();
        return nbrOutputChars;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    size_t decompressFile(FILE *inputFile, FILE *outPutStream)
    {
        if (!init(inputFile, outPutStream))
            return 0;
        bIsCompressProcess = false;

        unsigned int cCode, lenRepeat;
        int td;

        while (indexInputText < lenInputText)
        {
            cCode = fgetc(inputFile);
            indexInputText += 1;

            if (96 == cCode)
            {
                cCode = fgetc(inputFile);
                indexInputText += 1;
                if (96 == cCode)
                    addCharToOutputText(cCode);
                else
                {
                    if (65 < cCode)
                    {
                        if (96 < cCode)
                            cCode -= 62;
                        else
                            cCode -= 61;
                        lenRepeat = cCode;

                        cCode = fgetc(inputFile);
                        indexInputText += 1;
                        td = cCode - 28;
                        cCode = fgetc(inputFile);
                        indexInputText += 1;
                        td += 94 * (cCode - 33);

                        size_t iPos = nbrOutputChars - td;
                        for (size_t i = 0; i < lenRepeat; i++)
                            addCharToOutputText(readCharFromTxtBuff(iPos + i));
                    }
                    else
                    {
                        if (32 < cCode)
                            addCharToOutputText((cCode - 34) & 127);
                    }
                }
            }
            else
            {
                addCharToOutputText(cCode);
            }
        }
        end();
        return nbrOutputChars;
    }
} asciiCompressDecompress;

#endif
