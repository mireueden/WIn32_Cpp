#pragma once

#include "idefine.h"

// brga, stride, w, h
typedef void (*MethodImageFilter)(uint8* bgra, int width, int height, int stride);

void setImageFilter(MethodImageFilter method);
Texture* createImageFilter(const char* szFormat, ...);

// 이미지 필터 예제
void ImageFilterGrey(uint8* bgra, int width, int height, int stride);
void ImageFilterMirror(uint8* bgra, int width, int height, int stride);

// grey
// rgb => hsi


Texture* createImageGrey(const char* szFormat, ...);