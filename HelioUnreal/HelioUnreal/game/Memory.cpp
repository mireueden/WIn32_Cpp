#include "Memory.h"

char strMem[] = "Hello";

// 정적 메모리
// 속도가 빠름 => 속도가 빠른 단기 기억 메모리(임시 사용 목적)
// 용량이 작음 => 용량을 많이 잡게 되면 속도가 느려짐
// 크기가 고정됨 => 변경이 불가능해 코드의 융통성이 없음, 메모리 낭비가 일어날 수도 있음

// 동적 메모리
// 속도가 덜빠름 => 장기 기억메모리(데이터 보존 목적)
// 내가 지우지 않는 이상 지워지지 않음
// 용량이 가장 큼 => 권한이 큰 만큼 의무도 존재
// 직접 삭제를 해야함(메모리 누수의 가능성)
// 크기가 필요에 따라 융성성있게 변경이 가능함

// 링크 리스트
// 새로운 데이터를 추가하거나 중간에 위치한 데이터를 지우려고 할때 사용함
// 속도가 느림
// 사용하기 좋다, 



// obj 에서는 retain count
// java 에서는 ref count

// JNI(java native interface)
// local/global ref count 참조수를 모두 확인함
// smart point

// 지우는 기능들?
// release
// dispose
// clean
// free
// clear
// delete
//destoroy
//class Super
//{
//	Super()
//	{
//		retainCount = 1;
//	}
//	Super(const Super& s)
//	{
//		retainCount++;
//	}
//	int retainCount;
//}

void cbArray(void* data);



void loadMemory()
{
	// s0:문자열의 길이2 크기3, 버퍼의 크기 60
	//char s0[60] = "Hi";// [3]
	char s0[60] = { 'H', 'i', 0 };// [3]
	// stack(60) + heap(3)
	s0[1] = 'o';// "Ho"

	char* s1 = new char[4];// = "Hi2";
#if 1
	strcpy(s1, "Hi2");// 4 byte
#else
	memcpy(s1, "Hi2", 4);
	s1[3] = 0;
#endif
	delete s1;// "Hello"

	s1 = new char[6];
	strcpy(s1, "Hello");

	s1 = s0;

	iArray* arr = new iArray(cbArray);

	const char* str[] = {
		"Hello", "World", "!!"
	};
	for (int i = 0; i < 3; i++)
	{
		int len = strlen(str[i]);
		char* t = new char[len + 1];
		strcpy(t, str[i]);
		arr->add(t);
	}

	char* sss = new char[4];
	strcpy(sss, "fun");
	arr->add(1, sss);

	for (int n = 0; n < arr->count; n++)
	{
		char* t = (char*)arr->at(n);
		printf("arr[%d] = %s\n", n, t);
	}
	delete arr;
}

void cbArray(void* data)
{
	char* s = (char*)data;
	printf("%s\n", s);
	delete s;
}

void freeMemory()
{
}

void drawMemory(float dt)
{
}

void keyMemory(iKeyStat stat, iPoint point)
{
}

