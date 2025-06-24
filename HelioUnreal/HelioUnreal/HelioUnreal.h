#pragma once

#include "resource.h"

#include "idefine.h"
#include "Game.h"

void resize(int width, int height);
iPoint convertCoord(float x, float y);

#if 0

Win32 API + GDI / GDI +
OpenGL1.x
OpenGL3.x + glsl
OpenAL

디버깅 시작 F5
브레이크 포인트 F9
스텝 오버 F10(해당 라인을 실행)
스텝 인투 F11(함수등에 들어갈 수 있는 곳으로 실행)
Q) 실수형의 값을 디버깅 모드에서 알 수 없나 ? !
Q) include ", < 구분
Q) 디버깅 nan, inf 언제 등장

Q)링크 리스트 헤드 타입
Q)WM_SIZING reinterpret_cast 에 대해
Q)iPointAbs(수식) 왜 여기에 에러 ? 수식이 들어가면 에러발생

#endif

