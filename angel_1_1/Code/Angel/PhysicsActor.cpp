#include "StdAfx.h"
#include "PhysicsActor.h"
#include "World.h"
#include "Box2D.h"
#include "Console.h"
#include "ActorFactory.h"

PhysicsActor::PhysicsActor(void)
:	_physBody(NULL)
{
}

PhysicsActor::~PhysicsActor()
{
	if( _physBody != NULL )
	{
		_physBody->m_userData = NULL;
		theWorld.GetPhysicsWorld().DestroyBody(_physBody);
	}
}


void PhysicsActor::InitPhysics(float density, float friction, float restitution, eShapeType shapeType, bool isSensor, int groupIndex, int collisionFlags, bool fixedRotation)
{
	if (!theWorld.IsPhysicsSetUp())
	{
		std::cout << "ERROR: World physics must be initialized before Actor's." << std::endl;
		return;
	}

	b2CircleDef circle;
	b2PolygonDef box;
	b2ShapeDef* shape;
	if (shapeType == SHAPETYPE_BOX)
	{
		// The extents is just a vector of the box's half widths. 
		// Box2D is tuned for meters, kilograms, and seconds. (Unless you've changed its units.)
		box.SetAsBox(0.5f*_size.X, 0.5f*_size.Y);
		shape = &box;
	}
	else if (shapeType == SHAPETYPE_CIRCLE)
	{
		// TODO: handle ellipse?
		circle.radius = 0.5f*_size.X;
		shape = &circle;
	}
	else
	{
		std::cout << "ERROR: Invalid shape type given." << std::endl;
		return;
	}

	shape->density = density;
	shape->friction = friction;
	shape->restitution = restitution;
	shape->groupIndex = groupIndex;
	shape->isSensor = isSensor;
	if( collisionFlags != -1 )
	{
		shape->maskBits = (short)collisionFlags;
		shape->categoryBits = (short)collisionFlags;
	}

	InitShape( shape );

	b2BodyDef bd;
	bd.userData = this;
	bd.position.Set(_position.X, _position.Y);
	bd.angle = MathUtil::ToRadians(_rotation);
	bd.fixedRotation = fixedRotation;
	if (MathUtil::FuzzyEquals(density, 0.0))
	{
		_physBody = theWorld.GetPhysicsWorld().CreateStaticBody(&bd);
	}
	else
	{
		_physBody = theWorld.GetPhysicsWorld().CreateDynamicBody(&bd);
	}
	_physBody->CreateShape(shape);
	_physBody->SetMassFromShapes();
	CustomInitPhysics();
}

void PhysicsActor::ApplyForce(const Vector2& force, const Vector2& point)
{
	if (_physBody != NULL)
	{
		_physBody->ApplyForce(b2Vec2(force.X, force.Y), b2Vec2(point.X + _position.X, point.Y + _position.Y));
	}
}

void PhysicsActor::ApplyLocalForce(const Vector2& force, const Vector2& point)
{
	if (_physBody != NULL)
	{
		_physBody->ApplyForce(_physBody->GetWorldVector(b2Vec2(force.X, force.Y)), b2Vec2(point.X + _position.X, point.Y + _position.Y));
	}
}

void PhysicsActor::ApplyTorque(float torque)
{
	if (_physBody != NULL)
	{
		_physBody->ApplyTorque(torque);
	}
}

void PhysicsActor::ApplyImpulse(const Vector2& impulse, const Vector2& point)
{
	if (_physBody != NULL)
	{
		_physBody->ApplyImpulse(b2Vec2(impulse.X, impulse.Y), b2Vec2(point.X + _position.X, point.Y + _position.Y));
	}
}

void PhysicsActor::SetSize(float x, float y)
{
	if (_physBody == NULL)
		Actor::SetSize(x, y);
	else
		printf("WARNING: SetSize() had no effect - don't change this actor after physics have been initialized.");
}

void PhysicsActor::SetDrawSize(float x, float y)
{
	Actor::SetSize(x,y);
}

void PhysicsActor::SetPosition(float x, float y)
{
	if (_physBody == NULL)
		Actor::SetPosition(x, y);
	else
		printf("WARNING: SetPosition() had no effect - don't change this actor after physics have been initialized.");
}

void PhysicsActor::SetPosition(Vector2 pos)
{
	if (_physBody == NULL)
		Actor::SetPosition(pos);
	else
		printf("WARNING: SetPosition() had no effect - don't change this actor after physics have been initialized.");
}

void PhysicsActor::SetRotation(float rotation)
{
	if (_physBody == NULL)
		Actor::SetRotation(rotation);
	else
		printf("WARNING: SetRotation() had no effect - don't change this actor after physics have been initialized.");
}

void PhysicsActor::_syncPosRot(float x, float y, float rotation)
{
	_position.X = x;
	_position.Y = y;
	_rotation = rotation;
	while (_rotation > 180.f)
		_rotation -= 360.f;
	while (_rotation < -180.f)
		_rotation += 360.f;
}

void ActorFactorySetDensity(const String& input)
{
	ACTORFACTORY_GETDELEGATE(pDel, PhysicsActorFactoryDelegate);

	pDel->SetDensity( StringToFloat(input) );
}

void ActorFactorySetFriction(const String& input)
{
	ACTORFACTORY_GETDELEGATE(pDel, PhysicsActorFactoryDelegate);

	pDel->SetFriction( StringToFloat(input) );
}

void ActorFactorySetRestitution(const String& input)
{
	ACTORFACTORY_GETDELEGATE(pDel, PhysicsActorFactoryDelegate);

	pDel->SetRestitution( StringToFloat(input) );
}

void ActorFactorySetShapeType(const String& input)
{
	ACTORFACTORY_GETDELEGATE(pDel, PhysicsActorFactoryDelegate);

	String shapeTypeStr = ToUpper( TrimString(input) );

	PhysicsActor::eShapeType shapeType = PhysicsActor::SHAPETYPE_BOX;
	if( shapeTypeStr == "CIRCLE" )
		shapeType = PhysicsActor::SHAPETYPE_CIRCLE;
	//TODO: add new shapes here

	pDel->SetShapeType( shapeType );
}

void ActorFactoryAddCollisionFlag( const String& input )
{
	ACTORFACTORY_GETDELEGATE(pDel, PhysicsActorFactoryDelegate);
	String cvarName = "phys_cc_";
	cvarName += TrimString( input );

	ConsoleVariable* pCVar = theConsole.GetConsoleItemManager().FindCVar( cvarName );
	if( pCVar == NULL )
		return;

	pDel->AddCollisionFlag(pCVar->GetIntVal());
}

void ActorFactorySetIsSensor(const String& input)
{
	ACTORFACTORY_GETDELEGATE(pDel, PhysicsActorFactoryDelegate);
	pDel->SetIsSensor( StringToInt(input) );
}

void ActorFactorySetGroupIndex(const String& input)
{
	ACTORFACTORY_GETDELEGATE(pDel, PhysicsActorFactoryDelegate);

	pDel->SetGroupIndex( StringToInt(input) );
}

void PhysicsActorFactoryDelegate::RegisterOriginalConsoleCommands()
{
	CONSOLE_DECLARECMDSTATICFLAGS( ActorFactorySetDensity, ActorFactorySetDensity, ConsoleCommand::CCF_CONFIG );
	CONSOLE_DECLARECMDSTATICFLAGS( ActorFactorySetFriction, ActorFactorySetFriction, ConsoleCommand::CCF_CONFIG );
	CONSOLE_DECLARECMDSTATICFLAGS( ActorFactorySetRestitution, ActorFactorySetRestitution, ConsoleCommand::CCF_CONFIG );
	CONSOLE_DECLARECMDSTATICFLAGS( ActorFactorySetShapeType, ActorFactorySetShapeType, ConsoleCommand::CCF_CONFIG );
	CONSOLE_DECLARECMDSTATICFLAGS( ActorFactoryAddCollisionFlag, ActorFactoryAddCollisionFlag, ConsoleCommand::CCF_CONFIG );
	CONSOLE_DECLARECMDSTATICFLAGS( ActorFactorySetIsSensor, ActorFactorySetIsSensor, ConsoleCommand::CCF_CONFIG );
	CONSOLE_DECLARECMDSTATICFLAGS( ActorFactorySetGroupIndex, ActorFactorySetGroupIndex, ConsoleCommand::CCF_CONFIG );
}

void PhysicsActorFactoryDelegate::InitializeDelegate()
{
	//Set physics vals back to defaults
	_density = 1.f;
	_friction = 0.3f;
	_restitution = 0.0f;
	_shapeType = PhysicsActor::SHAPETYPE_BOX;
	_collisionFlags = -1;
	_isSensor = false;
	_groupIndex = 0;
}


Actor* PhysicsActorFactoryDelegate::CreateInstance()
{
	return new PhysicsActor();
}

void PhysicsActorFactoryDelegate::FinishInstance(Actor* pActor )
{
	((PhysicsActor*)pActor)->InitPhysics(_density, _friction, _restitution, _shapeType, _isSensor, _groupIndex, _collisionFlags);
}

void PhysicsActorFactoryDelegate::SetDensity( float density )
{
	_density = density;
}

void PhysicsActorFactoryDelegate::SetFriction(float friction)
{
	_friction = friction;
}

void PhysicsActorFactoryDelegate::SetRestitution(float restitution)
{
	_restitution = restitution;
}

void PhysicsActorFactoryDelegate::SetShapeType(PhysicsActor::eShapeType shapeType)
{
	_shapeType = shapeType;
}

void PhysicsActorFactoryDelegate::SetIsSensor(bool isSensor)
{
	_isSensor = isSensor;
}

void PhysicsActorFactoryDelegate::SetGroupIndex(int groupIndex)
{
	_groupIndex = groupIndex;
}

void PhysicsActorFactoryDelegate::AddCollisionFlag( int flagVal )
{
	if( _collisionFlags == -1 )
		_collisionFlags = 0;
	_collisionFlags |= flagVal;
}
