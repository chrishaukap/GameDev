#pragma once

#include "Actors/Actor.h"

struct Particle
{
   Particle() : 
      _pos(0,0), _vel(0,0), _age(0), _lifetime(0), 
      _color(1,1,1,1), _scale(0) 
   {}
   virtual ~Particle() {}
   void MakeFree() {
      _age = -1.0f;
   }
	Vector2 _pos;
	Vector2 _vel;
	float _age;
	float _lifetime;
	Color _color;
	float _scale;
};

class MyParticleActor : public Actor
{
public:
	MyParticleActor();
	~MyParticleActor();

	virtual void Update(float dt);
	
	virtual void Render();

	void SetSystemLifetime(float lifetime);
	
	void SetParticleLifetime(float lifetime);
	void SetSpread(float radians);
	void SetEndScale(float scale);
	void SetEndColor(const Color& color);
	
	void SetSpeedRange(float minSpeed, float maxSpeed);	
	void SetMinSpeed(float minSpeed);	
	void SetMaxSpeed(float maxSpeed);
	   
   void generateCluster(int numParticles);
   // sets the particles to be used by this class.
   // takes ownership of the particles
   void SetParticles(std::vector<Particle*> particles);
	void GetParticles(std::vector<Particle*>& particles) const;
	
	virtual const String GetClassName() { return "ParticleActor"; }

protected:
    
	Particle** _particles;
	int		_maxParticlesAlive;
	int		_numParticlesAlive;
   
	float	_systemLifetime;
	float	_particleLifetime;

	float	_spreadRadians;

	Color	_endColor; 

	float	_minSpeed; 
	float	_maxSpeed;

	float	_endScale; 
};

