#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class Character
{
private:
	ValueVector enemySPs;
	Sprite* initPositionSprite;
	vector<Vec2> _spawnPositions;

public:
	void switchingAction(Sprite* actionSprite, Action* newAction, bool repeat);
	Animation* createAnimationSpriteSheet(Texture2D* texture, int numberOflineSprite, int numberOfSprites, float delay, float width, float height);
	Animation* createAnimationMultiSprite(const char* spriteLocation, int numberOfSprites, float delay);
	vector<Sprite*> setEnemyPosition();
	void getEnemySpawnPointPositions(TMXObjectGroup* objects);
	vector<Sprite*> positions;
};

#endif // !__CHARACTER_H__
