#pragma once

#include "iStd.h"

struct Particle
{
	float life, _life;
	iPoint p, v; // position, vector
	float ss, se;// size
	iColor4f cs, ce;
	bool paint(float dt);
#if 0
	float as, ae; // acc of v
	float degree, spin;
#endif
};


struct ParticleSystem
{
	Particle* _particle;
	Particle** particle;
	int particleNum, particleMax;

	float createDt, _createDt;

	float life, lifeVar; // 0.2 ~ 0.3
	// p
	int v, vVar; // v
	float ss, ssVar, se, seVar; // as 0.9~1, ae 0~1
	iColor4f cs, csVar;
	iColor4f ce, ceVar;

#if 0
	float a, aVar;// as 0.9~1,  ae 0~1
	float d, dVar;
	iColor4f cs, csVar;
#endif
	ParticleSystem();
	ParticleSystem(const char* szFormat, ...);
	virtual ~ParticleSystem();

	void paint(float dt, iPoint point);
	void initParticle(Particle* p, iPoint pos);
	void start();
	void stop();

	void save(const char* path);
	void load(const char* path);
};


