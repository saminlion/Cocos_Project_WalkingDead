#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class Character
{
private:
	ValueVector dragonSPs;
	ValueVector birdSPs;
	ValueVector dkSPs;

	Sprite* initPositionSprite;
	vector<Vec2> _spawnPositions;
	vector<Vec2> _dragonPositions;
	vector<Vec2> _birdPositions;
	vector<Vec2> _dkPositions;

public:
	Character();
	void switchingAction(Sprite* actionSprite, Action* newAction, bool repeat);
	Animation* createAnimationSpriteSheet(Texture2D* texture, int numberOflineSprite, int numberOfSprites, float delay, float width, float height);
	Animation* createAnimationMultiSprite(const char* spriteLocation, int numberOfSprites, float delay);

	void ClearOnce();

	vector<Sprite*> setEnemyPosition();

	vector<Sprite*> setDragonPosition();
	void getDragonSpawnPointPositions(TMXObjectGroup* objects);

	vector<Sprite*> setBirdPosition();
	void getBirdSpawnPointPositions(TMXObjectGroup* objects);

	vector<Sprite*> setDKPosition();
	void getDKSpawnPointPositions(TMXObjectGroup* objects);

	vector<Sprite*> dragonPositions;
	vector<Sprite*> birdPositions;
	vector<Sprite*> dkPositions;
	vector<Sprite*> enemyPositions;

	int enemyPositionSize;

};

#endif // !__CHARACTER_H__
