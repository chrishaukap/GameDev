#ifndef AICHARACTER_H
#define AICHARACTER_H

#include "characterBase.h"

namespace CDH
{

struct Rectangle;
class Projectile;
class ForegroundObject;
class AICharacter : public CharacterBase
{
public:
   AICharacter(const Vector3& position, 
					const Vector3& rotation, 
					const Rectangle& polygon,
					CHUint hp);  
   virtual ~AICharacter();
   
protected:   
   static const CHUint m_enemyTeam;
   CHUint m_hitpoints;
   CHUint m_lifetime;

private:
   AICharacter(const AICharacter& C);
   const AICharacter& operator=(const AICharacter& C);
};


}
#endif //AICHARACTER_H
