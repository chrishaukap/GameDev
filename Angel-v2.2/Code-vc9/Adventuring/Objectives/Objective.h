#pragma once

class Objective
{
public:   

   virtual ~Objective();
   bool isComplete() const;
   void doWork(float dt);
   bool cannotComplete() const {return m_cannotComplete;}
   const Objective& activeObjective() const;
   const std::string& name() const;
protected:
   Objective();
   std::string m_name;
   std::list<Objective*> m_prereqs;

   const Objective& currentObjective() const;
   
   bool hasPrereqs() const;
   bool arePrereqsComplete() const;
   void workOnPrereqs(float dt);

   virtual void applyWork(float dt)  = 0;
   virtual bool objectiveMet() const = 0;
   bool m_cannotComplete;
};





