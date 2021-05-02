#pragma once

#include <memory>
#include <vector>
#include <bitset>
#include <optional>

#include <box2d/box2d.h>
#include <json/json.h>
#include "Globals.h"
#include "BaseConstants.h"
#include "GameObject.h"
#include "GameConfig.h"

#include "ObjectPoolManager.h"
//class ObjectPoolManager;

struct SceneAction
{
	int actionCode{ 0 };
	std::string sceneId;
};

struct PhysicsConfig
{
	b2Vec2 gravity{};
	float timeStep{};
	int velocityIterations{};
	int positionIterations{};
	bool b2DebugDrawMode{false};
};

class Scene
{

public:
	Scene(std::string sceneId);
	~Scene();

	void run();
	void render();
	void update();
	void clearEvents();
	GameObject* addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle=0., bool cameraFollow=false);
	GameObject* addGameObject(std::shared_ptr<GameObject> gameObject, int layer);
	void addKeyAction(SDL_Keycode, SceneAction);
	void applyCurrentControlMode();
	SDL_FPoint calcWindowPosition(int globalPosition);
	
	void stepB2PhysicsWorld() {
		m_physicsWorld->Step(m_physicsConfig.timeStep,
			m_physicsConfig.velocityIterations,
			m_physicsConfig.positionIterations);
	}

	const std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS>& gameObjects() {
		return m_gameObjects;
	}

	std::string id() {
		return m_id;
	}
	int parentSceneIndex() {
		return m_parentSceneIndex;
	}

	//Accessor Functions
	b2World* physicsWorld() {
		return m_physicsWorld;
	}

	void setState(SceneState state) {
		m_state = state;
	}

	SceneState state() {
		return m_state;
	}

	ObjectPoolManager& objectPoolManager() {
		return m_objectPoolManager;
	}

	PhysicsConfig physicsConfig() { return m_physicsConfig; }

	void setInputControlMode(int inputControlMode);

	std::optional<SceneAction> getkeycodeAction(SDL_Keycode keycode) {
		if (m_sceneKeyActions.find(keycode) != m_sceneKeyActions.end()) {
			return m_sceneKeyActions.at(keycode);
		}
		else {
			return std::nullopt;
		}
	}
	
private:
	std::string m_id;
	SceneState m_state;
	b2World* m_physicsWorld{ nullptr };
	PhysicsConfig m_physicsConfig;

	int m_inputControlMode;
	std::bitset<8> m_sceneTags;
	std::map<SDL_Keycode, SceneAction> m_sceneKeyActions;
	ObjectPoolManager m_objectPoolManager;

	int m_parentSceneIndex;
	std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS> m_gameObjects;

	void _processGameObjectInterdependecies();

};

