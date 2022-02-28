// The original script is from http://codelobe.com/tools/ascii-compress :
/* ********** ********** ********** ********** ********** ********** ********** **********
'use strict';new function(){function s(c){function a(b){for(;0<b--;)l=c.charCodeAt(e++)&127,32>l&&9!=l&&10!=l||127==l?(d+="`",l=l+34&127):96==l&&(d+="`"),d+=String.fromCharCode(l)}function b(){h&&(e+=h,m-=h,d+="`",f-=5,h-=5,h+=66,96<=h&&(h+=1),d+=String.fromCharCode(h),l=f%94,f=(f-l)/94,d+=String.fromCharCode(l+33),d+=String.fromCharCode(f+33),h=0)}for(var d="",e=0,m=0,f=0,h=0,p,k=c.length,l;e<k;){5>m&&(m=5);e+m>k&&(b(),a(k-e));p=c.substr(e,m);var n=8840<e?e-8840:0;p=c.substring(n,e).lastIndexOf(p);0<=p?(f=e-
(n+p),h=m++,64<=h&&b()):h?b():a(1)}return d}function t(c){function a(){b=c.charCodeAt(m++)}for(var b,d,e,m=0,f="";m<c.length;)a(),96==b?(a(),96==b?f+=String.fromCharCode(b):65<b?(e=b-=96<b?62:61,a(),d=b-28,a(),d+=94*(b-33),f+=f.substr(f.length-d,e)):32<b&&(f+=String.fromCharCode(b-34&127))):f+=String.fromCharCode(b);return f}function u(a){return 1>arguments.length?k:k=a}function v(a,g){var b=new FileReader;b.onload=function(d){g&&g(a,b.result)};b.onerror=function(b){g&&g(a,null)};b.readAsArrayBuffer(a)}
function r(){var c=0,g=0,b;for(b in a)if(c++,a[b].el)g++;else{var d=document.getElementById(b);if(a[b].el=d)if(g++,null!=a[b].ev)for(var e=0;e<a[b].ev.length;e+=2)d.addEventListener(a[b].ev[e],a[b].ev[e+1],!1)}if(g<c)window.setTimeout(r,100);else{for(b in a)a[b]=a[b].el;w()}}function n(c){for(;a.fileName.firstChild;)a.fileName.removeChild(a.fileName.firstChild);a.fileName.appendChild(document.createTextNode(c))}function x(c){1>c.length||new v(c[0],function(c,b){if(b){for(var d=new Uint8Array(b),e=
"",m=d.length,f=0,h=0;h<m;++h){var k=d[h];127<k?f++:e+=String.fromCharCode(k)}n("Loaded: "+c.name+(f?" ("+f+" invalid chars removed)":""));a.inputTxt.value=e}else a.inputTxt.value="Error Loading file: "+c.name})}function w(){function c(a){a.preventDefault();a.stopPropagation()}var g=document.getElementsByTagName("body")[0];g.addEventListener("dragenter",c,!1);g.addEventListener("dragleave",c,!1);g.addEventListener("dragover",c,!1);g.addEventListener("drop",function(b){if(null!=k){var d=b.dataTransfer.files;
if(!(1>d.length)){try{k(d)}catch(e){a.inputTxt.value="Error accepting files: "+e}c(b)}}},!1);u(x)}var k=null,a={docBody:null,loadBtn:["click",function(){null==k||null==a.fileInput||a.fileInput.click()}],saveBtn:["click",function(){var c=document.createElement("a");c.href="data:text/plain;base64,"+btoa(a.inputTxt.value).replace(/=/g,"%3D");c.download="compressed.txt";a.docBody.appendChild(c);c.click();a.docBody.removeChild(c)}],inputTxt:null,fileInput:["change",function(){null==k||1>a.fileInput.files.length||
k(a.fileInput.files)}],fileName:null,compressBtn:["click",function(){var c=a.inputTxt.value.length,g=(a.inputTxt.value=s(a.inputTxt.value)).length;n("Compressed "+c+" chars to "+g+(0==c?"":" (saved "+(1E4-1E4*(g/c)|0)/100+"%)"))}],decompressBtn:["click",function(){var c=a.inputTxt.value.length,g=(a.inputTxt.value=t(a.inputTxt.value)).length;n("Decompressed "+c+" chars to "+g+(0==c?"":" (grew "+(1E4*(g/c)|0)/100+"%)"))}],selectAllBtn:["click",function(){a.inputTxt.select()}]},q;for(q in a)a[q]={el:null,
ev:a[q]};r()};
   ********** ********** ********** ********** ********** ********** ********** ********** */

// The following one is a more "human reading" version to be translated in Python and C
// Usage : 
//  decompressedTxt = decompressTxt( compressedTxt )
//  compressedTxt = compressTxt( decompressedTxt )

function _charCodeAt(string, index) {
    return string.charCodeAt(index)
}

function _fromCharCode(charCode) {
    return String.fromCharCode(charCode)
}

function _substr(string, start, length) {
    return string.substr(start, length) // the second argument to substr is the maximum length to return
}

function _substring(string, start, stopAt) {
    return string.substring(start, stopAt) // The second argument to substring is the index to stop at(but not include)
}

function _lastIndexOf(string, searchValue) {
    return string.lastIndexOf(searchValue)
}

function _length(string) {
    return string.length
}

var indexInputText = 0
var outputText = ""
var Gsm = 0
var Gsf = 0

function sa(sab, inputText) {
    var sasl = 0
    while (0 < sab) {
        sab -= 1
        sasl = _charCodeAt(inputText, indexInputText) & 127
        indexInputText += 1
        if (32 > sasl && 9 != sasl && 10 != sasl || 127 == sasl) {
            outputText += "`"
            sasl = sasl + 34 & 127
        } else {
            if (96 == sasl) {
                outputText += "`"
            }
        }
        outputText += _fromCharCode(sasl)
    }
}

function sb(sbsh) {
    var sbsl = 0

    indexInputText += sbsh
    Gsm -= sbsh
    outputText += "`"
    Gsf -= 5
    sbsh -= 5
    sbsh += 66
    if (96 <= sbsh) {
        sbsh += 1
    }
    outputText += _fromCharCode(sbsh)
    sbsl = Gsf % 94
    Gsf = (Gsf - sbsl) / 94
    outputText += _fromCharCode(sbsl + 33)
    outputText += _fromCharCode(Gsf + 33)
}

function compressTxt(inputText) {
    outputText = ""
    indexInputText = 0
    Gsm = 0
    Gsf = 0
    var sh = 0
    var sp = ""
    var sp2 = 0
    var lenInputText = _length(inputText)
    while (indexInputText < lenInputText) {
        if (5 > Gsm) {
            Gsm = 5
        }

        if (indexInputText + Gsm > lenInputText) {
            if (sh) {
                sb(sh)
                sh = 0
            }
            sa(lenInputText - indexInputText, inputText)
        }
        if (indexInputText < lenInputText) {

            var n = 0
            if (8840 < indexInputText)
                n = indexInputText - 8840

            sp = _substr(inputText, indexInputText, Gsm)
            sp2 = _lastIndexOf(_substring(inputText, n, indexInputText), sp)

            if (0 <= sp2) {
                Gsf = indexInputText - (n + sp2)

                sh = Gsm
                Gsm += 1

                if (64 <= sh) {
                    if (sh) {
                        sb(sh)
                        sh = 0
                    }
                }
            } else {
                if (sh) {
                    sb(sh)
                    sh = 0
                } else {
                    sa(1, inputText)
                }
            }
        }
    }
    return outputText
}

function decompressTxt(inputText) {
    var tb = 0
    var td = 0
    var te = 0
    var tm = 0
    var tf = ""
    while (tm < _length(inputText)) {
        tb = _charCodeAt(inputText, tm)
        tm += 1
        if (96 == tb) {
            tb = _charCodeAt(inputText, tm)
            tm += 1
            if (96 == tb) {
                tf += _fromCharCode(tb)
            } else {
                if (65 < tb) {
                    if (96 < tb) {
                        tb -= 62
                    } else {
                        tb -= 61
                    }
                    te = tb

                    tb = _charCodeAt(inputText, tm)
                    tm += 1
                    td = tb - 28
                    tb = _charCodeAt(inputText, tm)
                    tm += 1
                    td += 94 * (tb - 33)
                    tf += _substr(tf, _length(tf) - td, te)
                } else {
                    if (32 < tb) {
                        tf += _fromCharCode(tb - 34 & 127)
                    }
                }
            }
        } else {
            tf += _fromCharCode(tb)
        }
    }
    return tf
}