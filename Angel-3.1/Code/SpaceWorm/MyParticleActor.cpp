#include "stdafx.h"
#include "MyParticleActor.h"
#include "CollidingParticle.h"
#include "Util/MathUtil.h"


MyParticleActor::MyParticleActor()
{
	_maxParticlesAlive = 0;
	_particles = 0;
   
	_numParticlesAlive = 0;

	_systemLifetime = 0.0f;
	_particleLifetime = 2.0f;

	_spreadRadians = 0.0f;

	_endColor = Color(1.0f, 1.0f, 1.0f);

	_minSpeed = 2.0f;
	_maxSpeed = 4.0f;

	_endScale = 1.0f;
}

MyParticleActor::~MyParticleActor()
{
	delete [] _particles;
}

void MyParticleActor::Update(float dt)
{
	Actor::Update(dt);

	if (_maxParticlesAlive == 0)
		return;

	//
	// Update existing particles.
	//
	_numParticlesAlive = 0;
	for (int i=0; i<_maxParticlesAlive; ++i)
	{
		Particle* currentParticle = _particles[i];

		if (currentParticle->_age < 0.0f)
			continue;

		if (currentParticle->_age < currentParticle->_lifetime)
		{
			currentParticle->_age += dt;

			if (currentParticle->_age < currentParticle->_lifetime)
			{
				// Where are we in our lifespan? (0..1)
				float lifePercent = currentParticle->_age / currentParticle->_lifetime;

				// Determine current position based on last known position, velocity and
				// current time delta.
				currentParticle->_pos = currentParticle->_pos + currentParticle->_vel * dt;
            
				currentParticle->_color = MathUtil::Lerp(_color, _endColor, lifePercent);

				currentParticle->_scale = MathUtil::Lerp(1.0f, _endScale, lifePercent);
				
				++_numParticlesAlive;
			}
			else 
			{
				currentParticle->_age = -1.0f;
				
			}
		}
	}

	// Systems with 0.0f lifetime live forever.
	if (_systemLifetime > 0.0f)
	{
		_systemLifetime -= dt;
		if (_systemLifetime <= 0.f)
			_systemLifetime = -1.f;
	}

	// We're dead, but we're waiting for our particle to finish.
	if (_systemLifetime < 0.0f)
	{
		if (_numParticlesAlive == 0)
		{
			Destroy();
		}

		return;
	}
}

void MyParticleActor::Render()
{
	if (!_particles)
		return;

	int textureReference = _spriteTextureReferences[_spriteCurrentFrame];
	if (textureReference >= 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureReference);
	}

	// Render all of our particles.
	for (int i=0; i<_maxParticlesAlive; ++i)
	{
		Particle* currentParticle = _particles[i];

		if (currentParticle->_age < 0.0f)
			continue;

		glPushMatrix();

		glTranslatef(currentParticle->_pos.X, currentParticle->_pos.Y, 0.0f);
		glRotatef(_rotation, 0, 0, 1);
		glScalef(_size.X * currentParticle->_scale, _size.Y * currentParticle->_scale, 1.0f);

		glColor4f(	currentParticle->_color.R, 
					   currentParticle->_color.G, 
					   currentParticle->_color.B, 
					   currentParticle->_color.A);
		
		const static float vertices[] = {
			-0.5f,  0.5f,
			-0.5f, -0.5f,
			 0.5f,  0.5f,
			 0.5f, -0.5f,
		};
		const static float texCoords[] = {
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
		};
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();
	}

	if (textureReference >= 0)
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void MyParticleActor::SetParticles(std::vector<Particle*> particles)
{   
   for(int i=0; i<_maxParticlesAlive; ++i)
      delete _particles[i];
   delete [] _particles;

	_maxParticlesAlive = particles.size();

	_particles = new Particle*[_maxParticlesAlive];
   for(int i=0; i<_maxParticlesAlive; ++i)
      _particles[i] = particles[i];

	// Make them all available.   Age < 0.0f = free.
	//for (int i=0; i<_maxParticlesAlive; ++i)
	//	_particles[i]->MakeFree();
}
void MyParticleActor::GetParticles(std::vector<Particle*>& particles) const
{
   for(int i=0; i<_maxParticlesAlive; ++i)
      particles.push_back(_particles[i]);
}

void MyParticleActor::SetSystemLifetime(float lifetime)
{
	if (lifetime < 0.0f)
	{
		lifetime = 0.0f;
	}
	_systemLifetime = lifetime;
}

void MyParticleActor::SetParticleLifetime(float lifetime)
{
	if (lifetime < 0.0f)
	{
		lifetime = 0.0f;
	}
	_particleLifetime = lifetime;
}

void MyParticleActor::SetSpread(float radians)
{
	_spreadRadians = radians;
}

void MyParticleActor::SetEndScale(float scale)
{
	_endScale = scale;
}

void MyParticleActor::SetEndColor(const Color& color)
{
	_endColor = color;
}

void MyParticleActor::SetSpeedRange(float minSpeed, float maxSpeed)
{
	_minSpeed = minSpeed;
	_maxSpeed = maxSpeed;
}

void MyParticleActor::SetMinSpeed(float minSpeed)
{
	_minSpeed = minSpeed;
}

void MyParticleActor::SetMaxSpeed(float maxSpeed)
{
	_maxSpeed = maxSpeed;
}

void MyParticleActor::generateCluster(int numParticles)
{
   std::vector<Particle*> particles;
   for(int i=0; i<numParticles; ++i)
   {
      CollidingParticle* particle = new CollidingParticle();
		float rot = MathUtil::ToRadians(GetRotation());
		float particleRot;

		particle->_age = 0.0f;
		particle->_lifetime = _particleLifetime;
		particle->_pos = _position;
		particle->_scale = 1.0f;
		particle->_color = _color;
				
		particleRot = MathUtil::RandomFloatWithError(rot, _spreadRadians);
		float speed = MathUtil::RandomFloatInRange(_minSpeed, _maxSpeed); 
		particle->_vel = Vector2(speed*cos(particleRot), speed*sin(particleRot));
      
      particles.push_back(particle);   
	}
   this->SetParticles(particles);
}