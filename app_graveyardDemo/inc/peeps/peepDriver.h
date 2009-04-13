#ifndef PEEPDRIVER_H
#define PEEPDRIVER_H

#include "defines.h"
#include <vector>

class AbstractPeep;
class BloodEngine;
class Peep;
class BloodSpout;

class PeepDriver
{
private: 
   class DeadPeep;
   
   // owns both the BloodSpout and the DeadPeep*
   typedef std::vector<DeadPeep*> PEEP_PARTS;
public:
   typedef std::vector<Peep*> PEEPS;
   
   PeepDriver(BloodEngine& bloodEngine);
   ~PeepDriver();

   CHUint numPeeps() const {return (CHUint)m_peeps.size();}
   PEEPS& getPeeps() {return m_peeps;}
   void initPeeps();
   void process();
   void processParts();
   void renderPeeps() const;
   void renderParts() const;
   
   void killPeep(const AbstractPeep& peep);   

   // DEBUG
   CHUint numPeepParts() const;
private:
   BloodSpout& createBloodSpoutForBodyPart(const AbstractPeep& peep);
   void insertNewPeep(Peep* peep);

   PEEPS m_peeps;
   PEEP_PARTS m_peepParts;
   int m_maxPeeps;
   BloodEngine& m_bloodEngine;
};
#endif // PEEPDRIVER_H