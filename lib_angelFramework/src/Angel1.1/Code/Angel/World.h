#pragma once

#include "Renderable.h"
#include "Box2D.h"
#include "PhysicsUtil.h"
#include "GameManager.h"
#include "RenderableIterator.h"

#include <vector>
#include <set>

//forward declaration
class Actor;
class PhysicsActor;
class /*Physics*/DebugDraw;

#define MAX_TIMESTEP 1.0f
#define MAX_CONTACT_POINTS 2048

#define theWorld World::GetInstance()

class World : public b2ContactListener
{
public:
	static World& GetInstance();

	bool Initialize(int argc, char* argv[], 
						 unsigned int windowWidth=1024, unsigned int windowHeight=768, 
						 unsigned int windowPositionX=100, unsigned int windowPositionY=100,
						 String windowName="Angel Engine");
	bool SetupPhysics(Vector2 gravity = Vector2(0, -10), Vector2 maxVertex = Vector2(100.0f, 100.0f), Vector2 minVertex = Vector2(-100.0f, -100.0f));
	void Destroy();
	void ResetWorld();
	void StartGame();
	void StopGame();

	void UpdateRenderables(float frame_dt);
	void CleanupRenderables();
	void DrawRenderables();

	const float GetDT();
	const bool StopSimulation();
	const bool StartSimulation();
	
	void SetBackgroundColor(Color3f bgColor);

	static void DisplayCallback();
	static void IdleCallback();

	void Add(Renderable *newElement, int layer = 0);
	void Remove(Renderable *oldElement);
	void UpdateLayer(Renderable* element, int newLayer);

	// physics
	b2World &GetPhysicsWorld();
	const bool IsPhysicsSetUp() { return _physicsSetUp; }

	RenderLayers& GetLayers() { return _layers; }

	void WakeAllPhysics();
	void AddCollisionListener(CollisionListener* listener);
	void RemoveCollisionListener(CollisionListener* listener);

	//Contact listener implementations
	void Add(b2ContactPoint* point);
	void Persist(b2ContactPoint* point);
	void Remove(b2ContactPoint* point);

	void SetSideBlockers(bool turnOn, float resitution=-1.0f);
	void CameraChangeNotify();

	float GetCurrentTimeSeconds() { return ( float(_currTime)*0.001f ); }
	float GetTimeSinceSeconds( float lastTime ) {return GetCurrentTimeSeconds() - lastTime;}

	void SetGameManager(GameManager* gameManager);
	GameManager* GetGameManager() { return _gameManager; }

	RenderableIterator GetFirstRenderable()
	{
		RenderableIterator iter;
		return iter.begin(); 
	}
	RenderableIterator GetLastRenderable()
	{
		RenderableIterator iter;
		return iter.end();
	}

	void UnloadAll();
   void getWindowDimensions(unsigned int &width, unsigned int &height) const {width = m_windowWidth; height = m_windowHeight;}

protected:
	World();
	static World* s_World;
	float CalculateNewDT();
	void Simulate(bool simRunning);
	void TickAndRender();
	void ProcessDeferredAdds();
	void ProcessDeferredLayerChanges();
	void ProcessDeferredRemoves();
	void RunPhysics(float frame_dt);

private:
   unsigned int m_windowWidth;
   unsigned int m_windowHeight;
	struct RenderableLayerPair
	{
		Renderable* _renderable;
		int			_layer;
	};
	int _prevTime;
	int _currTime;
	float _dt;
	float _physicsRemainderDT;

	bool _simulateOn;
	bool _initialized;
	bool _started;


	RenderLayers _layers;

	std::vector<RenderableLayerPair> _deferredAdds;	
	std::vector<RenderableLayerPair> _deferredLayerChanges;
	std::vector<Renderable*> _deferredRemoves;

	bool _elementsLocked;

	b2World *_physicsWorld;
	bool _physicsSetUp;

	/*Physics*/DebugDraw *_physicsDebugDraw;

	void bufferContactPoint(b2ContactPoint* cp);
	b2ContactPoint _contactPoints[MAX_CONTACT_POINTS];
	int _contactPointCount;
	std::set<CollisionListener*> _collisionListeners;

	bool _blockersOn;
	float _blockerRestitution;
	PhysicsActor* _blockers[4];

	GameManager* _gameManager;
};
