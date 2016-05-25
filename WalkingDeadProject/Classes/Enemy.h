#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Character.h"
#include "Box2D\Box2D.h"

using namespace cocos2d;
using namespace std;

class Enemy : public Character
{
public:
	Enemy();

	vector<Sprite*> enemySprites;

	std::vector<Sprite*> CreateFirstEnemy(std::vector<Sprite*> positions);
	std::vector<Sprite*> CreateSecondEnemy(std::vector<Sprite*> positions);

	vector<Sprite*> firstEnemySprites;
	std::vector<Sprite*> secondEnemySprites;

	Sprite* fireSprite;
	Sprite* firstEnemySprite;

	Animation* firstEnemyAnimation;
	Animate* firstEnemyAnimate;
	Sequence* wormsequence;

	Animation* secondEnemyAnimation;
	Animate* secondEnemyAnimate;
	Sequence* crapsequence;
	
	//Fire

	//void ShootFire(b2Body* fireBody, Sprite* fire);
	//Sprite* CreateFire();

	//Shoot

};

#endif // !__ENEMY_H__
