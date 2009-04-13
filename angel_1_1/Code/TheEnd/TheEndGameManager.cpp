#include "StdAfx.h"
#include "TheEndGameManager.h"

#include "World.h"
#include "TextRendering.h"
#include "LifeChoicePoints.h"

#include "GainLossScreen.h"
#include "InteractiveScreen.h"
#include "CutSceneScreen.h"
#include "AgeGroups.h"

static void InitializeLevels();
static void PopulateLevelWithLifeChoices(InteractiveLevel& level);
static std::vector<InteractiveLevel> levels;

GameScreen::GameScreen() {}

void GameScreen::Start(){}

void GameScreen::Stop()
{
	std::vector<Renderable*>::iterator it = m_objects.begin();
	while(m_objects.end() != it)
	{
		theWorld.Remove(*it);
		it++;
	}
}
void GameScreen::Kill()
{
   Stop();
	std::vector<Renderable*>::iterator it = m_objects.begin();
	while(m_objects.end() != it)
	{
		delete *it;
		it++;
	}
	m_objects.clear();
}

void GameScreen::Update(float dt) {}
void GameScreen::Render() {}


TheEndGameManager* TheEndGameManager::s_TheEndGameManager = NULL;

TheEndGameManager::TheEndGameManager()
{
	CONSOLE_DECLARECMDMETHOD(MoveForwards, this, TheEndGameManager, MoveForwards);
	CONSOLE_DECLARECMDMETHOD(MoveBackwards, this, TheEndGameManager, MoveBackwards);

   InitializeLevels();
	thePointTracker.CreateAllLifeChoices();


	m_screens.push_back(new CutSceneScreen());    

   unsigned int sz = (unsigned int)levels.size();
   for(unsigned int i=0; i<sz; ++i )
   {
		InteractiveScreen *screen = new InteractiveScreen;
		screen->setLevel( &(levels[i]) );
      m_screens.push_back(screen);		

      m_screens.push_back(new GainLossScreen( levels[i].m_ageGroup ) );
   }

   theWorld.Add(m_screens[CutScene]);
   m_screens[CutScene]->Start();
	m_currentScreen = CutScene;
}

TheEndGameManager& TheEndGameManager::getInstance()
{
	if (s_TheEndGameManager == NULL)
	{
		s_TheEndGameManager = new TheEndGameManager();
	}
	return *s_TheEndGameManager;
}

void TheEndGameManager::MoveForwards(const String& input)
{
   unsigned int numScreens = (unsigned int)m_screens.size();
	if ((m_currentScreen >= 0) && (m_currentScreen < (numScreens-1)))
	{
		m_screens[m_currentScreen]->Stop();
		theWorld.Remove(m_screens[m_currentScreen]);

      ++m_currentScreen;
		m_screens[ m_currentScreen ]->Start();
		theWorld.Add(m_screens[m_currentScreen]);
	}
}

void TheEndGameManager::MoveBackwards(const String& input)
{
   unsigned int numScreens = (unsigned int)m_screens.size();
	if (m_currentScreen > numScreens)
	{
		m_screens[m_currentScreen]->Stop();
		theWorld.Remove(m_screens[m_currentScreen]);

      --m_currentScreen;
		m_screens[ m_currentScreen ]->Start();
		theWorld.Add(m_screens[m_currentScreen]);
	}
}

void TheEndGameManager::Render()
{

	glColor3f(0.5f, 0.5f, 0.5f);
	char* infoString = "";
	int xOffset = 900;
	DrawGameText(infoString, "ConsoleSmall", xOffset, 763);
}

void InitializeLevels()
{
   unsigned int sz = AgeGroups::InvalidAgeGroup;
   for( unsigned int i=0; i<sz; ++i )
   {
		levels.push_back( InteractiveLevel( AgeGroups::Group(i) ) );
      PopulateLevelWithLifeChoices( levels[i] );
   }
}

void PopulateLevelWithLifeChoices(InteractiveLevel& level)
{
   // populate first tier of Life Choices (always available)
   level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Family ] );
   level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Friendship ] );
   level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Education ] );
   
   if( (unsigned int)level.m_ageGroup >= (unsigned int)AgeGroups::Ages15to20 )
   {
      level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Children ] );
      level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Health] );
      level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Love] );
      level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Success] );
      level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Travel] );
   }

   if( (unsigned int)level.m_ageGroup >= (unsigned int)AgeGroups::Ages30to35)
      level.m_availableLifeChoices.push_back( thePointTracker.m_lifeChoiceStrings[ thePointTracker.Legacy ] );
      
}