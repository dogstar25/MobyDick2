#ifndef PARTICLE_MACHINE_H
#define PARTICLE_MACHINE_H

#include <vector>
#include <SDL2/SDL_image.h>

#include "ParticleEmission.h"

class Scene;

class ParticleMachine
{
public:
	ParticleMachine(Scene* scene);
	~ParticleMachine();

	void update();
	
	void runParticleEmissions();
	void add(ParticleEmission*);
	void fireBullet(
		std::string poolId,
		b2Vec2 origin,
		float angle,
		int force
	);

	void emit(
		std::string poolId,
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

private:

	std::vector <ParticleEmission*> m_particleEmissions;
	//Reference pointer to parent Scene
	Scene* m_parentScene; //memory owned by SceneManager




};

#endif