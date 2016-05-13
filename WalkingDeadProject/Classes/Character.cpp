#include "Character.h"

#define PTM_RATIO 32

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

	////int i != newAction->getTag();
	//auto prevAction = actionSprite->getActionManager();

	//for (int i = 0; i < actionSprite->getNumberOfRunningActions(); i++)
	//{
	//	auto prevAction = actionSprite->getActionByTag(i);

	//	bool checkAction = prevAction->isDone();

	//	if (i != newAction->getTag())
	//	{
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
	//	}
	//}
}

void Character::getEnemySpawnPointPositions(TMXObjectGroup* objects)
{
	enemySPs = objects->getObjects();

	for (auto enemySP : enemySPs)
	{
		auto VMobj = enemySP.asValueMap();

		int vx = VMobj["x"].asInt();
		int vy = VMobj["y"].asInt();

		Vec2 initPosition = Vec2(vx, vy);

		_spawnPositions.push_back(initPosition);

		log("Enemy Pos : %f ...... %f", initPosition.x, initPosition.y);
	}
}

vector<Sprite*> Character::setEnemyPosition()
{
	for (int i = 0; i < _spawnPositions.size(); i++)
	{
		auto vec2 = (Vec2)_spawnPositions.at(i);

		initPositionSprite = Sprite::create("images/Dummy.png");

		initPositionSprite->setPosition(vec2.x, vec2.y + 30);

		log("Set Enemy Position : %f ..... %f", vec2.x, vec2.y);

		initPositionSprite->setTag(i);

		positions.push_back(initPositionSprite);

		log("Check Tag : %d", initPositionSprite->getTag());
	}

	return positions;
}
