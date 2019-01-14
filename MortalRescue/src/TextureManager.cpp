#include "TextureManager.h"
#include "game.h"
#include <math.h>
#include <list>

bool TextureManager::init(SDL_Window* pWindow)
{

	//Create the main renderer
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

	//SDL_RenderSetScale(pRenderer, (1/Game::config.scaleFactor), (1/Game::config.scaleFactor));
	//SDL_RenderSetScale(pRenderer, 25 ,25);

	//Load all of the textures for the game
	loadTextures();

	return true;
}

bool TextureManager::render(GameObject* gameObject)
{
	SDL_Rect srcRect, destRect;

	destRect.w = (gameObject->definition.xSize * Game::config.scaleFactor);
	destRect.h = (gameObject->definition.ySize * Game::config.scaleFactor);

	destRect.x = (gameObject->physicsBody->GetPosition().x *  Game::config.scaleFactor) - (destRect.w /2) ;
	destRect.y = (gameObject->physicsBody->GetPosition().y *  Game::config.scaleFactor) - (destRect.h /2) ;

	//If this is a primitive shape object just drawa a rectangle
	if (gameObject->definition.isPrimitiveShape == true)
	{
		SDL_SetRenderDrawColor(pRenderer, 
			gameObject->definition.primativeColor.r,
			gameObject->definition.primativeColor.g,
			gameObject->definition.primativeColor.b,
			gameObject->definition.primativeColor.a);
		SDL_RenderFillRect(pRenderer, &destRect);
		//drawPoly(gameObject->physicsBody);
	}
	else
	{
		//TODO:if this is the player then do not use the box2d angle. Use our own controller by keyboard/mouse
		float angle = gameObject->physicsBody->GetAngle();
		angle = angle * 180 / M_PI;

		//If this is animated object then get its current animation frame texture, 
		// otherwise get its static texture
		SDL_Texture* texure=NULL;
		SDL_Rect *textureSourceRect = NULL;
		if (gameObject->definition.isAnimated) {

			texure = gameObject->animations[gameObject->currentAnimationState].texture;
			textureSourceRect = &gameObject->animations[gameObject->currentAnimationState].currentTextureAnimationSrcRect;
		}
		else {

			texure = gameObject->staticTexture;
		}

		SDL_RenderCopyEx(pRenderer, texure, textureSourceRect, &destRect, angle,
			NULL, SDL_FLIP_NONE);

	}

	//std::cout << "Dest X is " << destRect.x << " \n";

	return true;
}

bool TextureManager::render(b2Body* body)
{
	SDL_Rect srcRect, destRect;

	//SDL_RenderClear(pRenderer);
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 0);
	destRect.x = round(body->GetPosition().x);
	destRect.y = round(body->GetPosition().y);
	destRect.w = 5;
	destRect.h = 5;
	SDL_RenderFillRect(pRenderer, &destRect);
	//SDL_RenderPresent(pRenderer);
	//SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

	return true;
}

bool TextureManager::loadTextures()
{

	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/textureAssets.json");
	ifs >> root;

	//Get and store config values
	string filename, id;
	SDL_Surface* tempSurface;
	SDL_Texture* texture;

	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for(auto itr : root["textures"])
	{
		id = itr["id"].asString();
		filename = itr["filename"].asString();

		tempSurface = IMG_Load(filename.c_str());
		texture = SDL_CreateTextureFromSurface(this->pRenderer, tempSurface);
		SDL_FreeSurface(tempSurface);

		//this->textureMap.insert(pair<string, SDL_Texture*>(id, texture));
		this->textureMap[id]= texture;

	}
	

	return true;
}

SDL_Texture * TextureManager::getTexture(string id)
{
	SDL_Texture *tempTexture= nullptr;

	tempTexture = this->textureMap[id];

	return tempTexture;
}

bool TextureManager::present()
{
	SDL_RenderPresent(pRenderer);
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

	return true;
}

bool TextureManager::clear()
{
	SDL_RenderClear(pRenderer);

	return true;
}

void TextureManager::drawPoly(b2Body* body)
{


	b2Fixture* fixture = body->GetFixtureList();
	while (fixture != NULL)
	{
		
		b2Shape* s = fixture->GetShape();
		b2PolygonShape* shape = (b2PolygonShape*)s;

		SDL_Point *points = new SDL_Point[shape->m_count+1];

		b2Vec2 firstVector;
		bool firstFound = false;
		SDL_Point point;
		// Build list of transformed vertices
		for (int i = 0; i < shape->m_count; ++i) {

			b2Vec2 vector = shape->m_vertices[i];
			//If this is the first vector, then save it
			if (firstFound == false) {
				firstVector = vector;
				firstFound = true;
			}

			point.x = vector.x * Game::config.scaleFactor;
			point.y = vector.y * Game::config.scaleFactor;
			points[i] = point;
		}

		//Add the first point to the end to complete closed shape
		point.x = firstVector.x;
		point.y = firstVector.y;
		points[shape->m_count] = point;

		SDL_RenderDrawLines(this->pRenderer, points, shape->m_count+1);

		delete[] points;

		fixture = body->GetFixtureList()->GetNext();
	}

}

