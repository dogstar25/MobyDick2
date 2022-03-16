#include "PlayerControlComponent.h"


#include "../SceneManager.h"
#include "../EnumMaps.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

PlayerControlComponent::PlayerControlComponent()
{

}

PlayerControlComponent::PlayerControlComponent(Json::Value componentJSON)
{

	for (Json::Value itrControls : componentJSON["controls"])
	{
		int controlFlag = EnumMap::instance().toEnum(itrControls.asString());
		m_controls.set(controlFlag);
	}

}

PlayerControlComponent::~PlayerControlComponent()
{

}

void PlayerControlComponent::init()
{

	m_controls.reset();

}

void PlayerControlComponent::update()
{

	handleActions();

	if (m_controls.test(INPUT_CONTROL_MOVEMENT))
	{
		handleMovement();
	}


}

void PlayerControlComponent::handleActions()
{

	if (SceneManager::instance().playerInputEvents().empty() == false) {
		//convenience reference to outside component(s)
		const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

		const Uint8* keyStates = nullptr;

		for (auto& inputEvent : SceneManager::instance().playerInputEvents())
		{
			std::optional<Action> playerAction{};
			keyStates = inputEvent.keyStates;

			switch (inputEvent.event.type)
			{
				//case SDL_KEYUP:
				case SDL_KEYDOWN:

					//playerAction = getKeyAction(keyCode);
					//playerAction.value()->perform();

					if (keyStates[SDL_SCANCODE_G])
					{
						std::cout << "Dropped Weapon" << "\n";
					}
					if (keyStates[SDL_SCANCODE_E])
					{
						actionComponent->performInteractAction({"test", 5.0, 1});
					}

					break;
				case SDL_MOUSEBUTTONDOWN:

					if (m_controls.test(INPUT_CONTROL_USE))
					{
						actionComponent->performUseAction();
					}

					break;
				default:
					break;
			}

		}
	}
}

/*
*/
void PlayerControlComponent::handleMovement()
{
	int mouseX=0, mouseY=0;
	int direction = 0, strafe = 0;

	//convenience reference to outside component(s)
	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	//Handle Keyboard related movement
	const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (currentKeyStates[SDL_SCANCODE_W])
	{
		direction = -1;
	}
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		direction = 1;
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		strafe = -1;
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		strafe = 1;
	}

	//Keyboard movement
	//Json::Value runtimeParms(Json::objectValue);
	//runtimeParms["direction"] = direction;
	//runtimeParms["strafe"] = strafe;

	actionComponent->performMoveAction(direction, strafe);

	//Handle Mouse related movement
	const uint32_t currentMouseStates = SDL_GetRelativeMouseState(&mouseX, &mouseY);
	float angularVelocity = mouseX * game->contextMananger()->getMouseSensitivity();

	//TEST
	//runtimeParms.clear();
	//runtimeParms["angularVelocity"] = angularVelocity;
	////auto rotateAction = actionComponent->getAction(ACTION_ROTATE);
	////rotateAction->perform(parent(), runtimeParms);
	//auto test = runtimeParms["angularVelocity"].asFloat();

	actionComponent->performRotateAction(angularVelocity);
	//actionComponent->performRotateAction(angularVelocity, runtimeParms);

}


/*
Use this version of handleMovement when you are dealing with controlling multiple objects at the same time
with PlayerControlComponent - Override with this version of PlayerControlComponent at the game level - really only the mouse logic 
*/
//void PlayerControlComponent::handleMovement()
//{
//
//	//convenience reference to outside component(s)
//	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
//
//	const Uint8* keyStates = nullptr;
//
//	for (auto& inputEvent : SceneManager::instance().playerInputEvents())
//	{
//		int direction = 0;
//		int strafe = 0;
//		keyStates = inputEvent.keyStates;
//
//		switch (inputEvent.event.type)
//		{
//		case SDL_KEYUP:
//		case SDL_KEYDOWN:
//		{
//			if (keyStates[SDL_SCANCODE_W])
//			{
//				direction = -1;
//			}
//			if (keyStates[SDL_SCANCODE_S])
//			{
//				direction = 1;
//			}
//			if (keyStates[SDL_SCANCODE_A])
//			{
//				strafe = 1;
//			}
//			if (keyStates[SDL_SCANCODE_D])
//			{
//				strafe = -1;
//			}
//
//			std::cout << "HandleMovement " << direction << " " << strafe << "\sn";
//			actionComponent->performMoveAction(direction, strafe);
//
//			break;
//		}
//		case SDL_MOUSEMOTION:
//		{
//			auto angularVelocity = inputEvent.event.motion.xrel * GameConfig::instance().mouseSensitivity();
//			actionComponent->performRotateAction(angularVelocity);
//
//			break;
//		}
//		default:
//			break;
//		}
//
//	}
//
//}





