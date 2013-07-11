#pragma once

class GameScreen : public Renderable
{
public:
	GameScreen();

	void Start();
	void Stop();
   
   // suspends updating/rendering of objects in this screen.  removes them from Angel world (but doesn't delete them)
   void Pause();

   // resumes updating/rendering of objects in this screen.  adds objects back to Angel world.
   void Resume();

   void Update(float dt);
	virtual void Render();
	
	virtual void ReceiveMessage(Message *message) {}
   bool isStarted() const {return m_isStarted;}
   bool isPaused() const {return m_isPaused;}

protected:
   virtual void doPause() = 0;
   virtual void doResume() = 0;
   virtual void doStart() = 0;
   virtual void doStop() = 0;
   virtual void doUpdate(float dt);

   void AddRenderable(Renderable* renderable, int renderLayer);
   void ClearRenderablesInScreen();

private:
	std::map<Renderable*, int> m_objectLayerMap;
   bool m_isStarted;
   bool m_isPaused;
   void UnsuspendGameObjects();
   void SuspendGameObjects();
};