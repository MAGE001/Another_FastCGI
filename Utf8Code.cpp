#include <wchar.h>
#include <errno.h>
#include <ctype.h>

#include "Utf8Code.h"

CUtf8Code::CUtf8Code()
{
}

CUtf8Code::~CUtf8Code()
{
}

int CUtf8Code::UnicodeToUtf8(const wchar_t *pIn,char*pOut,int size)
{
    iconv_t v = iconv_open("utf-8", "unicode");
    memset(pOut,0,size);

    int loop = wcslen(pIn);
    int k = (size - 1)/2;
    while(loop-- && k--) {
        char **pin = (char**)&pIn;
        char **pout = (char**)&pOut;
        size_t nIn = sizeof(wchar_t);
        size_t nOut = sizeof(wchar_t);
        size_t ret = iconv(v, pin, &nIn, pout, &nOut);
        if(ret == (size_t)-1 && (errno == EILSEQ || errno == EINVAL)) {
            iconv_close(v);
            return -1;
        }
        pOut--;
    }
    iconv_close(v);
    return 0;
}


int CUtf8Code::Utf8ToUnicode(const char *pIn,wchar_t *pOut,int size)
{
    memset(pOut,0,size);

    size_t len = strlen(pIn);
    size_t nOut = len*sizeof(wchar_t);
    size_t num = nOut;

    char *pBuf = new char[(len+1)*sizeof(wchar_t)];
    char *pfree = pBuf;
    memset(pBuf,0,(len+1)*sizeof(wchar_t));

    unsigned short *pSrc = (unsigned short*)pBuf + 1;//ignore 0xff 0xfe

    iconv_t v = iconv_open("unicode","utf-8");
    size_t ret = iconv(v, (char**)&pIn, &len, (char**)&pBuf, &nOut);
    iconv_close(v);
    if(ret == (size_t)-1 && (errno == EILSEQ || errno == EINVAL)) {
        return -1;
    }


    int loop = (num - nOut)/2;
    int outlen = size;
    while(loop-- && outlen--) {
        *pOut++ = (*pSrc++);
    }
    delete[] pfree;
    pfree = NULL;
    return 0;
}


void CUtf8Code::UrlUTF8Decode(const char* pInData, char* pOutData,int size)
{
    char tmp[2] = {0};
    int i = 0;
    int len = strlen(pInData);
    int k = len < size? len : size;

    while(i < k) {
        if(pInData[i] == '%') {
            tmp[0] = pInData[i + 1];
            tmp[1] = pInData[i + 2];

            *pOutData = StrToBin(tmp);
            i = i + 3;
        } else {
            *pOutData = pInData[i];
            i++;
        }
        pOutData ++;
    }

    return ;
}


void CUtf8Code::Utf8ToUrl(const char *pInData,std::string &strOut)
{
    size_t len = strlen(pInData);
    for(size_t i = 0; i < len; i++) {
        if(isalnum(pInData[i])) {
            char tmp[2]= {0};
            snprintf(tmp,sizeof(tmp),"%c",pInData[i]);
            strOut.append(tmp);
        } else if(isspace(pInData[i])) {
            strOut.append("+");
        } else {
            char tmp[4] = {0};
            snprintf(tmp,sizeof(tmp),"%%%X%X",((unsigned char)pInData[i])>>4,((unsigned char)pInData[i])&0xF);
            strOut.append(tmp);
        }
    }
}

char CUtf8Code::CharToInt(char ch)
{
    if(ch >= '0' && ch <= '9')return (char)(ch - '0');
    if(ch >= 'a' && ch <= 'f')return (char)(ch - 'a' + 10);
    if(ch >= 'A' && ch <= 'F')return (char)(ch - 'A' + 10);

    return -1;
}

char CUtf8Code::StrToBin(char* str)
{
    char tempWord[2] = {0};
    char chn = '\0';

    tempWord[0] = CharToInt(str[0]);
    tempWord[1] = CharToInt(str[1]);

    chn = (tempWord[0] << 4) | tempWord[1];
    return chn;
}


int CUtf8Code::charConver(const char* pIn,char *pOut,int size,const char *pTo,const char *pFrom)
{
    if(pIn == NULL || pOut == NULL || pTo == NULL || pFrom == NULL)
        return -2;
    size_t len = strlen(pIn);
    size_t outlen = size;
    char *p1 = const_cast<char *>(pIn);
    char *p2 = pOut;

    iconv_t v = iconv_open(pTo,pFrom);//iconv_open("gbk","utf-8");
    if(v == iconv_t(-1)) {
        printf("iconv_open err %d,%s\n",errno,strerror(errno));
        return -1;
    }
    size_t ret = iconv(v, (char**)&p1, &len, (char**)&p2, &outlen);
    iconv_close(v);

    if(ret == (size_t)-1 && (errno == EILSEQ || errno == EINVAL)) {
        return -1;
    }

    return 0;
}
