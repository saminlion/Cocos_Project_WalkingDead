#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class Character
{
private:
	ValueVector wormsPs;
	ValueVector crapsPs;
	Sprite* initPositionSprite;

public:
	vector<Vec2> _wormPositions;
	vector<Vec2> _crapPositions;
	vector<Vec2> _spawnPositions;
	Character();
	void switchingAction(Sprite* actionSprite, Action* newAction, bool repeat);
	Animation* createAnimationSpriteSheet(Texture2D* texture, int numberOflineSprite, int numberOfSprites, float delay, float width, float height);
	Animation* createAnimationMultiSprite(const char* spriteLocation, int numberOfSprites, float delay);
	Animation* createAnimationPlist(const std::string plist, const char* spriteName, int numberOfSprites, float delay);

	void ClearOnce();

	vector<Sprite*> setWormPosition();
	void getwormspawnPointPositions(TMXObjectGroup* objects);

	vector<Sprite*> setCrapPosition();
	void getcrapspawnPointPositions(TMXObjectGroup* objects);

	vector<Sprite*> wormPositions;
	vector<Sprite*> crapPositions;

	int enemyPositionSize;

};

#endif // !__CHARACTER_H__
