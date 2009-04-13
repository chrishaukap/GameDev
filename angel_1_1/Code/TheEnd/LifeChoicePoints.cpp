#include "stdafx.h"
#include "LifeChoicePoints.h"
#include "LifeChoice.h"

LifeChoicePoints* LifeChoicePoints::s_lifeChoicePoints = NULL;
const String LifeChoicePoints::m_lifeChoiceStrings[] = 
   {
      "Family", "Love", "Friendship", "Health", "Success", "Education", "Travel", "Children", "Legacy"
   };

LifeChoicePoints& LifeChoicePoints::GetInstance()
{
   if (s_lifeChoicePoints == NULL)
   {
	   s_lifeChoicePoints = new LifeChoicePoints();
   }
   return *s_lifeChoicePoints;
}

void LifeChoicePoints::Destroy()
{
	delete s_lifeChoicePoints;
	s_lifeChoicePoints = NULL;
}

LifeChoicePoints::LifeChoicePoints() : 
   m_maxAvailablePoints(0), 
   m_availablePoints(m_maxAvailablePoints),
   m_lifeChoices(),
   m_choiceOrganizer()
{   
}
LifeChoicePoints::~LifeChoicePoints()
{   
   delete m_choiceOrganizer;
   LifeChoiceMap::iterator iter = m_lifeChoices.begin();
	while( iter != m_lifeChoices.end() )
	{
		delete (iter->second);
		++iter;
	}
	m_lifeChoices.clear();
   
}

unsigned int 
LifeChoicePoints::getMaxPoints() const
{
	return m_maxAvailablePoints;
}
unsigned int 
LifeChoicePoints::getAvailablePoints() const 
{
   return m_availablePoints;
}
void 
LifeChoicePoints::setAvailablePoints(unsigned int value) 
{ 
   m_maxAvailablePoints = m_availablePoints = value;
}
void 
LifeChoicePoints::decrementAvailablePoints() 
{
   if(m_availablePoints >0) 
      --m_availablePoints; 
}
void 
LifeChoicePoints::incrementAvailablePoints() 
{
   if(m_availablePoints < m_maxAvailablePoints) 
      ++m_availablePoints; 
}

void
LifeChoicePoints::finalizePoints()
{
   LifeChoiceMap::iterator iter = m_lifeChoices.begin();
   while( iter != m_lifeChoices.end() )
   {
      iter->second->finalizePoints();
      ++iter;
   }
}

void LifeChoicePoints::CreateAllLifeChoices()
{
	// HAUKAP - render location should be handled in different way... what if we want to move them easily?
	float width = 3.0f, height = 4.0f;
	Color3f color(0, 0, 0);

   unsigned int sz = NumLifeChoices;
   for( unsigned int i=0; i<sz; ++i )
   {
      thePointTracker.CreateLifeChoice(Vector2(0.0f,0.0f), width, height, 1, color, m_lifeChoiceStrings[i]);
	}		
}

void 
LifeChoicePoints::CreateLifeChoice(Vector2 position, 
                                   float iconWidth, float iconHeight,
                                   float bttnSize,
                                   Color3f color,
                                   String description)
{
	LifeChoice *lifechoice = new LifeChoice(position.X, position.Y, iconWidth, iconHeight,
                                           1, color, description );
	m_lifeChoices[ description ] = lifechoice;
}

void 
LifeChoicePoints::ZeroOutPointsInLifeChoices()
{
	LifeChoiceMap::iterator iter = m_lifeChoices.begin();
	while( iter != m_lifeChoices.end() )
	{
		iter->second->SetPoints(0);
		++iter;
	}
}

void 
LifeChoicePoints::AddLifeChoicesToWorld(const std::vector<String>& choiceStrings)
{
	unsigned int sz = (unsigned int)choiceStrings.size();
   if(sz > 0)
   {
      std::vector<LifeChoice*> choices;
      unsigned int sz = (unsigned int)choiceStrings.size();
      for( unsigned int i=0; i<sz; ++i )
         choices.push_back( m_lifeChoices[choiceStrings[i]] );

      float startY = 7.0f;
      float padding = 0.5f;
      m_choiceOrganizer = new ChoiceOrganizer(choices, startY, padding);
   }
}
void 
LifeChoicePoints::RemoveLifeChoicesFromWorld()
{
   delete m_choiceOrganizer;
   m_choiceOrganizer = NULL;
}

void 
LifeChoicePoints::addUpkeepToLifeChoice(NameValuePair choiceUpkeep)
{
	unsigned int currentPoints = m_lifeChoices[choiceUpkeep.name]->GetPoints();
	if( (int)currentPoints + choiceUpkeep.value >= 0 )
		m_lifeChoices[choiceUpkeep.name]->SetPoints( currentPoints + choiceUpkeep.value);
}

unsigned int 
LifeChoicePoints::getPointsInLifeChoice(String str) const
{
	LifeChoicePoints* hack = const_cast<LifeChoicePoints*>(this);
	return hack->m_lifeChoices[str]->GetPoints();
}

LifeChoicePoints::ChoiceOrganizer::ChoiceOrganizer(std::vector<LifeChoice*> choices,
                                                   float startY,
                                                   float padding) :
   m_activeChoices(choices),      
   m_startY(7.0f),
   m_padding(0.5f)
{
   addObjects();
}
void
LifeChoicePoints::ChoiceOrganizer::addObjects()
{
	unsigned int sz = (unsigned int)m_activeChoices.size();
	for( unsigned int i=0; i<sz; ++i )
   {
		m_activeChoices[i]->AddToWorld();
   }

   float lifeChoiceWidth = 3.0f; /*HAUKAP - get this width from somewhere...*/
   float totalWidth = lifeChoiceWidth  * sz + m_padding * (sz-1) - lifeChoiceWidth;
   float x = -(totalWidth * 0.5f);
   float y = m_startY;

	for( unsigned int i=0; i<sz; ++i )
   {      
      m_activeChoices[i]->SetPosition(x,y);
      x += lifeChoiceWidth + m_padding;
   }  
}
void
LifeChoicePoints::ChoiceOrganizer::removeObjects()
{
	unsigned int sz = (unsigned int)m_activeChoices.size();
	for( unsigned int i=0; i<sz; ++i )
		m_activeChoices[i]->RemoveFromWorld();
   m_activeChoices.clear();
}