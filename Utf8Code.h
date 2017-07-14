#ifndef CUTF8CODE_H
#define CUTF8CODE_H
#include <iconv.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

class CUtf8Code
{
 public:
    CUtf8Code();
    ~CUtf8Code();
    void UrlUTF8Decode(const char* pInData, char* pOutData,int size);
    int UnicodeToUtf8(const wchar_t *pIn,char*pOut,int size);
    int  Utf8ToUnicode(const char *pIn,wchar_t *pOut,int size);
    void Utf8ToUrl(const char *pInData,std::string &strOut);
    char StrToBin(char* str);
    char CharToInt(char ch);
    int charConver(const char* pIn,char *pOut,int size,const char *pTo,const char *pFrom);
};

#endif // CUTF8CODE_H
