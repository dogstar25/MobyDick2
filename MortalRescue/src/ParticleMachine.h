#pragma once
#include <vector>
#include <SDL_image.h>

#include "ParticleEmission.h"

using namespace std;

class ParticleMachine
{
public:
	ParticleMachine();
	~ParticleMachine();
	void update();
	
	void runParticleEmissions();
	void add(ParticleEmission*);
	void fireBullet(
		string poolId,
		b2Vec2 origin,
		float angle,
		int force
	);

	vector <ParticleEmission*> particleEmissions;

	void emit(
		string poolId,
		b2Vec2 originMin,
		b2Vec2 originMax,
		int forceMin,
		int forceMax,
		float lifetimeMin,
		float lifetimeMax,
		bool alphaFade,
		float angleMin,
		float angleMax,
		float particleSizeMin,
		float particleSizeMax,
		SDL_Color colorRangeBegin,
		SDL_Color colorRangeEnd,
		int particleSpawnCountMin,
		int particleSpawnCountMax
	);

};

