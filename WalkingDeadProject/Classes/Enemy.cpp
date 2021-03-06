#include "Enemy.h"

Enemy::Enemy()
{
	log("Enemy Initialize");
}

std::vector<Sprite*> Enemy::CreateFirstEnemy(std::vector<Sprite*> positions)
{
	for (int i = 0; i < positions.size(); i++) //for (int i = 0; i < 10; i++)
	{
		firstEnemySprite = (Sprite*)(positions.at(i));

		firstEnemySprite = Sprite::create("images/Enemy/Worm_1.png");

		firstEnemyAnimation = Character::createAnimationMultiSprite("images/Enemy/Worm_%d.png", 8, 0.2f);

		firstEnemySprite->setTag(i);

		firstEnemySprite->setName("Worm");

		firstEnemyAnimate = Animate::create(firstEnemyAnimation);

		Character::switchingAction(firstEnemySprite, firstEnemyAnimate, true);

		firstEnemySprites.push_back(firstEnemySprite);
	}

	return firstEnemySprites;
}

std::vector<Sprite*> Enemy::CreateSecondEnemy(std::vector<Sprite*> positions)
{
	for (int i = 0; i < positions.size(); i++) //for (int i = 0; i < 10; i++)
	{
		auto secondEnemySprite = Sprite::create("images/Enemy/Crap_1.png");

		secondEnemyAnimation = Character::createAnimationMultiSprite("images/Enemy/Crap_%d.png", 11, 0.2f);

		secondEnemySprite->setTag(i);

		secondEnemySprite->setName("Crap");

		secondEnemySprite->setScale(1.f);

		secondEnemyAnimate = Animate::create(secondEnemyAnimation);

		Character::switchingAction(secondEnemySprite, secondEnemyAnimate, true);

		secondEnemySprites.push_back(secondEnemySprite);
	}

	return secondEnemySprites;
}

