#include "stdafx.h"
#include "DetailScreen.h"
#include "Engine/Hero.h"
#include "Engine/GameWorld.h"

#define UPPER_LEFT_CORNER Vector2(-10,9) 
std::vector<std::string> s_monsterArtList;
static void InitMonsterArtList()
{
   s_monsterArtList.push_back(std::string("Resources/Images/monster.png"));
   s_monsterArtList.push_back(std::string("Resources/Images/monster1.png"));
   s_monsterArtList.push_back(std::string("Resources/Images/monster2.png"));
   s_monsterArtList.push_back(std::string("Resources/Images/monster3.png"));
   s_monsterArtList.push_back(std::string("Resources/Images/monster4.png"));
   s_monsterArtList.push_back(std::string("Resources/Images/monster5.png"));
   s_monsterArtList.push_back(std::string("Resources/Images/monster6.png"));
}
std::vector<std::string> s_treeArtList;
static void InitTreeArtList()
{
   s_treeArtList.push_back(std::string("Resources/Images/tree1.png"));
   s_treeArtList.push_back(std::string("Resources/Images/tree2.png"));
   s_treeArtList.push_back(std::string("Resources/Images/tree3.png"));
   s_treeArtList.push_back(std::string("Resources/Images/tree4.png"));
}

DetailScreen::DetailScreen(unsigned int screenWidth, unsigned int screenHeight) :
   m_actors()
{
   m_screenWidth = screenWidth;
   m_screenHeight = screenHeight;
}
DetailScreen::~DetailScreen()
{

}

void DetailScreen::doStop()
{
   size_t sz = m_actors.size();
   for( size_t i=0; i<sz; ++i)
      delete m_actors[i];
   m_actors.clear();
}
static Vector3 s_CameraPosition;
static std::vector<Actor*> s_backgroundElements;
void DetailScreen::doStart()
{
   InitMonsterArtList();
   InitTreeArtList();
   
   s_CameraPosition = Vector3(0,0,10.0);
   doResume();
}

void DetailScreen::doPause()
{
   ClearRenderablesInScreen();

   size_t sz = m_actors.size();
   for( size_t i=0; i<sz; ++i)
      m_actors[i]->Destroy();
   
   sz = s_backgroundElements.size();
   for( size_t i=0; i<sz; ++i)
      s_backgroundElements[i]->Destroy();

   s_CameraPosition = theCamera.GetPosition3();
}

void DetailScreen::RenderCombatScene()
{
   Actor* monster = new Actor;
   monster->SetName("monster");
   monster->SetPosition(4,0);
   monster->SetSize(1.0f);
   monster->SetColor(1,1,1,1);

   int rand = MathUtil::RandomIntInRange(0, s_monsterArtList.size());
   monster->SetSprite(s_monsterArtList[rand]);
   AddRenderable(monster, 2);
   m_actors.push_back(monster);

   Actor* heroActor = new Actor;
   const Hero& hero = theGameWorld.hero();
   switch(hero.state())
   {
      case Hero::Hunting:
      case Hero::Camping:
         {
            Actor* campfire = new Actor;
            campfire->SetName("campfire");
            campfire->SetPosition(2,0);
            campfire->SetSize(1.0f);
            campfire->SetColor(1,1,1,1);
            campfire->SetSprite("Resources/Images/fire.png");
            AddRenderable(campfire, 2);
            m_actors.push_back(campfire);  
         }
      case Hero::Travel:
      case Hero::ArcaneTraining: 
         {
            Actor* heroActor = new Actor;
            heroActor->SetName("heroRender");
            heroActor->SetPosition(2,2);
            heroActor->SetSize(1.0f);
            heroActor->SetColor(1,1,1,1);
            heroActor->SetSprite("Resources/Images/combat.png");
            AddRenderable(heroActor, 2);
            m_actors.push_back(heroActor);            
         }
         break;
      case Hero::ArcheryTraining:
      case Hero::MartialTraining:
      case Hero::Researching:
      case Hero::Resting:
      default: assert(!" Unknown HeroState");
   }
}



void DetailScreen::RenderBackGround()
{   
   for(int i=0; i< 20; ++i)
   {
      float spacer = MathUtil::RandomFloatInRange(0, 5.0f);

      Actor* tree = new Actor;
      tree->SetPosition( -i * 7 + spacer,0);
      tree->SetSize(5.0f);
      tree->SetColor(1,1,1,1);
      tree->SetSprite(s_treeArtList[MathUtil::RandomIntInRange(0,s_treeArtList.size())]);
      AddRenderable(tree, 0);
      s_backgroundElements.push_back(tree);
   }
}
static TextActor* s_actor = NULL;
void DetailScreen::doResume()
{
   // cleanup actors list first
   {
      size_t sz = m_actors.size();
      for( size_t i=0; i<sz; ++i)
         delete m_actors[i];
      m_actors.clear();
      
      sz = s_backgroundElements.size();
      for( size_t i=0; i<sz; ++i)
         delete s_backgroundElements[i];
      s_backgroundElements.clear();
   }

   theCamera.SetPosition(s_CameraPosition);
   const Hero& hero = theGameWorld.hero();
   
   TextActor* actor = new TextActor("Console", "");
   actor->SetSize(1.0f);
   actor->SetColor(0,1,0);
   actor->SetPosition(UPPER_LEFT_CORNER);
   actor->SetDisplayString("blah");
   AddRenderable(actor,10);
   m_actors.push_back(actor);
   s_actor = actor;

   RenderBackGround();

   if(hero.inCombat())
   {      
      RenderCombatScene();
   }
   else
      switch(hero.state())
      {
      case Hero::ArcaneTraining: break;
      case Hero::ArcheryTraining: break;
      case Hero::MartialTraining: break;
      case Hero::Camping:
         {
            Actor* campfire = new Actor;
            campfire->SetName("campfire");
            campfire->SetPosition(2,0);
            campfire->SetSize(1.0f);
            campfire->SetColor(1,1,1,1);
            campfire->SetSprite("Resources/Images/fire.png");
            AddRenderable(campfire, 2);
            m_actors.push_back(campfire);

            Actor* heroActor = new Actor;
            heroActor->SetName("heroRender");
            heroActor->SetPosition(2,2);
            heroActor->SetSize(1.0f);
            heroActor->SetColor(1,1,1,1);
            heroActor->SetSprite("Resources/Images/hunting.png");
            AddRenderable(heroActor, 2);
            m_actors.push_back(heroActor);
         }
         break;
      case Hero::Hunting: 
         {
            Actor* heroActor = new Actor;
            heroActor->SetName("heroRender");
            heroActor->SetPosition(2,2);
            heroActor->SetSize(1.0f);
            heroActor->SetColor(1,1,1,1);
            heroActor->SetSprite("Resources/Images/hunting.png");
            AddRenderable(heroActor, 2);
            m_actors.push_back(heroActor);
         }
      case Hero::Researching: break;
      case Hero::Resting: break;
      case Hero::Travel: break;
      default: assert(!" Unknown HeroState");
      }   
}
void DetailScreen::UpdateDebug()
{
   const Hero& hero = theGameWorld.hero();
   s_actor->SetDisplayString(hero.stateToString());
}

void DetailScreen::doUpdate(float dt)
{
   const float scrollRate = 0.01f;
   size_t sz = s_backgroundElements.size();
   for(int i=0; i<sz; ++i)
      s_backgroundElements[i]->SetPosition(
          s_backgroundElements[i]->GetPosition().X + scrollRate,
          s_backgroundElements[i]->GetPosition().Y);
   UpdateDebug();
}

