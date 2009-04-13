
#include "stdafx.h"

#include "Renderable.h"
#include "World.h"

#include "GridActor.h"
#include "GridWorld.hpp"

class MyActor : public Actor
{
public: 
   
};

int main(int argc, char* argv[])
{
   theWorld.Initialize(argc, argv, 1024, 768, 200, 200, "BoxGame");
	theWorld.Add(new GridActor());
	
	GridWorld theGrid("world.omg");
   
   const float gridsize = 1.0f;
   const float gridStartX = -6.5f;
   const float gridStartY = 8.5f;

	for (int j = 0; 
        ((j < MAX_WORLD_HEIGHT) && (theGrid.mGrid[0][j] != NULL) && (theGrid.mGrid[0][j]->mpGround->mIcon != '\n')); 
        j++)
	{
		for (int i = 0;
           ((i < MAX_WORLD_WIDTH) && (theGrid.mGrid[i][j]->mpGround->mIcon != '\n')); 
           i++)
		{
         if(!theGrid.mGrid[i][j]->IsNull())
         {
            MyActor *a = new MyActor;
            a->SetPosition(gridStartX + (float(i) * gridsize), gridStartY - (float(j) * gridsize));
            Object *thing = theGrid.mGrid[i][j]->mpGround;
            Object *ground = theGrid.mGrid[i][j]->mpThing;            
				if (!thing->IsNull())
				{
               switch(thing->mIcon)
               {
               case 'P': a->SetColor(0.33f,0,0); break;
               case 'L': a->SetColor(0,0,0);break;
               default: assert(0); break;
               }
				}
				else
				{
               switch(ground->mIcon)
               {
               case 'F':
               case 'C': a->SetColor(0,0,1.0f); break;
               case '0': a->SetColor(0.0f,1,0); break;
               case 'B': a->SetColor(1,0,0); break;
               case 'I': a->SetColor(0.66f,0,0); break;
               default: assert(0); break;
               }
				}
            theWorld.Add(a);
         }              
		}
	}
	
	theWorld.StartGame();

	theWorld.Destroy();
	
	return 0;
}
