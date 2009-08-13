#pragma once

#include "Actor.h"

#include "Box2D.h"
struct b2ShapeDef;


class PhysicsActor : public Actor
{
public:
	PhysicsActor();
	virtual ~PhysicsActor();

	enum eShapeType
	{
		SHAPETYPE_BOX,
		SHAPETYPE_CIRCLE,
	};

	virtual void InitPhysics(float density = 1.f, float friction = 0.3f, float restitution = 0.0f, eShapeType shapeType = SHAPETYPE_BOX, bool isSensor = false, int groupIndex = 0, int collisionFlags =-1, bool fixedRotation = false);
	virtual void CustomInitPhysics() {}

	// point parameters in Apply* functions are locations in actor space 
	// (if you want world space, convert yourself or use GetBody() and the underlying Box2D API)
	void ApplyForce(const Vector2& force, const Vector2& point);
	void ApplyLocalForce(const Vector2& force, const Vector2& point);	// apply a local space force on the object
	void ApplyTorque(float torque);
	void ApplyImpulse(const Vector2& impulse, const Vector2& point);
	void GetLinearVelocity(Vector2& linearVelocity) ;

	// note, wake the body before applying velocities or it will continue sleeping (if it was)
	b2Body *GetBody() { return _physBody; }

	// these are dangerous things to do, so have been disabled.
	// implement if you know exactly what you want physics to do in these cases.
	void SetSize(float x, float y = -1.f);
	void SetDrawSize(float x, float y = -1.f);
	void SetPosition(float x, float y);
	void SetPosition(Vector2 pos);
	void SetRotation(float rotation);
	virtual bool HandlesCollisionEvents() {return false;}

protected:
	virtual void InitShape(b2ShapeDef* shapeDef ) {shapeDef;}
	b2Body *_physBody;

private:
	friend class World;

	void _syncPosRot(float x, float y, float rotation);
};

class PhysicsActorFactoryDelegate : public ActorFactoryDelegate
{
public:
	virtual void RegisterOriginalConsoleCommands();
	virtual Actor* CreateInstance(); 
	virtual void FinishInstance(Actor* pActor );

	virtual void SetDensity( float density );
	virtual void SetFriction(float friction);
	virtual void SetRestitution(float restitution);
	virtual void SetShapeType(PhysicsActor::eShapeType shapeType);
	virtual void SetIsSensor(bool isSensor);
	virtual void AddCollisionFlag( int flagVal );
	virtual void SetGroupIndex(int groupIndex);

protected:
	virtual void InitializeDelegate();
	float						_density;
	float						_friction;
	float						_restitution;
	PhysicsActor::eShapeType	_shapeType;
	int							_collisionFlags;
	bool						_isSensor;
	int							_groupIndex;
};
