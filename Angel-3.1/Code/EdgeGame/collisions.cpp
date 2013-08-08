#include "collisions.h"

#include "Tim.h"
#include "Indie.h"
#include "Money.h"

#include <assert.h>

namespace Edge
{
   void CollisionIndieIndie(iCollidable& _indie1, iCollidable& _indie2)
   {
      Indie* indie1 = dynamic_cast<Indie*>(&_indie1);
      Indie* indie2 = dynamic_cast<Indie*>(&_indie2);
		assert(indie1 != indie2);

		if( ( !indie1->isOnTeam() || !indie2->isOnTeam() ) ||
				indie1->getTeamNumber() != indie2->getTeamNumber() )
		{
			indie1->joinIndie(*indie2);
			indie2->joinIndie(*indie1);
			if(indie1->isLeader() && indie2->isLeader())
				indie2->demote();
			indie1->setTeamColor();
		}
   }
   void CollisionTimIndie(iCollidable& _tim, iCollidable& _indie)
   {
      Indie* indie = dynamic_cast<Indie*>(&_indie);	

      Tim* tim = dynamic_cast<Tim*>(&_tim);		
		indie->killMe();
   }
   void CollisionMoneyIndie(iCollidable& _money, iCollidable& _indie)
   {
      Money* money = dynamic_cast<Money*>(&_money);
      Indie* indie = dynamic_cast<Indie*>(&_indie);

		money->markMeForDeletion();
			
		Vector2 formerSize = indie->GetSize();
		//indie->SetSize(formerSize.X * 2, formerSize.Y * 2);
   }
}
        