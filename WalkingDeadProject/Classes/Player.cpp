#include "Player.h"

USING_NS_CC;

Player::Player()
{

}

void Player::Attack(bool isTransformed, Sprite* effect, b2Body* effectBody, float chargeCount)
{
	effectBody->SetActive(true);
	
	if (chargeCount < 5.0f)
	{
		if (!isTransformed)
		{
			playerAction(UNATTACK);
		}

		else
		{
			playerAction(TNATTACK);
		}
	}

	else
	{
		if (!isTransformed)
		{
			playerAction(UCATTACK);
		}

		else
		{
			playerAction(TCATTACK);
		}
	}

	effect->setPosition(Vec2(playerPos.x + 90, playerPos.y + 30));
	effect->setScale(1.5f);
}

void Player::Jump(bool isTransformed)
{
	if (!isTransformed)
	{
		playerAction(UJUMP);

		auto jumpUp = MoveBy::create(0.5f, Vec2(0, 50));
		auto jumpDown = jumpUp->reverse();
		jumpAction = Sequence::create(jumpUp, jumpDown, CallFunc::create(CC_CALLBACK_0(Player::playerAction, this, UWALK)), nullptr);
		playerSprite->runAction(jumpAction);
	}

	else
	{
		playerAction(TJUMP);

		auto jumpUp = MoveBy::create(0.5f, Vec2(0, 50));
		auto jumpDown = jumpUp->reverse();
		jumpAction = Sequence::create(jumpUp, jumpDown, CallFunc::create(CC_CALLBACK_0(Player::playerAction, this, TWALK)), nullptr);
		playerSprite->runAction(jumpAction);
	}
}

void Player::JumpAttack(bool isTransformed, Sprite* effect, b2Body* effectBody, float chargeCount)
{
	effectBody->SetActive(true);

	if (!isTransformed)
	{
		playerAction(UJUMP);

		auto jumpUp = MoveBy::create(1.f, Vec2(0, 50));
		auto jumpDown = jumpUp->reverse();
		jumpAction = Sequence::create(jumpUp, jumpDown, CallFunc::create(CC_CALLBACK_0(Player::playerAction, this, UWALK)), nullptr);
		playerSprite->runAction(jumpAction);
	}

	else
	{
		playerAction(TJUMP);

		auto jumpUp = MoveBy::create(1.f, Vec2(0, 50));
		auto jumpDown = jumpUp->reverse();
		jumpAction = Sequence::create(jumpUp, jumpDown, CallFunc::create(CC_CALLBACK_0(Player::playerAction, this, TWALK)), nullptr);
		playerSprite->runAction(jumpAction);
		playerAction(UNATTACK);
	}

	effect->setPosition(Vec2(playerPos.x + 90, playerPos.y + 30));
	effect->setScale(1.5f);
}

void Player::playerAction(int actionNumber)
{
	//playerSprite->stopAllActions();

	switch (actionNumber)
	{
	case UWALK:
		playerAnimation = Character::createAnimationMultiSprite("images/HI/HI_Walk_%d.png", 8, 0.3f);
		playerAnimate = Animate::create(playerAnimation);
		playerAnimate->setTag(UWALK);
		Character::switchingAction(playerSprite, playerAnimate, true);
		break;

	case TWALK:
		playerAnimation = Character::createAnimationMultiSprite("images/HI2/HI2_Walk_%d.png", 8, 0.3f);
		playerAnimate = Animate::create(playerAnimation);
		playerAnimate->setTag(TWALK);
		Character::switchingAction(playerSprite, playerAnimate, true);
		break;

	case UNATTACK:
		playerAnimation = Character::createAnimationMultiSprite("images/HI/HI_Attack_%d.png", 6, 0.1f);
		playerAnimate = Animate::create(playerAnimation);
		playerAnimate->setTag(UNATTACK);
		Character::switchingAction(playerSprite, playerAnimate, false);
		break;

	case TNATTACK:
		playerAnimation = Character::createAnimationMultiSprite("images/HI2/HI2_Attack_%d.png", 7, 0.1f);
		playerAnimate = Animate::create(playerAnimation);
		playerAnimate->setTag(TNATTACK);
		Character::switchingAction(playerSprite, playerAnimate, false);
		break;

	case UCATTACK:
		playerAnimation = Character::createAnimationMultiSprite("images/HI/HI_Charge_%d.png", 6, 0.1f);
		playerAnimate = Animate::create(playerAnimation);
		playerAnimate->setTag(UCATTACK);
		Character::switchingAction(playerSprite, playerAnimate, false);
		break;

	case TCATTACK:
		playerAnimation = Character::createAnimationMultiSprite("images/HI2/HI2_Charge_%d.png", 7, 0.1f);
		playerAnimate = Animate::create(playerAnimation);
		playerAnimate->setTag(TCATTACK);
		Character::switchingAction(playerSprite, playerAnimate, false);
		break;

	case UJUMP:
		playerAnimation = Character::createAnimationMultiSprite("images/HI/HI_Jump_%d.png", 9, 0.3f);
		playerAnimate = Animate::create(playerAnimation);
		playerAnimate->setTag(UJUMP);
		Character::switchingAction(playerSprite, playerAnimate, false);
		break;

	case TJUMP:
		playerAnimation = Character::createAnimationMultiSprite("images/HI2/HI2_Jump_%d.png", 9, 0.3f);
		playerAnimate = Animate::create(playerAnimation);
		playerAnimate->setTag(TJUMP);
		Character::switchingAction(playerSprite, playerAnimate, false);
		break;
	}
}