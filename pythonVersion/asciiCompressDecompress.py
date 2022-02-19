# javascript to python : https://nemisj.com/python-api-javascript/

def _charCodeAt(string, index):
    return ord(string[index])


def _fromCharCode(charCode):
    return chr(charCode)


def _substr(string, start, length):
    return string[start:start+length]


def _substring(string, start, stopAt):
    return string[start:stopAt]


def _lastIndexOf(string, searchValue):
    return string.rfind(searchValue)


def _length(string):
    return len(string)


def compressTxt(inputText):
    def sa(sab, inputText):
        nonlocal Gse
        nonlocal GcompressedTxt
        nonlocal Gsm
        nonlocal Gsf
        sasl = 0
        while (0 < sab):
            sab -= 1
            sasl = _charCodeAt(inputText, Gse) & 127
            Gse += 1
            if (32 > sasl and 9 != sasl and 10 != sasl or 127 == sasl):
                GcompressedTxt += "`"
                sasl = sasl + 34 & 127
            else:
                if (96 == sasl):
                    GcompressedTxt += "`"
            GcompressedTxt += _fromCharCode(sasl)

    def sb(sbsh):
        nonlocal Gse
        nonlocal GcompressedTxt
        nonlocal Gsm
        nonlocal Gsf

        sbsl = 0

        Gse += sbsh
        Gsm -= sbsh
        GcompressedTxt += "`"
        Gsf -= 5
        sbsh -= 5
        sbsh += 66
        if (96 <= sbsh):
            sbsh += 1

        GcompressedTxt += _fromCharCode(sbsh)
        sbsl = Gsf % 94
        Gsf = (Gsf - sbsl) // 94
        GcompressedTxt += _fromCharCode(sbsl + 33)
        GcompressedTxt += _fromCharCode(Gsf + 33)

    GcompressedTxt = ""
    Gse = 0
    Gsm = 0
    Gsf = 0
    sh = 0
    sp = ""
    sk = _length(inputText)
    while (Gse < sk):
        if (5 > Gsm):
            Gsm = 5

        if (Gse + Gsm > sk):
            if (sh):
                sb(sh)
                sh = 0

            sa(sk - Gse, inputText)

        sp = _substr(inputText, Gse, Gsm)
        n = 0
        if (8840 < Gse):
            n = Gse - 8840
        sp = _lastIndexOf(_substring(inputText, n, Gse), sp)
        if (0 <= sp):
            Gsf = Gse - (n + sp)
            sh = Gsm
            Gsm += 1
            if (64 <= sh):
                if (sh):
                    sb(sh)
                    sh = 0
        else:
            if (sh):
                sb(sh)
                sh = 0
            else:
                sa(1, inputText)

    return GcompressedTxt


def decompressTxt(inputText):
    tb = 0
    td = 0
    te = 0
    tm = 0
    tf = ""
    while (tm < _length(inputText)):
        tb = _charCodeAt(inputText, tm)
        tm += 1
        if (96 == tb):
            tb = _charCodeAt(inputText, tm)
            tm += 1
            if (96 == tb):
                tf += _fromCharCode(tb)
            else:
                if (65 < tb):
                    if (96 < tb):
                        tb -= 62
                    else:
                        tb -= 61
                    te = tb

                    tb = _charCodeAt(inputText, tm)
                    tm += 1
                    td = tb - 28
                    tb = _charCodeAt(inputText, tm)
                    tm += 1
                    td += 94 * (tb - 33)
                    tf += _substr(tf, _length(tf) - td, te)
                else:
                    if (32 < tb):
                        tf += _fromCharCode(tb - 34 & 127)

        else:
            tf += _fromCharCode(tb)

    return tf
