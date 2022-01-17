#pragma once
#include "Component.h"

class UIControlComponent : public Component
{
public:

	UIControlComponent();
	UIControlComponent(Json::Value componentJSON);
	~UIControlComponent();
	void update();


private:
	std::bitset<8> m_controls;





	

};

