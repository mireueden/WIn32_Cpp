#pragma once

class iFPS
{
private:
	iFPS();

public:
	virtual ~iFPS();


	static iFPS* share();
	float update();
	int framePerSec();
	
	unsigned long lastUpdate;
	int count;
	float takeTime;
};
#define fpsInterval 5.0f

