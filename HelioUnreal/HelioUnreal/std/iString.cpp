// HelioUnreal.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "iString.h"
#include <stdio.h>
#include <string.h>


iString::iString()
{
	printf("iString()\n");
	s = NULL;
}

iString::iString(const iString& s)
{
	printf("iString(const iString& s)\n");
	int len = strlen(s.s);
	char* t = new char[len + 1];
	strcpy(t, s.s);
	this->s = t;
}

iString::iString(const char* s)
{
	printf("iString(const char* s)\n");
	int len = strlen(s);
	char* t = new char[len + 1];
	strcpy(t, s);
	this->s = t;
}

iString::~iString()
{
	printf("~iString\n");
	if (s)
		delete s;
}

iString iString::operator+(const char* s)
{
	int len0 = strlen(this->s);// "Hello"
	int len1 = strlen(s);// " World"

	char* t = new char[len0 + len1 + 1];// "Hello World"
	strcpy(t, this->s);
	strcpy(&t[len0], s);

	iString r(t);
	return r;
}

iString iString::operator+=(const char* s)
{
	int len0 = strlen(this->s);
	int len1 = strlen(s);

	char* t = new char[len0 + len1 + 1];
	strcpy(t, this->s);
	strcpy(&t[len0], s);

	if (this->s)
		delete this->s;
	this->s = t;

	return *this;
}

iString iString::operator+=(const iString& s)
{
	int len0 = strlen(this->s);
	int len1 = strlen(s.s);

	char* t = new char[len0 + len1 + 1];
	strcpy(t, this->s);
	strcpy(&t[len0], s.s);

	if (this->s)
		delete this->s;
	this->s = t;

	return *this;;
}


bool iString::operator==(const iString& s)
{
	if (strcmp(this->s, s.s))
		return false;
	return true;
}

bool iString::operator==(const char* s)
{
	if (strcmp(this->s, s))
		return false;
	return true;
}

iString& iString::operator=(const iString& s)
{
	int len = strlen(s.s);
	char* t = new char[len + 1];
	strcpy(t, s.s);

	if (this->s)
		delete this->s;
	this->s = t;

	return *this;
}

iString iString::operator=(const char* s)
{
	int len = strlen(s);
	char* t = new char[len + 1];
	strcpy(t, s);

	if (this->s)
		delete this->s;
	this->s = t;

	return *this;
}




int STRCMP(const char* s0, const char* s1)
{
	int i;
	// for(초기값; 조건; 증감)
	for (i = 0; s0[i]; i++)
	{
		if (s1[i] == NULL || s0[i] != s1[i])// s1의 문자열이 더 짧다
			return 1;
	}
	if (s1[i]) // s1의 문자열이 더 길다
		return 1;

	return 0;
}
#define strcmp STRCMP