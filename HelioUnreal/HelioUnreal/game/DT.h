#pragma once

#include "iStd.h"

void loadDT();
void freeDT();
void drawDT(float dt);
void keyDT(iKeyStat stat, iPoint point);

// DTLogin.h / cpp
// 로그인 : 사원번호 / 비밀번호

// DTProc.h / cpp
// DTNotice.h / cpp
// 공지사항(점검 / 유의사항)
// DTProcObj(super) + DTProcMake + DTProcRepair . h/cpp
// 공정(하나의 부품 => 제품화 되는 과정)
// DTNoticeUI.h / cpp
// 고장나면 표시할 디스플레이, 수리의 방법, 알림
// DTNoticePop.h / cpp
// 수주 관련 정보(팝업)
// 로그아웃
