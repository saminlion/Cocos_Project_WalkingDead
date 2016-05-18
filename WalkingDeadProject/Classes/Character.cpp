#include "Character.h"

#define PTM_RATIO 32

Character::Character()
{
	log("Start Character Class");
	_spawnPositions.clear();
	_dragonPositions.clear();
	_birdPositions.clear();
	_dkPositions.clear();
	enemyPositions.clear();
	dragonSPs.clear();
	birdSPs.clear();
	dkSPs.clear();
}

void Character::ClearOnce()
{
	_spawnPositions.clear();
	_dragonPositions.clear();
	_birdPositions.clear();
	_dkPositions.clear();
	enemyPositions.clear();
	dragonSPs.clear();
	birdSPs.clear();
	dkSPs.clear();
}

/**
* Return Sprite createAnimation.
*
* @see createAnimation(Texture2D*, int, int, float, float, float)
*
* @param texture      2D texture
* @param numberOflineSprite	how many sprtes are in one line
* @param numberOfSprites	how many sprtes are in sprite sheet
* @param delay	how long waith between sprite
* @param width	width of sprite in sprite sheet
* @param height	height of sprite in sprite sheet
* @lua NA
*/

Animation* Character::createAnimationSpriteSheet(Texture2D* texture, int numberOflineSprite, int numberOfSprites, float delay, float width, float height)
{
	auto animation = Animation::create();
	animation->setDelayPerUnit(delay);

	for (int i = 0; i < numberOfSprites; i++)
	{
		int colum = i % numberOflineSprite;
		int row = i / numberOflineSprite;

		animation->addSpriteFrameWithTexture(texture, Rect(colum * width, row * height, width, height));
	}

	return animation;
}

/**
* Return Sprite createAnimationMultiSprite.
*
* @see createAnimationMultiSprite(const char*, int, float)
*
* @param spriteLocation      sprite location
* @param numberOfSprites	how many sprtes are in sprite sheet
* @param delay	how long waith between sprite
* @lua NA
*/

Animation* Character::createAnimationMultiSprite(const char* spriteLocation, int numberOfSprites, float delay)
{
	char str[100];

	auto animation = Animation::create();
	animation->setDelayPerUnit(delay);

	for (int i = 1; i < numberOfSprites + 1; i++)
	{
		sprintf(str, spriteLocation, i);

		animation->addSpriteFrameWithFile(str);
	}

	return animation;
}

void Character::switchingAction(Sprite* actionSprite, Action* newAction, bool repeat)
{
	if (!repeat)
	{
		actionSprite->stopAllActions();
		actionSprite->runAction(newAction);
	}
	else if (repeat)
	{
		auto rep = RepeatForever::create((ActionInterval*)newAction);
		actionSprite->runAction(rep);
	}
}

vector<Sprite*> Character::setDragonPosition()
{
	log("Dragon Spawn Point Size : %d", _dragonPositions.size());

	for (int i = 0; i < _dragonPositions.size(); i++)
	{
		auto vec2 = (Vec2)_dragonPositions.at(i);

		initPositionSprite = Sprite::create("images/Dummy.png");

		initPositionSprite->setPosition(vec2.x, vec2.y + 30);

		initPositionSprite->setTag(i);

		dragonPositions.push_back(initPositionSprite);
	}

	return dragonPositions;
}

void Character::getDragonSpawnPointPositions(TMXObjectGroup* objects)
{
	dragonSPs = objects->getObjects();

	for (auto enemySP : dragonSPs)
	{
		auto VMobj = enemySP.asValueMap();

		int vx = VMobj["x"].asInt();
		int vy = VMobj["y"].asInt();

		Vec2 initPosition = Vec2(vx, vy);

		_spawnPositions.push_back(initPosition);
		_dragonPositions.push_back(initPosition);
	}
}

vector<Sprite*> Character::setBirdPosition()
{
	log("Bird Spawn Point Size : %d", _birdPositions.size());

	for (int i = 0; i < _birdPositions.size(); i++)
	{
		auto vec2 = (Vec2)_birdPositions.at(i);

		initPositionSprite = Sprite::create("images/Dummy.png");

		initPositionSprite->setPosition(vec2.x, vec2.y + 30);

		initPositionSprite->setTag(i);

		birdPositions.push_back(initPositionSprite);
	}

	return birdPositions;
}

void Character::getBirdSpawnPointPositions(TMXObjectGroup* objects)
{
	birdSPs = objects->getObjects();

	for (auto enemySP : birdSPs)
	{
		auto VMobj = enemySP.asValueMap();

		int vx = VMobj["x"].asInt();
		int vy = VMobj["y"].asInt();

		Vec2 initPosition = Vec2(vx, vy);

		_birdPositions.push_back(initPosition);
		_spawnPositions.push_back(initPosition);
	}
}

vector<Sprite*> Character::setDKPosition()
{
	for (int i = 0; i < _spawnPositions.size(); i++)
	{
		auto vec2 = (Vec2)_spawnPositions.at(i);

		initPositionSprite = Sprite::create("images/Dummy.png");

		initPositionSprite->setPosition(vec2.x, vec2.y + 30);

		initPositionSprite->setTag(i);

		dkPositions.push_back(initPositionSprite);
	}

	return dkPositions;
}

void Character::getDKSpawnPointPositions(TMXObjectGroup* objects)
{
	dkSPs = objects->getObjects();

	for (auto enemySP : dkSPs)
	{
		auto VMobj = enemySP.asValueMap();

		int vx = VMobj["x"].asInt();
		int vy = VMobj["y"].asInt();

		Vec2 initPosition = Vec2(vx, vy);

		_spawnPositions.push_back(initPosition);
	}
}

vector<Sprite*> Character::setEnemyPosition()
{
	log("SpawnPoints Size : %d", _spawnPositions.size());

	for (int i = 0; i < _spawnPositions.size(); i++)
	{
		auto vec2 = (Vec2)_spawnPositions.at(i);

		initPositionSprite = Sprite::create("images/Dummy.png");

		initPositionSprite->setPosition(vec2.x, vec2.y + 30);

		initPositionSprite->setTag(i);

		enemyPositions.push_back(initPositionSprite);
	}

	return enemyPositions;
}
