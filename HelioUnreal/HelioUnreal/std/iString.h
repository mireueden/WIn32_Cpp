#pragma once


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


    bool trim();
    char* subString(int from, int to);
    char* copy();
};

