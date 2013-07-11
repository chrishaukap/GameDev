
//AdventurerGame is a singleton and inherits from:
//  GameManager -- so we receive update and render calls, plus collision notifications
#define theGame AdventurerGame::GetInstance()

class AdventurerGame : public GameManager
{
public:
   enum ScreenType
   {
      Invalid,
      WorldView,
      DetailView
   };

	void gotoWorldView();
	void gotoDetailView();

	static AdventurerGame &GetInstance();
   void Init(unsigned int screenWidth, unsigned int screenHeight);

   virtual void Update(float dt);
	virtual void Render();

	virtual void ReceiveMessage(Message* message);
	
protected:
	AdventurerGame();
   ~AdventurerGame();
	static AdventurerGame *s_AdventurerGame;

private:
   class WorldScreen* m_worldScreen;
   class DetailScreen* m_detailScreen;
   ScreenType m_screen;
};
