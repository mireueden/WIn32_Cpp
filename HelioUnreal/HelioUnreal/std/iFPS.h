#pragma once

class iFPS
{
private:
	iFPS();

public:
	virtual ~iFPS();


	static iFPS* share();
	float update();
	
	
	unsigned long lastUpdate;

};


