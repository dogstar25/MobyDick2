#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "GameObject.h"
#include <unordered_map>
#include <map>

class WeaponObject;
class GameObjectDefinition;


class WorldObject :	public GameObject
{
public:
	WorldObject();
	WorldObject(std::string, float, float, float);
	~WorldObject();

	void update() override;
	void setPosition(b2Vec2, float) override;
	SDL_FRect getRenderDestRect() override; 
	SDL_FRect getPositionRect() override;
	bool testStrength(int);
	virtual void setBox2DUserData(WorldObject*);
	void setActive(bool);
	void setStrength(float strength) { m_strength = strength; }
	void addWeapon(std::string weaponObjectId);

	
	//Accessor Functions
	b2Body* physicsBody() {
		return m_physicsBody;
	}
	float speed() {
		return m_speed;
	}
	float strength() {
		return m_strength;
	}
	WeaponObject* weapon() {
		return m_weapon;
	}

private:

	b2Body* m_physicsBody;
	float m_speed;
	float m_strength;
	WeaponObject* m_weapon;

	b2Body* buildB2Body(GameObjectDefinition*);
	uint16 setCollisionMask(uint16 category);



};

#endif