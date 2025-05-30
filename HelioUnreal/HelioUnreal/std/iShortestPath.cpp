#include "iShortestPath.h"

#include "iStd.h"

iShortestPath::iShortestPath()
{
	tile = NULL;
	tileX = 0;	tileY = 0;
	tileW = 0;	tileH = 0;
}

iShortestPath::~iShortestPath()
{
	if (tile)
		delete tile;
	if (path)
	{
		int len = tileX * tileY;
		for (int i = 0; i < len; i++)
			delete path[i].path;
		delete path;
		path = NULL;
	}

}

void iShortestPath::clean()
{
	if (tile)
	{
		delete tile;
		tile = NULL;
	}
	if (path)
	{
		int len = tileX * tileY;
		for (int i = 0; i < len; i++)
			delete path[i].path;
		delete path;
		path = NULL;
	}
}

void iShortestPath::set(uint8* tile, int tileX, int tileY, int tileW, int tileH)
{
	int len = tileX * tileY;
	if (this->tileX * this->tileY >= len)
	{
		this->tileW = tileW;
		this->tileH = tileH;
		memcpy(this->tile, tile, sizeof(uint8) * len);
		return;
	}
	clean();

	uint8* t = new uint8[len];
	memcpy(t, tile, sizeof(uint8) * len);
	this->tile = t;
	this->tileX = tileX; this->tileY = tileY;
	this->tileW = tileW; this->tileH = tileH;

	path = new Path[len];
	for (int i = 0; i < len; i++)
		path[i].path = new int[len];
}

// 다익스트라
// 모든 방문지역 1번만 방문 -> 시간 단축 목적
// (진행 방법)
// 모든 방문지 inf(무한값) + Visit=false(방문 X) + pathnum(경로0개) = 0
// 루프(한번에 한곳만 방문) : 더이상 방문할 수 없을때 끝.
// - 방문 X + 이동 0 => 가장 Min값
// 목표지역 정보 = 내가 얻으려는 정보


void iShortestPath::run(iPoint start, iPoint end, iPoint* result, int& resultNum)
{
	int x = (int)start.x / tileW;
	int y = (int)start.y / tileH;
	int s = tileX * y + x;

	x = (int)end.x / tileW;
	y = (int)end.y / tileH;
	int e = tileX * y + x;

	int len = tileX * tileY;
	//Path* path = new Path[len];
	for (int i = 0; i < len; i++)
	{
		Path* p = &path[i];
		p->visit = false;
		p->value = Inf;
		//p->path = new int[len];
		p->pathNum = 0;
	}
	path[s].value = 0;// 출발점 값 0
	tile[s] = 0;// CX

	// 루프(방문하지 않고 + 값이 가장 작은 값 부터 방문)
	for (;;)
	{
		int index = -1;// 방문할 곳 index
		int min = Inf;
		for (int i = 0; i < len; i++)
		{
			Path* p = &path[i];
			if (p->visit ||
				tile[i] == CX) continue;

			if (min > p->value)
			{
				min = p->value;
				index = i;
			}
		}
		if (index == -1)// 더 이상 방문할 곳이 없네??
			break;
		Path* curr = &path[index];
		curr->visit = true;
		curr->path[curr->pathNum] = index;
		curr->pathNum++;

		for (int i = 0; i < 4; i++)
		{
			int idx;
			if (i == 0)// left
			{
				if (index % tileX == 0) continue;
				idx = index - 1;
			}
			else if (i == 1)// right
			{
				if (index % tileX == tileX - 1) continue;
				idx = index + 1;
			}
			else if (i == 2)// up
			{
				if (index / tileX == 0) continue;
				idx = index - tileX;
			}
			else// if (i == 3)// down
			{
				if (index / tileX == tileY - 1) continue;
				idx = index + tileX;
			}
			if (tile[idx] == CX) continue;

			Path* p = NULL;
			p = &path[idx];
			if (p->value < curr->value) continue;

			p->value = curr->value + tile[idx];
			memcpy(p->path, curr->path, sizeof(int) * curr->pathNum);
			p->pathNum = curr->pathNum;
		}
	}

	Path* p = &path[e];// 결과
	//remove(p->path, p->pathNum);

	int j = p->pathNum - 1;
	for (int i = 1; i < j; i++)
	{
		int x = p->path[i] % tileX;
		int y = p->path[i] / tileX;
		result[i] = iPointMake(	tileW * x + tileW / 2,
								tileH * y + tileH / 2);
	}
	result[0] = start;
	result[j] = end;
	resultNum = p->pathNum;
}

void iShortestPath::remove(int* path, int& pathNum)
{
#if 1
	// 대각선
	// 경로중 좌,우 or 상,하 둘 주 하나씩 들고 있는 경우 자신 제거
	int n = pathNum - 2;
	for (int i = 0; i < n; i++)
	{
		int cx = path[i] % tileX;
		int cy = path[i] / tileX;
		int nx = path[i + 2] % tileX;
		int ny = path[i + 2] / tileX;
		if (abs(cx - nx) == 1 && abs(cy - ny) == 1)
		{
			// i+1이 되어야하는 이유 : 자기 자신은 제외해야함.
			//int len = (p->pathNum - 1) - (i + 2) + 1; // a = i + 2, b = p->pathNum - 1
			//int len = p->pathNum - 1 - i - 2 + 1; // a = i + 2, b = p->pathNum - 1
			// a- b : b - a + 1
			int len = pathNum - 1 - i - 2 + 1; // a = i + 2, b = p->pathNum - 1
			memcpy(&path[i + 1], &path[i + 2], sizeof(int) * len);
			//p->pathNum -= 2;
			pathNum--;
			//n -= 2;
			n--;
			//i--;
		}
	}

	// 직선 사이에 필요없는 방문지 정리
	for (int i = 0; i < n; i++)
	{
		int cx = path[i] % tileX;
		int cy = path[i] / tileX;
		int nx = path[i + 1] % tileX;
		int ny = path[i + 1] / tileX;

		bool sameX = (cx == nx); // x좌표가 같다는 것은 y방향으로 움직였다는 것

		if (sameX == false)
		{
			if (cy != ny) // 대각선까지 중복되는 걸 제거하고 싶지만, 시간 관계상 패스
				continue;
		}

		int j;
		for (j = i + 2; j < pathNum; j++)
		{
			if (sameX) // x 좌표가 같은 경우 y 방향으로 이동
			{
				if (cx != path[j] % tileX) // x좌표의 변동 == 방향 변경
					break;
			}
			else // x 좌표가 다른 겨우 x 방향으로 이동
			{
				if (cy != path[j] % tileX) // y좌표의 변동 == 방향 변경
					break;
			}

			// 제거 대상이 i+1 ~ j-1
			// j, pathNum-1 사이의 개수
			// (pathNum - 1) - (j) + 1
			int len = pathNum - 1 - j + 1;
			// 틀어진 위치 이전까지 직선으로 이어진 상태
			memcpy(&path[i + 1], &path[j], sizeof(int) * len);


		}
	}



#elif 1 // 직선 사이에 필요없는 방문지 정리
	// 대각선이 등장하는 위치부터 출발지 사이의 출력을 제거

	// 현재 위치 i의 다음 위치인 i+1위치의 노드를 제거하기 위해서는
	// i+2가 존재해야함.

	
	int newS = 0;
	int n = p->pathNum - 2;
	for (int i = 0; i < n; i++)
	{
		int cx = p->path[i] % tileX;
		int cy = p->path[i] / tileX;

		int nx = p->path[i + 2] % tileX;
		int ny = p->path[i + 2] / tileX;

		int sx = p->path[newS] / tileX;
		int sy = p->path[newS] / tileX;
		if (abs(cx - nx) == 1 && abs(cy - ny) == 1)
		{

			// n+1이 대각선 노드임.
			// s 위치부터 ~ 대각선 노드 사이의 모든 노드 덮어버리기
			// a ~ b : b - a + 1
			int len = s - i - 1 + 1;

			memcpy(&p->path[newS + 1], &p->path[i + 1], sizeof(int) * len);
			//p->pathNum -= 2;
			p->pathNum -= i;
			n--;

			// 대각선 위치가 새로운 S
			newS = p->path[i];
		}
		else if (abs(sx - cx) > 2 && abs(sy - cy))
		{

		}


	}

#endif

}





#if 0


bool runSP(uint8* tile, int tileX, int tileY, int s, int e, int* path, int& pathNum)
{
	path[0] = s;
	path[1] = e;
	pathNum = 2;

	return true;
}


bool runSP(uint8* tile, int tileX, int tileY, int s, int e,
	int* result, int& resultNum)
{
	int len = tileX * tileY;
	Path* path = new Path[len];
	for (int i = 0; i < len; i++)
	{
		Path* p = &path[i];
		p->visit = false;
		p->value = Inf;
		p->path = new int[len];
		p->pathNum = 0;
	}
	path[s].value = 0;// 출발점 값 0
	tile[s] = 0;// CX

	// 루프(방문하지 않고 + 값이 가장 작은 값 부터 방문)
	for (;;)
	{
		int index = -1;// 방문할 곳 index
		int min = Inf;
		for (int i = 0; i < len; i++)
		{
			Path* p = &path[i];
			if (p->visit ||
				tile[i] == CX) continue;

			if (min > p->value)
			{
				min = p->value;
				index = i;
			}
		}
		if (index == -1)// 더 이상 방문할 곳이 없네??
			break;
		Path* curr = &path[index];
		curr->visit = true;
		curr->path[curr->pathNum] = index;
		curr->pathNum++;

		for (int i = 0; i < 4; i++)
		{
			int idx;
			if (i == 0)// left
			{
				if (index % tileX == 0) continue;
				idx = index - 1;
			}
			else if (i == 1)// right
			{
				if (index % tileX == tileX - 1) continue;
				idx = index + 1;
			}
			else if (i == 2)// up
			{
				if (index / tileX == 0) continue;
				idx = index - tileX;
			}
			else// if (i == 3)// down
			{
				if (index / tileX == tileY - 1) continue;
				idx = index + tileX;
			}
			if (tile[idx] == CX) continue;
			Path* p = NULL;
			p = &path[idx];
			if (p->value < curr->value) continue;

			p->value = curr->value + tile[idx];
			memcpy(p->path, curr->path, sizeof(int) * curr->pathNum);
			p->pathNum = curr->pathNum;
		}
	}

	Path* p = &path[e];// 결과
	memcpy(result, p->path, sizeof(int) * p->pathNum);
	resultNum = p->pathNum;

	for (int i = 0; i < len; i++)
		delete path[i].path;
	delete path;

	return true;
}
#endif 
