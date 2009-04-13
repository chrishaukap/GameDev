#include "stdafx.h"
#include "PlaySoundCollisionResponse.h"
#include "PhysicsEventActor.h"
#include "StringUtil.h"
#include "Sound.h"

void PlaySoundCollisionResponse::Execute()
{
	theSound.PlaySound(_sound,1.0f,false,0);
}

CollisionResponse* PlaySoundCollisionResponse::FactoryMethod( const StringList& input )
{
	if( input.size() > 0 )
	{
		Sound* pSound = theSound.LoadSound(input[0].c_str());
		if (pSound)
		{
			PlaySoundCollisionResponse* pColResponse = new PlaySoundCollisionResponse();
			pColResponse->_volume = 1.0f;
			pColResponse->_sound = pSound;

			if( input.size() > 1 )
			{
				pColResponse->_volume = StringToFloat(input[1] );
			}

			return pColResponse;
		}
	}

	return NULL;
}
