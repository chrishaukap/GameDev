#pragma once

#include "StringUtil.h"

#include <hash_map>

class ActorFactoryDelegate;
class Actor;

#define theActorFactory ActorFactory::GetInstance()

#define ACTORFACTORY_GETDELEGATE( id, cl )\
	cl* id = (cl*)(theActorFactory.GetCurrentDelegate()); \
	if( id == NULL ) return;

#define ACTORFACTORY_GETACTOR( id, cl ) \
	cl* id = (cl*)(theActorFactory.GetCurrentActor()); \
	if( id == NULL ) return;

class ActorFactory
{
	typedef stdext::hash_map<String, ActorFactoryDelegate*> FactoryDelegateTable;
public:
	static ActorFactory &GetInstance();

	void Initialize();
	void Destroy();

	void AddActorFactoryDelegate( const String& className, ActorFactoryDelegate* del );

	void BeginActor( String actorDefFile, int layer );
	void InitializeActor( const String& className );
	Actor* EndActor();
	Actor* CreateActor( String actorDefFile, const String& name, int layer, void (*CustomInitActor)(Actor*) );

	ActorFactoryDelegate* GetCurrentDelegate() {return _currentDelegate;}
	Actor* GetCurrentActor() {return _currentActor;}

protected:
	ActorFactory();
	~ActorFactory();
	static ActorFactory *s_ActorFactory;

	ActorFactoryDelegate* FindDelegate( const String& className );

private:
	FactoryDelegateTable		_factoryDelegateTable;
	ActorFactoryDelegate*		_currentDelegate;
	Actor*						_currentActor;
	String						_currentActorClass;

};

void DummyInitActor( Actor* pActor);

//sjml - Guys, is this legit? Seems silly to have to call a console command
//  if we don't have to.
void LoadLevel( const String& input );