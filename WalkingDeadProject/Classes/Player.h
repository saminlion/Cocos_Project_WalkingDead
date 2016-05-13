#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Character.h"
#include "Box2D\Box2D.h"

using namespace cocos2d;

class Player : public Character
{
public:

	enum PlayerAction
	{
		UWALK,
		TWALK,
		UNATTACK,
		TNATTACK,
		UCATTACK,
		TCATTACK,
		UJUMP,
		TJUMP
	};

	Player();

	Sprite* effectNormal;
	Sprite* effectCharge;
	Sprite* playerSprite;

	Animation* playerAnimation;
	Animate* playerAnimate;
	Sequence* playerSequence;
	Sequence* jumpAction;
	Sequence* AttackAction;

	Vec2 playerPos;
	
	bool countStart;

	void playerAction(int actionNumber); 
	void Attack(bool isTransformed, Sprite* effect, b2Body* effectBody, float chargeCount);
	void Jump(bool isTransformed);
	void JumpAttack(bool isTransformed, Sprite* effect, b2Body* effectBody, float chargeCount);
};

#endif // !__PLAYER_H__
