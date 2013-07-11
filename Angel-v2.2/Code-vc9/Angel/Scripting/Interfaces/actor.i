%module(directors="1") angel
%{
#include "../../Actors/Actor.h"
#include "../../Infrastructure/TagCollection.h"
%}

typedef std::set<Actor*>	ActorSet;
typedef std::vector<Actor*>	ActorList;
%template(ActorSet)			std::set<Actor*>;
%template(ActorList)		std::vector<Actor*>;

enum spriteAnimationType
{
	SAT_None,
	SAT_Loop,
	SAT_PingPong,
	SAT_OneShot
};

enum actorDrawShape
{
    ADS_Square,
    ADS_Circle
};

%feature("director") Actor;
class Actor : public Renderable, public MessageListener 
{
   // I'm an actor!
public:
	Actor();
	virtual ~Actor();
	
	void SetSize(float x, float y = -1.f);
	void SetSize(Vector2 newSize);
	const Vector2 GetSize();
	void SetPosition(float x, float y);
	void SetPosition(Vector2 pos);
	const Vector2 GetPosition();
	void SetRotation(float rotation);
	const float GetRotation();
	void SetColor(float r, float g, float b, float a=1.0f);
	void SetColor(Color color);
	Color GetColor();
	void SetAlpha(float newAlpha);
	const float GetAlpha();
	void SetDrawShape( actorDrawShape DrawShape );
	
	void MoveTo(Vector2 newPosition, float duration, bool smooth=false, String onCompletionMessage="");
	void RotateTo(float newRotation, float duration, bool smooth=false, String onCompletionMessage="");
	void ChangeColorTo(Color newColor, float duration, bool smooth=false, String onCompletionMessage="");
	void ChangeSizeTo(Vector2 newSize, float duration, bool smooth=false, String onCompletionMessage="");
	void ChangeSizeTo(float newSize, float duration, bool smooth=false, String onCompletionMessage="");
	
	const int GetSpriteTexture(int frame = 0);
	
	bool SetSprite(String filename, int frame = 0, GLint clampmode = GL_CLAMP, GLint filtermode = GL_LINEAR, bool optional=0);
	void ClearSpriteInfo();
	void LoadSpriteFrames(String firstFilename, GLint clampmode = GL_CLAMP, GLint filtermode = GL_LINEAR);
	void PlaySpriteAnimation(float delay, spriteAnimationType animType = SAT_Loop, int startFrame = -1, int endFrame = -1, const char* _animName = NULL); 
	void SetSpriteFrame(int frame);
	const int GetSpriteFrame();
	bool IsSpriteAnimPlaying();
	void SetUVs(const Vector2 upright, const Vector2 lowleft);
	void GetUVs(Vector2 &upright, Vector2 &lowleft) const;
	
	const bool IsTagged(String tag);
	void Tag(String newTag);
	void Untag(String oldTag);
	const StringSet GetTags();
	
	const String SetName(String newName);
	const String GetName();
	static const Actor* GetNamed(String nameLookup);
	
	virtual void ReceiveMessage(Message* message) {}
	
	void SetLayer(int layerIndex);
	void SetLayer(String layerName);
	
	virtual void Update(float dt);
	
	Actor* GetSelf();
	virtual const String GetClassName();
};




%nodefaultctor TagCollection;
class TagCollection
{
public:
	static TagCollection& GetInstance();
	ActorSet GetObjectsTagged(String findTag);
	StringSet GetTagList();
};
