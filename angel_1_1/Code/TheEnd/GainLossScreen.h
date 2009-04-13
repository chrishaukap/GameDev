#pragma once

#include "TheEndGameManager.h"
#include "AgeGroups.h"
#include <vector>

class TextActor;
class GainLossScreen : public GameScreen
{
public:
   GainLossScreen(AgeGroups::Group group);

	virtual void Start();
	virtual void Stop();
private:
   typedef std::vector<TextActor*> GainsAndLosses;
   GainsAndLosses m_gainsLosses;
   AgeGroups::Group m_agegroup;
};