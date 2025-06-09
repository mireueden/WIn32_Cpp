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
    static void free(char** line, int lineNum);

    bool trim();
    char* subString(int from, int to);
    char* copy();
};

