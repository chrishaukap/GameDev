#include "stdafx.h"
#include "GotoTargetAIEvent.h"
#include "Sentient.h"
#include "PathFinder.h"
#include "Box2D.h"
#include "TagCollection.h"
#include "SpatialGraph.h"

GotoTargetAIEvent* GotoTargetAIEvent::Initialize( const String& targetTag, float moveSpeed, float arrivalDist )
{
	_targetTag = targetTag;
	GotoAIEvent::Initialize( Vector2::Zero, moveSpeed, arrivalDist );
	return this;
}

void GotoTargetAIEvent::Update(float dt)
{
	ActorSet taggedActors = tagList.GetObjectsTagged(_targetTag);
	for( ActorSet::iterator itr = taggedActors.begin(); itr != taggedActors.end(); itr++ )
	{
		Actor* pTargetActor = (*itr);
		if( theSpatialGraph.IsInPathableSpace( pTargetActor->GetPosition() ) )
		{
			_destination = pTargetActor->GetPosition();
			GotoAIEvent::Update( dt );
			return;
		}
	}

	//otherwise, we failed
	_moveFailed = true;
	IssueCallback();

}
