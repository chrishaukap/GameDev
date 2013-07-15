#pragma once

#include <map>
#include <vector>

class Star
{
public:
   Star();
   ~Star();
   bool IsDestroyed() const {return m_state == Destroyed;}
   void Destroy();
   void Update(float dt);
   const ParticleActor* particleCluster() const {return m_particleCluster;}
private:
   enum State
   {
      Twinkle, Decay, Exploding, Destroyed
   };
   State m_state;
   ParticleActor* m_particleCluster;
   Actor* m_graphic;
   float m_timeExistant;
};
class StarSystem
{
public:
   StarSystem();

   virtual void Update(float dt);
   void Cleanup();
   
   void GetStars(std::vector<const ParticleActor*>& stars) const;

private:

   typedef std::map<Star*, float> StarMap;
   StarMap m_stars;
   std::vector<Star*> m_destroyedStars;
};
