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

	std::vector<Sprite*> CreateDragon(std::vector<Sprite*> positions);
	Vector<Sprite*> CreateBird();
	Vector<Sprite*> CreateDK();
	
	vector<Sprite*> dragonSprites;

	Sprite* fireSprite;
	Sprite* dragonSprite;

	Vector<Sprite*> birdSprites;
	Vector<Sprite*> dkSprites;

	Animation* dragonAnimation;
	Animate* dragonAnimate;
	Sequence* dragonSequence;

	Animation* birdAnimation;
	Animate* birdAnimate;
	Sequence* birdSequence;

	Animation* dkAnimation;
	Animate* dkAnimate;
	Sequence* dkSequence;


	//Fire

	//void ShootFire(b2Body* fireBody, Sprite* fire);
	//Sprite* CreateFire();

	//Shoot

};

#endif // !__ENEMY_H__
