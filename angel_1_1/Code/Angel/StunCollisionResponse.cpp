#include "stdafx.h"
#include "StunCollisionResponse.h"
#include "PhysicsEventActor.h"
#include "StringUtil.h"

void StunCollisionResponse::Execute()
{
	_struck->OnNamedEvent( "stun" );
}

CollisionResponse* StunCollisionResponse::FactoryMethod( const StringList& input )
{
	input;
	return new StunCollisionResponse();
}
