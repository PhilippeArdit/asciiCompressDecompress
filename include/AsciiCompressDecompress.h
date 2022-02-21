/**
 * @file AsciiCompressDecompress.h
 *
 * Copyright (c) 2022 Philippe Ardit & Ionbird
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

const size_t txtBufSize = 9000;
char *txtBuf;
size_t txtBufEosPos = 0;
size_t txtBufStartPos = 0;
size_t nbFullBuff = 0;
size_t indexInputText = 0;
size_t Gsm = 0;
unsigned int Gsf = 0;
bool bIsCompressProcess = false;
FILE *_outPutStream;
void init();
void end();
void addCharToTxtBuff(const char c);
char readCharFromTxtBuff(size_t iVirtualPos);
void addCharToOutputText(char c);
void sa(size_t sab, char *inputText);
void sb(unsigned int sbsh);
size_t nbrOutputChars = 0;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// String buffer to store the latest characters of :
// - substring while compression
// - output string while decompression
//      The decompress algorythm may read back up to 8935 position
//      to copy some repeated substring at the end
//      Nb : no shift procesus here (too long) but a start and end pos index

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
}

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
    if (nbrOutputChars - iVirtualPos > txtBufSize)
        return '\0'; // We should never be here !

    size_t iPos = iVirtualPos % txtBufSize;
    return txtBuf[iPos];
}

void init(FILE *outPutStream)
{
    nbrOutputChars = 0;
    txtBufEosPos = 0;
    txtBuf = (char *)calloc(txtBufSize, sizeof(char));
    _outPutStream = outPutStream;
}
void end()
{
    free(txtBuf);
}

void addCharToOutputText(char c)
{
    if (!bIsCompressProcess)
        addCharToTxtBuff(c);
    putc(c, _outPutStream);
    nbrOutputChars++;
}

void sa(size_t sab, char *inputText)
{
    unsigned int sasl = 0;
    while (0 < sab)
    {
        sab -= 1;
        sasl = inputText[indexInputText] & 127;
        indexInputText += 1;
        if ((32 > sasl && 9 != sasl && 10 != sasl) || (127 == sasl))
        {
            addCharToOutputText('`');
            sasl = sasl + 34 & 127;
        }
        else
        {
            if (96 == sasl)
            {
                addCharToOutputText('`');
            }
        }
        addCharToOutputText(sasl);
    }
}

void sb(unsigned int sbsh)
{
    unsigned int sbsl = 0;
    indexInputText += sbsh;
    Gsm -= sbsh;
    addCharToOutputText('`');
    Gsf -= 5;
    sbsh -= 5;
    sbsh += 66;
    if (96 <= sbsh)
    {
        sbsh += 1;
    }
    addCharToOutputText(sbsh);
    sbsl = Gsf % 94;
    Gsf = (Gsf - sbsl) / 94;
    addCharToOutputText(sbsl + 33);
    addCharToOutputText(Gsf + 33);
}

size_t AsciiCompressTxt(char *inputText, size_t lenInputText, FILE *outPutStream)
{
    init(outPutStream);
    bIsCompressProcess = true;
    indexInputText = 0;
    Gsm = 0;
    Gsf = 0;
    unsigned int sh = 0;
    int sp2;
    while (indexInputText < lenInputText)
    {
        // addCharToOutputText(inputText[indexInputText++]);
        if (5 > Gsm)
            Gsm = 5;

        if (indexInputText + Gsm > lenInputText)
        {
            if (sh)
            {
                sb(sh);
                sh = 0;
            }
            sa(lenInputText - indexInputText, inputText);
        }
        if (indexInputText < lenInputText)
        {
            size_t n = 0;
            if (8840 < indexInputText)
                n = indexInputText - 8840;

            // sp1 = _substr(inputText, indexInputText, Gsm);
            // sp2 = _lastIndexOf(_substring(inputText, n, indexInputText), sp1);
            sp2 = -1;
            size_t j = 0;
            for (char *pInText = inputText + n; pInText < inputText + indexInputText && j + n <= indexInputText - Gsm; pInText++)
            {
                char *p1 = pInText;
                char *p2 = inputText + indexInputText;
                size_t i = 0;
                while (i < Gsm && *p1 == *p2)
                {
                    p1++;
                    p2++;
                    i++;
                }
                if (i == Gsm)
                    sp2 = j;
                j++;
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
                        sb(sh);
                        sh = 0;
                    }
                }
            }
            else
            {
                if (sh)
                {
                    sb(sh);
                    sh = 0;
                }
                else
                    sa(1, inputText);
            }
        }
    }
    end();
    return nbrOutputChars;
}

size_t AsciiDecompressTxt(char *inputText, size_t lenInputText, FILE *outPutStream)
{
    init(outPutStream);
    bIsCompressProcess = false;
    indexInputText = 0;

    unsigned int cCode = 0;
    int td = 0;
    unsigned int lenRepeat = 0;
    while (indexInputText < lenInputText)
    {
        // addCharToOutputText(inputText[indexInputText++]);

        cCode = inputText[indexInputText];
        indexInputText += 1;
        if (96 == cCode) // ` : escape code
        {                // if not a ` again, both following chars tel where to go back an how many characters to copy again
            cCode = inputText[indexInputText];
            indexInputText += 1;
            if (96 == cCode) // ` Grave accent
                addCharToOutputText(cCode);
            else
            {
                if (65 < cCode) // ` : escape code
                {
                    if (96 < cCode)
                        cCode -= 62; // min 97-62 = 35, max 126-62 = 64
                    else
                        cCode -= 61;   // max 96-61 = 35, min 66-61 = 5
                    lenRepeat = cCode; // min : 5, max : 64

                    cCode = inputText[indexInputText]; // 10 -> 127
                    indexInputText += 1;
                    td = cCode - 28;                   // -18 -> 99
                    cCode = inputText[indexInputText]; // 10 -> 127
                    indexInputText += 1;
                    td += 94 * (cCode - 33); // 99 + 94 * (127-33) = 8935

                    // tf += _substr(tf, _length(tf) - td, lenRepeat);
                    size_t iPos = nbrOutputChars - td;
                    for (size_t i = 0; i < lenRepeat; i++)
                        addCharToOutputText(readCharFromTxtBuff(iPos + i));
                }
                else
                {
                    if (32 < cCode)
                        addCharToOutputText(cCode - 34 & 127);
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

#endif
