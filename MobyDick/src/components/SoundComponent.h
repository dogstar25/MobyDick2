#ifndef SOUND_COMPONENT_H
#define SOUND_COMPONENT_H

#include "Component.h"

#include <vector>
#include <unordered_map>
#include <optional>

#include <json/json.h>
#include <box2d/box2d.h>

#include "../actions/Action.h"
#include "../actions/InteractAction.h"
#include "../actions/UseAction.h"
#include "../actions/NoAction.h"
#include "../actions/LoadNextLevelAction.h"
#include "../Util.h"
#include "../BaseConstants.h"



struct SoundItem {
	std::string id{};
	std::string soundAssetId{};
	bool isDistanceSensitive{};
	bool isContinuous{};
	int soundChannel{};
	int soundRange{};
};


class SoundComponent : public Component
{
public:
	SoundComponent(Json::Value definitionJSON);
	~SoundComponent();

	void update() override;
	void render();
	void postInit() override;

	SoundItem& getSound(std::string soundId);
	int playSound(std::string soundId);
	void stopSounds();
	void stopSound(int channel);

	void setParent(GameObject* gameObject) override;

private:
	std::unordered_map<std::string, SoundItem > m_sounds{};


	int _calculateSoundDistanceMagnitude(SDL_FPoint, SDL_FPoint, int soundRange);
	

};


#endif