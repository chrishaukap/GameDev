#include "cdhBase.h"
#include "actor/aiCharacter.h"
#include "actor/projectile.h"
#include <assert.h>

using namespace CDH;
const CHUint AICharacter::m_enemyTeam = 2;

AICharacter::AICharacter(const Vector3& position, 
								 const Vector3& rotation, 
								 const Rectangle& polygon,								 
								 CHUint hp) : 
	CharacterBase(position, rotation, polygon, CollidableCharacter, m_enemyTeam),
   m_hitpoints(hp),
   m_lifetime(200)
{}
AICharacter::~AICharacter()
{}
