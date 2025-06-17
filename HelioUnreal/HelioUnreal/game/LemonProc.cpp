#include "LemonProc.h"

Lemon* lemon;// 17 x 10
Texture** texLemonBg;
Texture** texLemonNum;

// =============================================
// Proc(17 x 10 타일로 1 ~ 9 숫자로 구성)
// =============================================

void loadLemonProc()
{
	// proc 리소스 로딩
	srand(time(NULL));

	lemon = new Lemon[170];// 17 x 10
	for (int i = 0; i < 170; i++)
	{
		Lemon* l = &lemon[i];
		l->num = 1 + rand() % 9;
		l->exist = (rand() % 100 < 7);
		l->selected = false;
	}

	// 640 x 480 17
	texLemonBg = new Texture * [2];
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(30, 30);
	for (int i = 0; i < 2; i++)
	{
		g->init(size.width, size.height);

		setRGBA(1, 1, 1, 1);
		g->fillRect(0, 0, size.width, size.height);
		if (i == 0)
		{
			setLineWidth(2);
			setRGBA(0, 0, 0, 1);
			g->drawRect(2, 2, size.width - 4, size.height - 4);
		}
		else
		{
			setRGBA(0, 1, 0, 0.5f);
			g->fillRect(0, 0, size.width, size.height);
			setRGBA(0, 1, 0, 1);
			g->drawRect(0, 0, size.width, size.height);
		}

		texLemonBg[i] = g->getTexture();
		g->clean();
	}

	texLemonNum = new Texture * [11];
	setStringSize(20);
	setStringRGBA(0, 0, 0, 1);
	for (int i = 0; i < 10; i++)
	{
		g->init(size.width, size.height);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%d", i);
		texLemonNum[i] = g->getTexture();
		g->clean();
	}
	g->init(size.width, size.height);
	texLemonNum[10] = g->getTexture();
	g->clean();

	loadLemonProcUI();
	loadLemonSetting();
	loadLemonCountDown();
}

void freeLemonProc()
{
	// proc 리소스 해제
	delete lemon;
	for (int i = 0; i < 2; i++)
		delete texLemonBg[i];
	delete texLemonBg;
	for (int i = 0; i < 11; i++)
		delete texLemonNum[i];
	delete texLemonNum;

	freeLemonProcUI();
	freeLemonSetting();
	freeLemonCountDown();
}


void drawLemonProc(float dt)
{
	setRGBA(0, 0, 0, 1);
	clear();
	setRGBA(1, 1, 1, 1);

	// proc
	for (int i = 0; i < 170; i++)
	{
		// type No.0 (Choice)
		// 사각형(선택o, 선택x, 없음) + 숫자
		// 그리는 난이도는 높음
		// 그릴때마다 사용하는 메모리 적음
		
		// type No.1
		// iStrTex
		// 그리는 난이도는 낮음
		// 그릴때마다 생성함으로써 메모리 과다사용
		int x = LEMON_SX + 30 * (i % 17);
		int y = LEMON_SY + 30 * (i / 17);
		Lemon* l = &lemon[i];
		if (l->num < 10)
		{
			drawImage(texLemonBg[l->selected], x, y, TOP | LEFT);
			if (l->exist)
			{
				setRGBA(1, 1, 0, 1);
				fillRect(x + 10, y + 10, 10, 10);
				setRGBA(1, 1, 1, 1);
			}
			drawImage(texLemonNum[l->num], x, y, TOP | LEFT);
		}
		else// if (l->num == 10)
		{
			drawImage(texLemonBg[0], x, y, TOP | LEFT);
			drawImage(texLemonNum[10], x, y, TOP | LEFT);
		}

	}

	drawLemonProcUI(dt);
	drawLemonSetting(dt);
	drawLemonCountDown(dt);
}

bool dragLemon = false;
int sx = -1, sy = -1;
void keyLemonProc(iKeyStat stat, iPoint point)
{
	if (keyLemonSetting(stat, point) ||
		keyLemonCountDown(stat, point))
		return;

	// proc
	switch (stat) {
	case iKeyStatBegan:
	{
		int x = point.x - LEMON_SX;
		x /= LEMON_W;
		int y = point.y - LEMON_SY;
		y /= LEMON_H;

		Lemon* l = &lemon[17 * y + x];
		l->selected = true;

		dragLemon = true;
		sx = x;
		sy = y;
	}
	break;

	case iKeyStatMoved:
	if (dragLemon)
	{
		int x = point.x - LEMON_SX;
		x /= LEMON_W;
		int y = point.y - LEMON_SY;
		y /= LEMON_H;
#if 1
		if (x < 0) x = 0;
		else if (x > 16) x = 16;

		if (y < 0) y = 0;
		else if (y > 9) y = 9;
#endif
		if (x == sx && y == sy)
		{
			for (int i = 0; i < 170; i++)
			{
				Lemon* l = &lemon[i];
				l->selected = false;
			}
			Lemon* l = &lemon[17 * sy + sx];
			l->selected = true;
		}
		if (x != sx || y != sy) 
		{
			// 전체 선택해제
			for (int i = 0; i < 170; i++)
			{
				Lemon* l = &lemon[i];
				l->selected = false;
			}

			// 모두 선택해야함.
			int cx = x < sx ? x : sx;
			int cy = y < sy ? y : sy;
			int nx = cx + abs(x - sx) + 1;
			int ny = cy + abs(y - sy) + 1;
			for (int j = cy; j < ny; j++)
			{
				for (int i = cx; i < nx; i++)
				{
					Lemon* l = &lemon[17 * j + i];
					l->selected = true;
				}
			}

		}
	}
	break;

	case iKeyStatEnded:
	if (dragLemon)
	{
		dragLemon = false;

		int sum = 0;
		for (int i = 0; i < 170; i++)
		{
			Lemon* l = &lemon[i];
			if (l->selected == false ||
				l->num == 10) continue;
			sum += l->num;
		}
		if (sum == 10)
		{
			int n = 0;
			// 선택된걸 모두 삭제
			for (int i = 0; i < 170; i++)
			{
				Lemon* l = &lemon[i];
				if (l->selected == false) continue;
				l->selected = false;
				if (l->num == 10) continue;
				l->num = 10;// 삭제
				n += 1 + 5 * l->exist;
			}
			//score += n;
			score->add(n);
		}
		else
		{
			for (int i = 0; i < 170; i++)
			{
				Lemon* l = &lemon[i];
				l->selected = false;
			}
		}

	}
		break;
	}
}


// =============================================
// ProcUI(위 : 점수&남은시간, 아래 : 현재 점수)
// =============================================
Number* score;
float takeTime;

iStrTex** stProcUI;
Texture* methodStUI(const char* s);

void loadLemonProcUI()
{
	score = new Number();
	takeTime = 0.0f;

	stProcUI = new iStrTex * [2];
	for (int i = 0; i < 2; i++)
		stProcUI[i] = new iStrTex(methodStUI);

}

void freeLemonProcUI()
{
	delete score;
	for (int i = 0; i < 2; i++)
		delete stProcUI[i];
	delete stProcUI;
}

Texture* methodStUI(const char* s)
{
	int lineNum;
	char** line = iString::split(lineNum, s);
	int index = atoi(line[0]);

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(100, 80);
	g->init(size.width, size.height);

	setRGBA(1, 1, 1, 1);
	g->fillRect(0, 0, size.width, size.height);
	setRGBA(0, 0, 0, 1);
	g->drawRect(5, 5, size.width - 10, size.height - 10);
	setRGBA(1, 1, 1, 1);


	setStringSize(25);
	if (index == 0)
	{
		int score = atoi(line[1]);
		setStringRGBA(0, 0, 0, 1);
		g->drawString(size.width / 2, 8, TOP | HCENTER, "점수");
		g->drawString(size.width / 2, 38, TOP | HCENTER, "%d", score);
	}
	else
	{
		int t = atoi(line[1]);
		setStringRGBA(0, 0, 0, 1);
		g->drawString(size.width / 2, 8, TOP | HCENTER, "남은시간");
		g->drawString(size.width / 2, 38, TOP | HCENTER, "%d:%d", t / 60, t % 60);
	}
	iString::free(line, lineNum);

	Texture* tex = g->getTexture();
	g->clean();
	return tex;
}
void drawLemonProcUI(float dt)
{
	score->update(dt);
	stProcUI[0]->paint(10, 10, TOP | LEFT, "0\n%d", score->get());


	if (numCountDown == 0)
		takeTime += dt;
	float t = 120 - takeTime;
	if (t < 0)
		t = 0;
	stProcUI[1]->paint(110, 10, TOP | LEFT, "1\n%.0f", t);
}

Number::Number()
{
	curr = 0;
	next = 0;
	_delta = 0.5f;
	delta = _delta;
}

Number::~Number() {}

void Number::update(float dt)
{
	if (curr == next)
		return;

	delta += dt;
	if (delta >= _delta)
	{
		delta = _delta;
		curr = next;
	}
}

int Number::get()
{
	return linear(curr, next, delta / _delta);
}

void Number::add(int n)
{
	curr = get();
	next += n;
	delta = 0.0f;
}


// =============================================
// Setting
// =============================================

void loadLemonSetting()
{
}

void freeLemonSetting()
{
}

void drawLemonSetting(float dt)
{
}

bool keyLemonSetting(iKeyStat stat, iPoint point)
{
	return false;
}

// =============================================
// CountDown
// =============================================
iStrTex* stCountDown;
int numCountDown;
float deltaCountDown; // 0 ~ 1흐르게 
// 3,2,1로 나와야함. 3 > 2,9x... 1초가 흐르기전 숫자 변경우려

void loadLemonCountDown()
{
	stCountDown = new iStrTex();
	numCountDown = 3;
	deltaCountDown = 0.0f;
}

void freeLemonCountDown()
{
	delete stCountDown;
}

void drawLemonCountDown(float dt)
{
	if (numCountDown == 0)
		return;

	setStringSize(100);
	setStringRGBA(0, 1, 0, 1);
	
	float rate;
	if (deltaCountDown < 0.15f)
		rate = 1.0f;
	else if (deltaCountDown < 0.85f)// 0.15 ~ 0.85
		rate = 1.0f + 0.2f * sin(deg2rad((deltaCountDown - 0.15) / 0.7f * 180));// 0 ~ 1
	else
		rate = 1.0f;
	stCountDown->set("%d", numCountDown);
	stCountDown->paint(devSize.width / 2, devSize.height / 2, VCENTER | HCENTER, rate, rate, 2, 0);

	deltaCountDown += dt;
	while (deltaCountDown >= 1.0f)
	{
		deltaCountDown -= 1.0f;
		numCountDown--;
	}
}

bool keyLemonCountDown(iKeyStat stat, iPoint point)
{
	if (numCountDown == 0)
		return false;
	return true;
}

