#include "stdafx.h"
#include "DestroyCollisionResponse.h"
#include "PhysicsEventActor.h"
#include "StringUtil.h"

void DestroyCollisionResponse::Execute()
{
	_struck->Destroy();
}

CollisionResponse* DestroyCollisionResponse::FactoryMethod( const StringList& input )
{
	input;
	return new DestroyCollisionResponse();
}
