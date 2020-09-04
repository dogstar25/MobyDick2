#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include "Component.h"

class TransformComponent;

class PhysicsComponent : public Component
{

public:
	PhysicsComponent() {};
	PhysicsComponent(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust);
	~PhysicsComponent();

	void update() override;
	void applyMovement(float velocity, int direction, int strafeDirection);
	void applyRotation(float angularVelocity);
	void setOffGrid();
	void setTransform(b2Vec2 positionVector, float angle);
	void setLinearVelocity(b2Vec2 velocityVector);
	void setPhysicsBodyActive(bool  active);
	void setFixedRotation(bool fixedRotation);
	void setBullet(bool isBullet);

	void attachItem(GameObject* inventoryObject);

	//Accessor functions
	b2Vec2 objectAnchorPoint() { return m_objectAnchorPoint; }
	b2Vec2 position() { return m_physicsBody->GetPosition(); }
	float angle() { return m_physicsBody->GetAngle(); }

private:

	b2Body* _buildB2Body(Json::Value physicsComponentJSON, Json::Value transformComponentJSON);
	uint16 _setCollisionMask(Json::Value physicsComponentJSON);

	b2Body* m_physicsBody{ nullptr };

	float m_friction{ 0 };
	float m_restitution{ 0 };
	float m_density{ 0 };
	float m_linearDamping{ 0 };
	float m_collisionRadius{ 0 };
	float m_angularDamping{ 0 };

	uint16 m_collisionCategory{ 0 };
	uint16 m_collisionShape{ 0 };
	uint16 m_physicsType{ 0 };

	b2Vec2 m_objectAnchorPoint{ 0 , 0 };

};

#endif