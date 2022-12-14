#include "ContactFilter.h"

#include "GameObject.h"
#include "components/PhysicsComponent.h"

ContactFilter::ContactFilter()
{

	using namespace ContactTag;

	m_contactMasks.resize(ContactTag::MAX_OBJECT_CATEGORIES);

	//General_solid - set all to collide
	m_contactMasks[GENERAL_SOLID].reset();
	m_contactMasks[GENERAL_SOLID].flip();

	//General_free - set all to NOT collide
	m_contactMasks[GENERAL_FREE].reset();

	//Initialize the level frame one
	m_contactMasks[LEVEL_CAGE].reset();

}

bool ContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{

	ContactDefinition* contactDefinitionA = reinterpret_cast<ContactDefinition*>(fixtureA->GetUserData().pointer);
	ContactDefinition* contactDefinitionB = reinterpret_cast<ContactDefinition*>(fixtureB->GetUserData().pointer);

	int contactTagA = contactDefinitionA->contactTag;
	int contactTagB = contactDefinitionB->contactTag;

	//If one of these objects is GENERAL_SOLID and the other one is NOT GENERAL_FREE then collide
	if ((contactTagA == ContactTag::GENERAL_SOLID && contactTagB != ContactTag::GENERAL_FREE ) || 
		(contactTagB == ContactTag::GENERAL_SOLID && contactTagA != ContactTag::GENERAL_FREE) ){
		return true;
	}

	auto& contactAMask = m_contactMasks[contactTagA];
	auto& contactBMask = m_contactMasks[contactTagB];

	if (contactAMask.test(contactTagB) &&
		contactBMask.test(contactTagA)) {
		return true;
	}
	else {
		return false;
	}

}
