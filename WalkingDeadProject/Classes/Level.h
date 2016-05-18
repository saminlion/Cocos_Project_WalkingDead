#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"
#include "Character.h"
#include "Box2D\Box2D.h"
#include "Player.h"

using namespace cocos2d;

enum StageNumber
{
	Stage0 = 0,
	Stage1 = 1,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
	Stage6
};

class Level
{
public:
	Level();

	float backgroud_x;
	int currentLevel = 0;

	Player* player;
	Character* character;

	TMXTiledMap* tmap;
	TMXLayer* background;
	TMXLayer* metainfo;
	TMXLayer* test;

	TMXObjectGroup* playerSpawnPoint;
	TMXObjectGroup* dragonSpawnPoint;
	TMXObjectGroup* birdSpawnPoint;
	TMXObjectGroup* dkSpanwPoint;

	void MoveMap(float f);
	void detectGround(float bx);
	void CreateLevel(int mapNumber);
};

#endif //__LEVEL_H__
