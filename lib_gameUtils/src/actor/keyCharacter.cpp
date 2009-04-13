#include "cdhBase.h"
#include "actor/keyCharacter.h"
#include "input/keyInput.h"
#include "actor/foregroundObject.h"

using namespace CDH;

const CHUint KeyCharacter::m_playerTeam = 1;

KeyCharacter::KeyCharacter(const KeyInput& input, 
									const Vector3& position, 
									const Vector3& rotation, 
									const Rectangle& polygon) : 
	CharacterBase(position, rotation, polygon, CollidableCharacter, m_playerTeam),
   m_input(input)
{}
KeyCharacter::~KeyCharacter()
{}

Status 
KeyCharacter::process()
{
	return StatusSuccess;
}

void KeyCharacter::resolveCollisionWithProjectile( const Projectile& object ){}
void KeyCharacter::resolveCollisionWithEnemy( const CharacterBase& object ){}
void KeyCharacter::resolveCollisionWithTerrain( const ForegroundObject& object ){}