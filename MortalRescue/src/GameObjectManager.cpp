#include "GameObjectManager.h"
#include "TextureManager.h"
#include "GameObjectDefinition.h"
#include "game.h"

bool GameObjectManager::init()
{

	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/gameObjectAssets.json");
	ifs >> root;

	string id, textureId;
	GameObjectDefinition* gameObjectDefinition;

	for (auto itr : root["gameObjects"])
	{
		gameObjectDefinition = new GameObjectDefinition();
		gameObjectDefinition->id = itr["id"].asString();
		gameObjectDefinition->description = itr["description"].asString();
		gameObjectDefinition->playerSpeed = itr["playerSpeed"].asDouble();
		gameObjectDefinition->xSize = itr["xSize"].asFloat();
		gameObjectDefinition->ySize = itr["ySize"].asFloat();
		gameObjectDefinition->initPosX = itr["initPosX"].asInt();
		gameObjectDefinition->initPosY = itr["initPosY"].asInt();
		gameObjectDefinition->texture = itr["texture"].asString();

		//If this has a textture then get and store it
		if (itr["primativeShape"].isNull() == false)
		{
			gameObjectDefinition->isPrimitiveShape = true;

			//color
			if (itr["primativeColor"]["random"].asBool() == true)
			{
				gameObjectDefinition->primativeColor = Game::util.generateRandomColor();
			}
			else
			{
				gameObjectDefinition->primativeColor.r = itr["primativeColor"]["red"].asInt();
				gameObjectDefinition->primativeColor.g = itr["primativeColor"]["green"].asInt();
				gameObjectDefinition->primativeColor.b = itr["primativeColor"]["blue"].asInt();
				gameObjectDefinition->primativeColor.a = itr["primativeColor"]["alpha"].asInt();
			}
		}

		//If this is a physics object then build the box2d body
		if (itr["physicsObject"].isNull() == false)
		{
			gameObjectDefinition->isPhysicsObject = true;
			gameObjectDefinition->friction = itr["physicsObject"]["friction"].asFloat();
			gameObjectDefinition->density = itr["physicsObject"]["density"].asFloat();
			gameObjectDefinition->linearDamping = itr["physicsObject"]["linearDamping"].asFloat();
			gameObjectDefinition->angularDamping = itr["physicsObject"]["angularDamping"].asFloat();
			gameObjectDefinition->physicsType = itr["physicsObject"]["type"].asString();

		}

		//Store Animations
		if (itr["animations"].isNull() == false)
		{
			gameObjectDefinition->isAnimated = true;
			GameObjectAnimation* animation;
			for (auto animItr : itr["animations"])
			{
				string texture = animItr["texture"].asString();
				string id = animItr["id"].asString();
				int frames = animItr["frames"].asInt();
				float speed = animItr["speed"].asFloat();
				animation = buildAnimation(gameObjectDefinition, id, texture, frames, speed);
				gameObjectDefinition->animations[id] = *animation;
			}
		}
		
		this->gameObjectDefinitions[gameObjectDefinition->id] = *gameObjectDefinition;

	}

	return true;

}

GameObject* GameObjectManager::buildGameObject(string gameObjectId, b2World* physicsWorld)
{

	GameObject* gameObject=nullptr;
	GameObjectDefinition* gameObjectDefinition;

	gameObject = new GameObject();
	gameObjectDefinition = &this->gameObjectDefinitions[gameObjectId];

	gameObject->definition = *gameObjectDefinition;
	gameObject->staticTexture = Game::textureManager.getTexture(gameObjectDefinition->texture);

	if (gameObjectDefinition->isPhysicsObject == true)
	{
		gameObject->physicsBody = buildB2Body(gameObjectDefinition, physicsWorld);
	}

	//build the animation objects
	for (auto & gameObjectDefinitionAnimation : gameObjectDefinition->animations) {
		
		gameObject->animations[gameObjectDefinitionAnimation.second.id] = gameObjectDefinitionAnimation.second;
		
	}


	return gameObject;

}

GameObjectAnimation* GameObjectManager::buildAnimation(GameObjectDefinition* gameObjectDefinition,
														string id, string textureId, int frames,
														float speed)
{

	GameObjectAnimation* animation=nullptr;
	animation = new GameObjectAnimation();

	animation->id = id;
	animation->frameCount = frames;
	animation->speed = speed;

	//Get pointer to textture
	animation->texture = Game::textureManager.getTexture(textureId);

	//Calculate how many columns and rows this animation texture has
	int width, height;
	//First get width of textture
	SDL_QueryTexture(animation->texture, NULL, NULL, &width, &height);

	//Calculate nnumber of rows and columns - remember to convert the gameObject size to pixels first
	int rows, columns;
	columns = width / (gameObjectDefinition->xSize * Game::config.scaleFactor);
	rows = height / (gameObjectDefinition->ySize * Game::config.scaleFactor);

	//Calculate top left corner of each animation frame
	SDL_Point point;
	int frameCount = 0;
	for (int rowIdx = 0; rowIdx < rows; rowIdx++) {
		for (int colIdx = 0; colIdx < columns; colIdx++) {

			point.x = colIdx * (gameObjectDefinition->xSize * Game::config.scaleFactor);
			point.y = rowIdx * (gameObjectDefinition->ySize * Game::config.scaleFactor);
			//animation->animationFramePositions[frameCount] = point;
			animation->animationFramePositions.push_back(point);

			//do not exceed the maximum number of frames that this texture holds
			frameCount++;
			if (frameCount >= animation->frameCount) {
				break;
			}
		}
	}
	
	//DRUNK: Initialze the current source rect to the first animation frame
	//SDL_Rect* sourceRect = nullptr;
	//animation->currentTextureAnimationSrcRect

	return animation;



}


b2Body * GameObjectManager::buildB2Body(GameObjectDefinition* gameObjectDefinition, b2World* physicsWorld)
{
	b2BodyDef bodyDef;

	if(gameObjectDefinition->physicsType.compare("B2_STATIC") == 0)
	{
		bodyDef.type = b2_staticBody;
	}
	else if (gameObjectDefinition->physicsType.compare("B2_KINEMATIC") == 0)
	{
		bodyDef.type = b2_kinematicBody;
	}
	else if (gameObjectDefinition->physicsType.compare("B2_DYNAMIC") == 0)
	{
		bodyDef.type = b2_dynamicBody;
	}

	bodyDef.position.Set(gameObjectDefinition->initPosX , gameObjectDefinition->initPosY);
	b2Body* body = physicsWorld->CreateBody(&bodyDef);

	b2PolygonShape box;
	float32 xSize = gameObjectDefinition->xSize / 2; //SetAsBox takes half-widths
	float32 YSize = gameObjectDefinition->ySize / 2;

	box.SetAsBox(xSize, YSize);
	std::cout << "BuildBox size for " << gameObjectDefinition->id << " was " << xSize << "\n";

	// Define the body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = gameObjectDefinition->density;

	// Override the default friction.
	fixtureDef.friction = gameObjectDefinition->friction;
	fixtureDef.restitution = .2;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	body->SetLinearDamping(gameObjectDefinition->linearDamping);
	body->SetAngularDamping(gameObjectDefinition->angularDamping);
	physicsWorld->SetAutoClearForces(true);
	
	return body;

}


GameObjectDefinition * GameObjectManager::getGameObjectDefinition(string id)
{
	return &this->gameObjectDefinitions[id];
}


