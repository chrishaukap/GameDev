#pragma once

#include "StringUtil.h"
#include "utils.h"
#include <map>
#include <vector>

#define thePointTracker LifeChoicePoints::GetInstance()

class LifeChoice;
class LifeChoicePoints
{
public:
   static LifeChoicePoints& GetInstance();	
	static void Destroy();


   LifeChoicePoints(); 
   ~LifeChoicePoints();
      
   // accessors for total number of points
	unsigned int getMaxPoints() const;
   unsigned int getAvailablePoints() const;
   void setAvailablePoints(unsigned int value);
   void decrementAvailablePoints();
   void incrementAvailablePoints();
   void finalizePoints();

   enum LifeChoices { Family, Love, Friendship, Health, Success, Education, Travel, Children, Legacy, NumLifeChoices };
	static const String m_lifeChoiceStrings[];

	// creates all the life choices
	void CreateAllLifeChoices();

   // accessors for life choices
	void RemoveLifeChoicesFromWorld();
	void AddLifeChoicesToWorld(const std::vector<String>& choices);
	void ZeroOutPointsInLifeChoices();
   unsigned int getPointsInLifeChoice(String lifechoice) const;
	void addUpkeepToLifeChoice(NameValuePair choiceUpkeep);
private:
   void CreateLifeChoice( Vector2 position, 
                          float iconWidth, float iconHeight,
                          float bttnSize,
                          Color3f color,
                          String description);

	static LifeChoicePoints* s_lifeChoicePoints;
   unsigned int m_availablePoints, m_maxAvailablePoints;
   
	typedef std::map<String, LifeChoice* > LifeChoiceMap;
   LifeChoiceMap m_lifeChoices;   

   class ChoiceOrganizer
   {
   public:
      ChoiceOrganizer(std::vector<LifeChoice*> choices,
                      float startY,
                      float padding);
      ~ChoiceOrganizer() {removeObjects();}
   private:
      ChoiceOrganizer();

      void addObjects();
      void removeObjects();
      std::vector<LifeChoice*> m_activeChoices;
      float m_startY;
      float m_padding;
   };
   ChoiceOrganizer* m_choiceOrganizer;
   
};


