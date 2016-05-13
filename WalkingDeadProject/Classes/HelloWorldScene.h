#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include <GLES-Render.h>
#include "Player.h"
#include "Character.h"
#include "Enemy.h"

#define PTM_RATIO 32
#define COCOS2D_DEBUG 2
#define MAX_TRANSCOUNT 50.f

using namespace cocos2d;

enum _entityCategory {
	FIRE = 0x0001,
	ENEMY = 0x0002,
	PLAYER = 0x0004,
};

class HelloWorld : public cocos2d::LayerColor, public b2ContactListener
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(HelloWorld);

#pragma region InitGame
	float backgroud_x;

	int count;

	////////////////////////////////////////
	Character* character;
	Player* player;
	Enemy* enemy;

	~HelloWorld();
	void MoveMap(float f);
	void detectGround(float bx);

	void tick(float dt);
	bool createBox2dWorld(bool debug);
	void clearSpriteBody(int tag);

	b2Body* addBody(Vec2 point, Size size, b2BodyType bodyType, Sprite* sprite, int tag, uint16 categoryBits, uint16 maskBits);

	Vec2 convertPosition(float px, float cx, float py, float cy);

	////////////////////////////////////////
	GLESDebugDraw* m_debugDraw;
	Size winSize;
	b2World* _world;

	bool bDrag;
	bool isAttack;
	bool isJumped;
	bool countStart;
	bool isTransFormed;
	
	ProgressTimer* progressTimer;

	b2Body *dragBody;
	//b2Body *gbody;
	b2Body* enemyBody;
	vector<b2Body*> enemyBodies;
	vector<b2Body*> gbodies;
	vector<Sprite*> enemyDatas;

	b2Body* effectNormalBody;
	b2Body* effectChargeBody;
	b2Body* effectJumpBody;
	b2Body* projectileBody;

	//Vector<Sprite*> fireSprites;
	//Vector<b2Body*> fireBodies;
	vector<Sprite*> positions;
	vector<Sprite*> fires;
	
	Sprite* fireSprite;
	Sprite* dragon;
	Sprite* position;

	vector<Sprite*> dragons;

	Sprite* effectDummy1;
	Sprite* effectDummy2;
	Sprite* effectDummy3;
	
	Sprite* attackButton;
	Sprite* jumpButton;
	Sprite* transformButton;

	Color3B transButtonColor;

	cocos2d::TMXTiledMap* tmap;
	cocos2d::TMXLayer* background;
	cocos2d::TMXLayer* metainfo;
	cocos2d::TMXLayer* test;

	TMXObjectGroup* playerSpawnPoint;
	TMXObjectGroup* enemySpawnPoint;

	vector<Vec2> positionDummies;
	Vec2 positionDummy;
	b2Vec2 enemyBodyPos;
	Vec2 initPosition;

	float chargeCount = 0.0f;
	float transformCount = MAX_TRANSCOUNT;
	float transbuttonAlpha = 255;
	float inverseTransformCount = 0.f;
	float attackDelay = 2.0f;

	int numberOfFire = 10;
	int vx;
	int vy;
	bool destoryFire;
	int i = 0;

	bool killSuccess;
	float deathScale = 1.f;

	virtual void onEnter();
	virtual void onExit();

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	void AfterAction(float dt);
	void EnemyDeath(float dt);
	void ShootFire(float dt);
	void FireDeath(float dt);
	
	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

protected:
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	cocos2d::CustomCommand _customCommand;
#pragma endregion
	void BeginContact(b2Contact* contact);
};
#endif // __HELLOWORLD_SCENE_H__