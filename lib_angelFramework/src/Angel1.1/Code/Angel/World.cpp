
#include "stdafx.h"

#include "World.h"
#include "Camera.h"
#include "Input.h"
#include "MouseInput.h"
#include "Xbox360Input.h"
#include "TextRendering.h"
#include "Actor.h"
#include "PhysicsActor.h"
#include "Console.h"
#include "StringUtil.h"
#include "Log.h"
#include "Sound.h"
#include "SpatialGraph.h"
#include "InputManager.h"
#include "ActorFactory.h"
#include "PhysicsDebugDraw.h"
#include "CollisionResponse.h"

#include <algorithm>

World* World::s_World = NULL;

World::World()
{
	_simulateOn = true;
	_initialized = false;
	_started = false;
	_physicsRemainderDT = 0.0f;
	_physicsSetUp = false;

	_blockersOn = false;
	_blockerRestitution = 0.0f;
	_blockers[0] = _blockers[1] = _blockers[2] = _blockers[3] = NULL;
	_physicsDebugDraw = NULL;
	_gameManager = NULL;
	_elementsLocked = false;
}

World& World::GetInstance()
{
	if (s_World == NULL)
	{
		s_World = new World();
	}
	return *s_World;
}

void ReloadLevel( const String& levelName )
{
	theWorld.UnloadAll();
	LoadLevel( levelName );
}


void UnloadAllStatic( const String& input )
{
	theWorld.UnloadAll();
}

bool World::Initialize(int argc, char* argv[], 
							  unsigned int windowWidth, 
							  unsigned int windowHeight, 
							  unsigned int windowPositionX,
							  unsigned int windowPositionY,
							  String windowName)
{
	if (_initialized)
	{
		return false;
	}
   m_windowWidth = windowWidth;
   m_windowHeight = windowHeight;

	//standard glut initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL );
	glutInitWindowSize(m_windowWidth, m_windowHeight);
	glutCreateWindow(windowName.c_str());
	glutPositionWindow(windowPositionX, windowPositionY);

	//set up callbacks
	glutReshapeFunc(Camera::ResizeCallback);
	glutDisplayFunc(DisplayCallback);
	glutIdleFunc(IdleCallback);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MouseMotion);
	glutMouseFunc(MouseButton);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keyboardSpecialDown);
	glutSpecialUpFunc(keyboardSpecialUp);
	glutIgnoreKeyRepeat(GL_TRUE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	_prevTime = glutGet(GLUT_ELAPSED_TIME);

	glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT, GL_FILL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearStencil(0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	theCamera.ResizeCallback(windowWidth, windowHeight);
	theControllerManager.Setup();

	theConsole;
	theInput;
	theSound;
	theActorFactory;
	theSpatialGraph;

	theConsole.ExecConfigFile( "autoexec" );

	CONSOLE_DECLARECMDSTATIC( UnloadAll, UnloadAllStatic );
	CONSOLE_DECLARECMDSTATIC( ReloadLevel, ReloadLevel );

	return _initialized = true;
}

bool World::SetupPhysics(Vector2 gravity, Vector2 maxVertex, Vector2 minVertex)
{
	if (_physicsSetUp)
	{
		return false;
	}

	//setup physics
	b2AABB worldAABB;
	worldAABB.lowerBound.Set(minVertex.X, minVertex.Y);
	worldAABB.upperBound.Set(maxVertex.X, maxVertex.Y);
	b2Vec2 gravityVector(gravity.X, gravity.Y);
	bool doSleep = true;
	_physicsWorld = new b2World(worldAABB, gravityVector, doSleep);

	_physicsWorld->SetListener(this);

	_physicsDebugDraw = new /*Physics*/DebugDraw();
	_physicsWorld->SetDebugDraw(_physicsDebugDraw);

	CONSOLE_DECLAREVAR( phys_gravity );
	if( phys_gravity->HasVal() )
	{
		Vector2 grav = phys_gravity->GetVector2Val();
		_physicsWorld->m_gravity = b2Vec2( grav.X, grav.Y );
	}

	return _physicsSetUp = true;
}


void World::Destroy()
{
	theSound.Destroy();
	theInput.Destroy();
	theConsole.Destroy();
	theActorFactory.Destroy();
	//rb - Why is this commented out?
	//theSpatialGraph.Destroy();

	delete _physicsDebugDraw;
}

void World::StartGame()
{
	if (_started)
	{
		return;
	}
	_started = true;

	//enter main loop
	glutMainLoop();
}

void World::StopGame()
{
	glutLeaveMainLoop();
}

float World::CalculateNewDT()
{
	_currTime = glutGet(GLUT_ELAPSED_TIME);
	_dt = MathUtil::Clamp((_currTime - _prevTime) * 0.001f, 0.0f, MAX_TIMESTEP);
	_prevTime = _currTime;
	return _dt;
}

void World::Simulate(bool simRunning)
{
	float frame_dt = CalculateNewDT();

	//system updates
	theConsole.Update( (float)frame_dt );
	theControllerManager.UpdateState();

	//make sure the game manager gets updates first, if we have one
	if (_gameManager)
	{
		_gameManager->Update(frame_dt);
	}

	if (simRunning)
	{
		//Clear out the collision contact points
		_contactPointCount = 0;

		//Update gravity
		CONSOLE_ONCVARCHANGED( phys_gravity, Vector2, 
		{
			GetPhysicsWorld().m_gravity = b2Vec2( _cvarNewVal.X, _cvarNewVal.Y );
			if( _cvarOldVal == Vector2(0) )
				WakeAllPhysics();
		})

		
		//rb - Flag that the _elements array is locked so we don't try to add any
		// new actors during the update.
		_elementsLocked = true;
			UpdateRenderables(frame_dt);
			CleanupRenderables();
		_elementsLocked = false; 

		// Now that we're done updating the list, allow any deferred Adds to be processed.
		ProcessDeferredAdds();
		ProcessDeferredLayerChanges();
		ProcessDeferredRemoves();

		RunPhysics(frame_dt);
		
		//if there are any system updates that still need to be run, put them here
	}
   
	// do cleanup
	if (_gameManager)
	{
		_gameManager->CleanUp();
      CleanupRenderables();
	}
}

void World::RunPhysics(float frame_dt)
{
	if (!_physicsSetUp) 
		return;

	// fixed time step
	const float physicsDT = 1.0f/60.f;
	
	float total_step = _physicsRemainderDT + frame_dt;
	while (total_step >= physicsDT)
	{
		// more iterations -> more stability, more cpu
		// tune to your liking...
		GetPhysicsWorld().Step(physicsDT, /*iterations*/ 10);
		total_step -= physicsDT;
	}
	_physicsRemainderDT = total_step;

	// update PhysicsActors
	for (b2Body* b = GetPhysicsWorld().GetBodyList(); b; b = b->m_next)
	{
		b2Vec2 vec = b->GetPosition();
		PhysicsActor *physActor = reinterpret_cast<PhysicsActor*>(b->GetUserData());
		if (physActor != NULL)
		{
			physActor->_syncPosRot(vec.x, vec.y, MathUtil::ToDegrees(b->GetAngle()));
		}
	}

	//TODO: this could be optimized by only storing contact points
	// if there's a listener who cares about them.
	// Should probably also think about setting up subscriptions
	//  so messages only get passed if the Actor cares about them. 
	// For now, everyone hears about everything. 
	for(int i=0; i < _contactPointCount; ++i)
	{
		const b2ContactPoint& contact = _contactPoints[i];
		Vector2 hitLoc(contact.position.x, contact.position.y);
		std::set<CollisionListener*>::iterator listenerIt = _collisionListeners.begin();
		while(listenerIt != _collisionListeners.end())
		{
			(*listenerIt)->OnCollision(contact.shape1, contact.shape2, contact.normalForce, hitLoc);
			++listenerIt;
		}
	}
	theCollisionResponseFactory.ProcessCollisions();
}

void World::bufferContactPoint(b2ContactPoint* point)
{
	if (_contactPointCount == MAX_CONTACT_POINTS)
	{
		return;
	}

	b2ContactPoint* cp = _contactPoints + _contactPointCount;
	cp->shape1 = point->shape1;
	cp->shape2 = point->shape2;
	cp->position = point->position;
	cp->normal = point->normal;
	cp->normalForce = point->normalForce;
	cp->tangentForce = point->tangentForce;

	_contactPointCount++;
}

void World::Add(b2ContactPoint* point)
{
	//Persist is the only consistent message we care about at the moment
	bufferContactPoint(point);
}

void World::Persist(b2ContactPoint* point)
{
	bufferContactPoint(point);
}

void World::Remove(b2ContactPoint* point)
{
	//Persist is the only consistent message we care about at the moment
}

void World::TickAndRender()
{
	Simulate(_simulateOn);

	// Setup the camera matrix.
	theCamera.Render();

	DrawRenderables();

	// Give the GameManager a chance to draw something.
	if (_gameManager)
		_gameManager->Render();

	//Render debug information
	theSpatialGraph.Render();

	//Draw developer console
	theConsole.Render();
}

void World::CleanupRenderables()
{
	RenderableIterator it = theWorld.GetFirstRenderable();
	while (it != theWorld.GetLastRenderable())
	{
		if ((*it)->IsDestroyed())
		{
			it = it.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void World::UpdateRenderables(float frame_dt)
{
	RenderableIterator it = theWorld.GetFirstRenderable();
	while (it != theWorld.GetLastRenderable())
	{
		(*it)->Update(frame_dt);
		++it;
	}
}

void World::DrawRenderables()
{
	RenderableIterator it = theWorld.GetFirstRenderable();
	while (it != theWorld.GetLastRenderable())
	{
		(*it)->Render();
		++it;
	}
}

void World::DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	theWorld.TickAndRender();

	glPopMatrix();
	glutSwapBuffers();
}

const float World::GetDT()
{
	return _dt;
}

const bool World::StartSimulation()
{
	return _simulateOn = true;
}

const bool World::StopSimulation()
{
	return _simulateOn = false;
}

void World::IdleCallback()
{
	glutPostRedisplay();
}

void World::ResetWorld()
{
	theCamera.Reset();
}

void World::SetBackgroundColor(Color3f bgColor)
{
	glClearColor(bgColor.R, bgColor.G, bgColor.B, 1.0f);
}

void World::Add(Renderable *newElement, int layer)
{
	// If we're not locked, add directly to _elements.
	if (!_elementsLocked)
	{
		newElement->_layer = layer;
		_layers[layer].push_back(newElement);
	}
	// If we're locked, add to _deferredAdds and we'll add the new
	// Renderable after we're done updating all the _elements.
	else
	{
		RenderableLayerPair addMe;
		addMe._layer = layer;
		addMe._renderable = newElement;
		_deferredAdds.push_back( addMe );
	}
}

void World::Remove(Renderable *oldElement)
{
	if (oldElement == NULL)
	{
		return;
	}

	if (_elementsLocked)
	{
		_deferredRemoves.push_back(oldElement);
		return;
	}

	// First, make sure that it isn't deferred in the _deferredAdds list.
	std::vector<RenderableLayerPair>::iterator it = _deferredAdds.begin();
	while (it != _deferredAdds.end())
	{
		if ((*it)._renderable == oldElement)
		{
			_deferredAdds.erase(it);
			return;
		}
		++it;
	}

	// If we didn't find it in the deferred list, find/remove it from the layers.
	bool found = false;
	// Find the layer that matches the elements layer.
	RenderLayers::iterator layer = _layers.find(oldElement->_layer);
	// Found the layer (list of renderables).
	if ( layer != _layers.end() )
	{
		// Now that we have the list of elements for this layer, let's remove the requested element.
		//rb - TODO - Cache off vector.
		std::vector<Renderable*>::iterator element = (*layer).second.begin();
		for ( ; element != (*layer).second.end(); ++element )
		{
			// Found it.
			if ( (*element) == oldElement )
			{
				// Remove the element.
				element = (*layer).second.erase(element);
				found = true;
				// Nothing else to do.
				break;
			}
		}
		if (!found)
		{
			//log or error handle
		}
	}
}

void World::UpdateLayer(Renderable* element, int newLayer)
{
	if (element->_layer == newLayer)
		return;

	RenderableLayerPair layerChange;
	layerChange._layer = newLayer;
	layerChange._renderable = element;
	_deferredLayerChanges.push_back( layerChange );
}

void World::AddCollisionListener(CollisionListener* listener)
{
	_collisionListeners.insert(listener);
}

void World::RemoveCollisionListener(CollisionListener* listener)
{
	_collisionListeners.erase(listener);
}

void World::WakeAllPhysics()
{
	for (b2Body* b = GetPhysicsWorld().GetBodyList(); b; b = b->m_next)
	{
		b->WakeUp();
	}
}

b2World& World::GetPhysicsWorld()
{
	return *_physicsWorld;
}

void World::SetSideBlockers(bool turnOn, float restitution)
{
	if (!MathUtil::FuzzyEquals(restitution, -1.0f))
	{
		_blockerRestitution = restitution;
	}

	if (_blockers[0] != NULL)
	{
		theWorld.Remove(_blockers[0]);
		theWorld.Remove(_blockers[1]);
		theWorld.Remove(_blockers[2]);
		theWorld.Remove(_blockers[3]);
		delete _blockers[0];
		delete _blockers[1];
		delete _blockers[2];
		delete _blockers[3];
		_blockers[0] = NULL;
		_blockers[1] = NULL;
		_blockers[2] = NULL;
		_blockers[3] = NULL;
	}

	if (!turnOn)
	{
		_blockersOn = false;
		return;
	}

	_blockersOn = true;

	Vector2 botLeft = theCamera.GetWorldMinVertex();
	Vector2 topRight = theCamera.GetWorldMaxVertex();
	float worldHeight = topRight.Y - botLeft.Y;
	float worldWidth = topRight.X - botLeft.X;

	float thickness = 5.0f; //just so it's thick enough to avoid tunnelling
	Vector2 screenOrigin(((topRight.X - botLeft.X) * 0.5f) + botLeft.X,
						 ((topRight.Y - botLeft.Y) * 0.5f) + botLeft.Y);
	
	//right blocker
	_blockers[0] = new PhysicsActor();
	_blockers[0]->SetPosition(topRight.X + (thickness * 0.5f), screenOrigin.Y);
	_blockers[0]->SetColor(1, .5, .5);
	_blockers[0]->SetSize(thickness, worldHeight + 1.0f);
	_blockers[0]->InitPhysics(0.0f, 0.1f, _blockerRestitution);

	//left blocker
	_blockers[1] = new PhysicsActor();
	_blockers[1]->SetPosition(botLeft.X - (thickness * 0.5f), screenOrigin.Y);
	_blockers[1]->SetColor(1, .5, .5);
	_blockers[1]->SetSize(thickness, worldHeight + 1.0f);
	_blockers[1]->InitPhysics(0.0f, 0.1f, _blockerRestitution);

	//top blocker
	_blockers[2] = new PhysicsActor();
	_blockers[2]->SetPosition(screenOrigin.X, topRight.Y + (thickness * 0.5f));
	_blockers[2]->SetColor(1.0f, .5f, .5f);
	_blockers[2]->SetSize(worldWidth + 1.0f, thickness);
	_blockers[2]->InitPhysics(0.0f, 0.1f, _blockerRestitution);

	//bottom blocker
	_blockers[3] = new PhysicsActor();
	_blockers[3]->SetPosition(screenOrigin.X, botLeft.Y - (thickness * 0.5f));
	_blockers[3]->SetColor(1.0f, .5f, .5f);
	_blockers[3]->SetSize(worldWidth + 1.0f, thickness);
	_blockers[3]->InitPhysics(0.0f, 0.1f, _blockerRestitution);

	// We don't want these removed when we call ReloadLevel.
	for (int i=0; i<4; ++i)
	{
		_blockers[i]->Tag("NoDelete");
	}
	
	theWorld.Add(_blockers[0]);
	theWorld.Add(_blockers[1]);
	theWorld.Add(_blockers[2]);
	theWorld.Add(_blockers[3]);
}

void World::CameraChangeNotify()
{
	if (_blockersOn)
	{
		SetSideBlockers(true);
	}
}

void World::ProcessDeferredAdds()
{
	std::vector<RenderableLayerPair>::iterator it = _deferredAdds.begin();
	while(it != _deferredAdds.end())
	{
		Add( (*it)._renderable, (*it)._layer );
		++it;
	}

	_deferredAdds.clear();
}

void World::ProcessDeferredLayerChanges()
{
	//TODO: use appropriate layer
	std::vector<RenderableLayerPair>::iterator it = _deferredLayerChanges.begin();
	while(it != _deferredLayerChanges.end())
	{
		Remove((*it)._renderable);
		Add( (*it)._renderable, (*it)._layer );
		++it;
	}
	_deferredLayerChanges.clear();
}

void World::ProcessDeferredRemoves()
{
	RenderList::iterator it = _deferredRemoves.begin();
	while(it != _deferredRemoves.end())
	{
		Remove(*it);
		++it;
	}
	_deferredRemoves.clear();
}

// This should only be done once. (at least for now)
void World::SetGameManager(GameManager* gameManager)
{
	if ( (_gameManager != NULL) || (gameManager == NULL) )
	{
		std::cout << "ERROR: Can only have one game manager!" << std::endl;
		return;
	}

	_gameManager = gameManager;
}

void World::UnloadAll()
{
	RenderableIterator it = theWorld.GetFirstRenderable();
	while (it != GetLastRenderable())
	{
		Renderable* renderable = (*it);
		if( _gameManager != NULL && _gameManager->IsProtectedFromUnloadAll(renderable))
		{
			// Let the persistent actors know that we're unloading the level.
			Actor* actor = dynamic_cast<Actor*>(renderable);
			if (actor)
				actor->LevelUnloaded();
			++it;
			continue;
		}

		it = it.erase( it );
		renderable->Destroy();
		delete renderable;
	}
}

