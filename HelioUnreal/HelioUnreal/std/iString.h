#pragma once

#include "iDefine.h"
#include "iStd.h"

struct iString
{
    // Default
    iString();
    iString(const iString &s);
    
    // Custom
    iString(const char* s);
    virtual ~iString();

    char* s;

    iString operator+ (const char* s);
    iString operator+= (const char* s);
    iString operator+= (const iString& s);

    bool operator== (const iString& s);
    bool operator== (const char* s);

    iString& operator= (const iString& s);
    iString operator= (const char* s);

    static char** split(int& lineNum, const char* s, char d = '\n');
    static char** splitWidth(int& lineNum, const char* s, int width); 
    // width를 기준으로 문자열을 나누기
    // 
    // 주인공은 그로 인해 갚는 것이 
    // 불가능한 수준의 빚을 지게 되었다. 
    // 이런 상황에 처한 주인공은 이 상황을 
    // 벗어날 수만 있다면 무엇이든지 
    // 지불할테니 제발 어떠한 누구라도 내게 
    // 기회를 달라며 울부짖는다.
    static void free(char** line, int lineNum);
    static bool isUTF8(const char* str);

    bool trim();
    char* subString(int from, int to);
    char* copy();
    static char* copy(const char* s);
};

