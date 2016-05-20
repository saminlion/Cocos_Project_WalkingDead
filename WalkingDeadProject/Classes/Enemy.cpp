#include "Enemy.h"

Enemy::Enemy()
{
	log("Enemy Initialize");

	//for (int i = 0; i < Character::setEnemyPosition().size(); i++)
	//{
	//	auto enemySprite = Sprite::create("images/Enemy/Dragon_Anim_1.png");

	//	enemySprite->setScale(1.f);

	//	enemySprite->setTag(i);

	//	log("EnemySprite Check Tag : %d", enemySprite->getTag());

	//	enemySprites.push_back(enemySprite);
	//}
}

std::vector<Sprite*> Enemy::CreateDragon(std::vector<Sprite*> positions)
{
	for (int i = 0; i < positions.size(); i++) //for (int i = 0; i < 10; i++)
	{
		dragonSprite = (Sprite*)(positions.at(i));

		dragonSprite = Sprite::create("images/Enemy/Dragon_Anim_1.png");

		dragonAnimation = Character::createAnimationMultiSprite("images/Enemy/Dragon_Anim_%d.png", 8, 0.2f);

		dragonSprite->setTag(i);

		dragonSprite->setName("Dragon");

		dragonAnimate = Animate::create(dragonAnimation);

		Character::switchingAction(dragonSprite, dragonAnimate, true);

		dragonSprites.push_back(dragonSprite);
	}
	return dragonSprites;
}

std::vector<Sprite*> Enemy::CreateBird(std::vector<Sprite*> positions)
{
	for (int i = 0; i < positions.size(); i++) //for (int i = 0; i < 10; i++)
	{
		auto birdSprite = Sprite::create("images/Enemy/BirdEnemyIdleSprite.png");

		//birdAnimation = Character::createAnimationMultiSprite("images/Enemy/Dragon_Anim_%d.png", 8, 0.2f);

		birdSprite->setTag(i);

		birdSprite->setName("Bird");

		birdSprite->setScale(1.f);

		//birdAnimate = Animate::create(birdAnimation);

		//Character::switchingAction(birdSprite, birdAnimate, true);

		birdSprites.push_back(birdSprite);
	}

	return birdSprites;
}

Vector<Sprite*> Enemy::CreateDK()
{
	for (int i = 0; i < 10; i++)
	{
		auto dkSprite = Sprite::create("images/Enemy/CursedKnight.png");

		//dkAnimation = Character::createAnimationMultiSprite("images/Enemy/Dragon_Anim_%d.png", 8, 0.2f);

		dkSprite->setTag(i);

		dkSprite->setName("DK");

		dkSprite->setScale(1.f);

		//dkAnimate = Animate::create(dkAnimation);

		//Character::switchingAction(dkSprite, dkAnimate, true);

		dkSprites.pushBack(dkSprite);
	}

	return dkSprites;
}



