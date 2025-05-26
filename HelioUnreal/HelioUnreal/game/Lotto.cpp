#include "Lotto.h"

#include <stdio.h>
#include <time.h>
#include "Tripple.h"

Button* btn;
Logic** logic;
Result* result;

int* answer;

int gachaIndex, gachaNum;

void loadLotto()
{
	btn = new Button(iPointMake(10, 50));
	logic = new Logic * [5];
	for (int i = 0; i < 5; i++)
		logic[i] = new Logic(iPointMake(10, 100 + 40 * i));
	result = new Result(iPointMake(10, 300));

	answer = new int[6];
	for (int i = 0; i < 6; i++)
		answer[i] = 1 + i;
	
	srand(time(NULL));
}
void freeLotto()
{
	delete btn;
	for (int i = 0; i < 5; i++)
		delete logic[i];
	delete logic;
	delete result;

	delete answer;
}

void drawLotto(float dt)
{
	btn->paint(dt);
	for (int i = 0; i < 5; i++)
		logic[i]->paint(dt);
	result->paint(dt);
}

void cbComplete()
{
	printf("all complete....\n");

	// initialize.....
}

void keyLotto(iKeyStat stat, iPoint point)
{
	if (stat == iKeyStatBegan)
	{
		if (btn->selected)
		{
			gachaNum = 1 + rand() % 5; // 1 ~ 5
			printf("gachaNum = %d\n", gachaNum);
			gachaIndex = 0;

			// money
			btn->start(Button::cb);
		}
	}
	else if (stat == iKeyStatMoved)
	{
		iRect r1;
		r1.origin = btn->position;
		r1.size = btn->size;
		btn->selected = containPoint(point, r1);
	}
	else if (stat == iKeyStatEnded)
	{

	}
}

Button::Button(iPoint position) : Lotto(position)
{
	size = iSizeMake(130, 35);
	selected = false;
}


Button::~Button()
{
	// to do
}

void Button::cb()
{
	printf("Button::cb\n");
	logic[0]->start(Logic::cb);
}

#define buttonAni 2.0f

Lotto::Lotto(iPoint position)
{
	delta = 0.0f;
	this->position = position;
	method = NULL;
}

Lotto::~Lotto()
{
	// to do
}

void Lotto::start(METHOD_VOID cb)
{
	delta = 0.000001f;
	method = cb;
}


void Button::paint(float dt)
{
	float scale = 1.0f;
	if (delta == 0.0f)
		;
	else if (delta < buttonAni)
	{
		// animation
		scale = delta / buttonAni;// 0 ~ 1
		scale = 1.0f - scale;// 1 ~ 0
		printf("scale = %f\n", scale);

		delta += dt;
		if (delta >= buttonAni)
		{
			if (method)
				method();
		}
	}
	else
		return;
	
	// draw :: scale
	if(selected)
		setRGBA(1, 0, 0, 1);
	else
		setRGBA(0, 0, 1, 1);
	fillRect(position.x, position.y, size.width, size.height);

	setStringSize(30);
	setStringRGBA(1, 1, 1, scale);
	drawString(position.x, position.y, "로또시작");
}


Logic::Logic(iPoint position) : Lotto(position)
{

	for (int i = 0; i < 6; i++)
	{
		number[i] = 0;
		right[i] = false;
	}
		
}

Logic::~Logic()
{
	// to do something...
}

void Logic::cb()
{
	printf("cbLogic\n");
	gachaIndex++;
	if (gachaIndex < gachaNum)
	{
		logic[gachaIndex]->start(Logic::cb);
	}
	else// if (gachaIndex == gachaNum)
	{
		// Result
		result->start(Result::cb);
	}
}
void Logic::paint(float dt)
{
	// 안보임(스타트) > 추첨중 > 수자를 보여줌 > 당첨여부
	if (delta == 0.0f)
	{
		// 안보임
	}
	// 추첨 중
	else if (delta < 2.0f)
	{
		// 추첨 중
		const char* s = NULL;
		if (delta < 0.3f)		s = "추";
		else if (delta < 0.6f)	s = "추첨";
		else if (delta < 0.9f)	s = "추첨중";
		else if (delta < 1.2f)	s = "추첨";
		else if (delta < 1.5f)	s = "추";
		else if (delta < 1.8f)	s = "추첨";
		else					s = "추첨중";

		setStringSize(30);
		setStringRGBA(1, 1, 1, 1);
		drawString(position.x, position.y, s);

		delta += dt;
		if (delta >= 2.0f)
		{
			shuffle();
		}
	}
	// 춧자 한 개씩 노출
	else if (delta < 3.2f) // 2 ~ 3.2 : 1.2 each 0.2
	{		
		for (int i = 0; i < 6; i++)
		{
			float a = 1.0f;
			float t = 2.0f + 0.2f * i;
			if (delta < t)				a = 0.0f;
			else if (delta < t + 0.2f)	a = (delta - t) / 0.2f;

			setStringRGBA(1, 1, 1, a);
			char s[10];
			sprintf(s, "%d", number[i]);
			drawString(position.x + 40 * i, position.y, s);
		}

		delta += dt;
	}
	else
	{		
		// 모든 숫자 노출(당첨 숫자 표시) + 당첨여부
		for (int i = 0; i < 6; i++)
		{
			if (right[i])
				setStringRGBA(0, 0, 1, 1);
			else
				setStringRGBA(1, 1, 1, 1);

			char s[10];
			sprintf(s, "%d", number[i]);
			drawString(position.x + 40 * i, position.y, s);
		}
		setStringRGBA(1, 1, 1, 1);

		char s[20];
		sprintf(s, "당첨여부 %d", rightNum);
		drawString(position.x + 240, position.y, s);

		delta += dt;
		if (delta > 3.7f)
		{
			if (method)
			{
				method();
				method = NULL;
			}
		}
	}
}


void Logic::shuffle()
{
	// 1 ~ 45 사이에 중복되지 않게
	// 6개의 숫자를 랜덤하게 뽑기
	//number[0] = 3;
	//number[1] = 1;
	//number[2] = 6;
	//number[3] = 40;
	//number[4] = 5;
	//number[5] = 29;

	int checkNum[6] = { 0,0,0,0,0,0 };

	for (int i = 0; i < 6; i++)
	{
		number[i] = 1 + rand() % 45; 
		checkNum[i] = number[i]; // 중복 체크를 위한 저장
		for (int j = 0; j < i; j++) // 
		{
			if (number[i] == number[j]) // 중복체크
				i--; // i감소로 다시 반복문 수행하게
		}
	}

	int arr[6] = { 5,18,9,6,22,1 };
	
		
	
	match();
}

void Logic::match()
{
	int n = 0;
	for (int i = 0; i < 6; i++)
	{
		bool exist = false;
		right[i] = false;
		for (int j = 0; j < 6; j++)
		{
			if (number[i] == answer[j])
			{
				right[i] = true;
				n++;
				break;
			}
		}
		number[i];
	}
	
	rightNum = n;
	printf("right number = %d\n", n);
}

Result::Result(iPoint position) : Lotto(position)
{

}

Result::~Result()
{
	// to do something~
}

void Result::cb()
{
	printf("all complete....\n");

	// initialize.....
}
void Result::paint(float dt)
{
	float a = 0.0f;
	if (delta == 0.0f);
	else
	{
		delta += dt;
		if (delta >= 1.5f)
		{
			if(method)
			{
				method();
				method = NULL;
			}
		}
	}
	a = delta / 1.0f;

	setStringSize(30);
	setStringRGBA(1, 1, 1, a);
	drawString(position.x, position.y, "Complete");
	setStringRGBA(1, 1, 1, 1);
}
