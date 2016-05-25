#include "Character.h"

#define PTM_RATIO 32

Character::Character()
{
	log("Start Character Class");
	_spawnPositions.clear();
	_wormPositions.clear();
	_crapPositions.clear();
	wormsPs.clear();
	crapsPs.clear();
}

void Character::ClearOnce()
{
	wormPositions.clear();
	setCrapPosition().clear();
	setWormPosition().clear();
	_spawnPositions.clear();
	_wormPositions.clear();
	_crapPositions.clear();
	wormsPs.clear();
	crapsPs.clear();
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

Animation* Character::createAnimationPlist(const std::string plist, const char* spriteName, int numberOfSprites, float delay)
{
	auto cache = SpriteFrameCache::getInstance();
	
	cache->addSpriteFramesWithFile(plist);
	
	Vector<SpriteFrame*> animFrames;

	char str[100] = { 0 };

	for (int i = 1; i < numberOfSprites; i++)
	{
		sprintf(str, spriteName, i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, delay);

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

void Character::getwormspawnPointPositions(TMXObjectGroup* objects)
{
	wormsPs = objects->getObjects();

	for (auto enemySP : wormsPs)
	{
		auto VMobj = enemySP.asValueMap();

		int vx = VMobj["x"].asInt();
		int vy = VMobj["y"].asInt();

		Vec2 initPosition = Vec2(vx, vy);

		_wormPositions.push_back(initPosition);

	}
}

vector<Sprite*> Character::setWormPosition()
{
	for (int i = 0; i < _wormPositions.size(); i++)
	{
		auto vec2 = (Vec2)_wormPositions.at(i);

		initPositionSprite = Sprite::create("images/Dummy.png");

		initPositionSprite->setPosition(vec2.x, vec2.y + 30);

		initPositionSprite->setTag(i);

		wormPositions.push_back(initPositionSprite);
	}

	return wormPositions;
}


void Character::getcrapspawnPointPositions(TMXObjectGroup* objects)
{
	crapsPs = objects->getObjects();

	for (auto enemySP : crapsPs)
	{
		auto VMobj = enemySP.asValueMap();

		int vx = VMobj["x"].asInt();
		int vy = VMobj["y"].asInt();

		Vec2 initPosition = Vec2(vx, vy);

		_crapPositions.push_back(initPosition);
	}
}

vector<Sprite*> Character::setCrapPosition()
{
	for (int i = 0; i < _crapPositions.size(); i++)
	{
		auto vec2 = (Vec2)_crapPositions.at(i);

		initPositionSprite = Sprite::create("images/Dummy.png");

		initPositionSprite->setPosition(vec2.x, vec2.y + 30);

		initPositionSprite->setTag(i);

		crapPositions.push_back(initPositionSprite);
	}

	return crapPositions;
}
