#pragma once

#include "TheEndGameManager.h"
#include "AgeGroups.h"

#include <map>

struct InteractiveLevel
{
   InteractiveLevel(AgeGroups::Group ageGroup) : m_availableLifeChoices(), m_ageGroup(ageGroup) {}

	AgeGroups::Group m_ageGroup;
   std::vector<String> m_availableLifeChoices;
};


class TextActor;
class LifeChoice;
class AdvanceButton;
class InteractiveScreen : public GameScreen
{
public:
	InteractiveScreen();

	void setLevel(InteractiveLevel *level) {m_level = level;}
	const InteractiveLevel* getLevel() const {return m_level;}

	virtual void Start();
	virtual void Stop();
	virtual void Kill();
	virtual void Update(float dt);

private:
   TextActor *m_t;
   const InteractiveLevel* m_level;
};