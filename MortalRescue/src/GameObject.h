#pragma once
#include <array>
#include <map>
#include <memory>
#include <SDL.h>
#include <string>

#include <Box2D/Box2D.h>
#include "TextureManager.h"


//Forward declarations
class GameObjectDefinition;
class Texture;
class TextObject;
class Animation;

/*
Game Object Types
*/
enum GameObjectType {

	GAME_OBJECT = 0,
	WORLD_OBJECT = 1,    //majority of game objects - any physics object
	PLAYER_OBJECT = 2,
	TEXT_OBJECT = 3

};

static const short CHILD_POSITIONS = 9;

class GameObject
{
private:

	std::string	
		m_definitionId,
		m_currentAnimationState;
	int
		m_mouseState;
	float
		m_angle;
	b2Vec2
		m_position,
		m_size;
	SDL_Color
		m_color;
	bool
		m_removeFromWorld;


	void init();
	virtual void onMouseHoverRender();
	virtual void onMouseClickRender();
	virtual void onMouseHoldRender();
	virtual void onMouseClickEvent();
	void updateMouseState();
	b2Vec2 calcChildPosition(std::shared_ptr<GameObject>, int, int, int);
	void updateChildObjects();
	void buildChildren();


protected:
	virtual SDL_Rect  getPositionRect();
	b2Vec2 matchParentRotation(SDL_Rect, SDL_Rect, float);
	void renderChildObjects();
	virtual SDL_Rect* getRenderTextureRect();
	virtual SDL_Texture* getRenderTexture();
	void setCurrentAnimationState(std::string animationState) {	m_currentAnimationState = animationState; }

public:


	Texture* texture;
	GameObjectDefinition* definition;
	std::map<std::string, Animation*> animations;


	enum GameObjectMouseState {

		MOUSE_NONE = 0,
		MOUSE_HOVER = 1,
		MOUSE_HOLD = 2,
		MOUSE_CLICKED = 3

	};

	GameObject();
	GameObject(std::string,float,float,float);
	virtual ~GameObject();

	virtual void update();
	virtual void render();

	virtual SDL_Rect  getRenderDestRect();
	virtual void setPosition(b2Vec2 position);
	virtual void setPosition(float xPosition, float yPosition);
	virtual void setPosition(b2Vec2 position, float angle);
	virtual void setPosition(float xPosition, float yPosition, float angle);
	virtual void setSize(b2Vec2 size);
	virtual void setSize(float xSize, float ySize);
	virtual void setAngle(float angle);
	void setColor(SDL_Color color) { m_color = color; }
	void setColorAlpha(int alpha) { m_color.a = alpha; }
	void setColor(int red, int green, int blue, int alpha);
	void setRemoveFromWorld(bool removeFromWorld) { m_removeFromWorld = removeFromWorld; }


	//Accessor Functions
	b2Vec2 size() { return m_size; }
	b2Vec2 position() {	return m_position; }
	float angle() { return m_angle; }
	SDL_Color color() { return m_color; }
	std::string definitionId() { return m_definitionId; }
	std::string currentAnimationState() { return m_currentAnimationState; }
	bool removeFromWorld() { return m_removeFromWorld; }

	/*
	Array that has 9 buckets and each bucket has a vector of GameObjects
	*/
	std::array<std::vector<std::shared_ptr<GameObject>>, CHILD_POSITIONS> childObjects;
	
};



