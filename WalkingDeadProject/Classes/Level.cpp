#include "Level.h"

Level::Level()
{
	CreateLevel(StageNumber::Stage1);
}

void Level::CreateLevel(int mapNumber)
{
	currentLevel = mapNumber;

	char str[100];

	sprintf(str, "tile/Platform%d.tmx", mapNumber);

	tmap = TMXTiledMap::create(str);

	background = tmap->getLayer("Background");

	playerSpawnPoint = tmap->getObjectGroup("Objects");

	dragonSpawnPoint = tmap->getObjectGroup("DragonSpawnPoints");

	birdSpawnPoint = tmap->getObjectGroup("BirdSpawnPoints");

	character->getDragonSpawnPointPositions(dragonSpawnPoint);

	character->getBirdSpawnPointPositions(birdSpawnPoint);
}

void Level::MoveMap(float f)
{
	if (backgroud_x == -1000)
	{
		CreateLevel(currentLevel + 1);
		//backgroud_x = 0;
		//tmap->setPosition(Vec2(backgroud_x, 0));
	}

	backgroud_x -= 2;
	tmap->setPosition(Vec2(backgroud_x, 0));
	detectGround(backgroud_x);
}

void Level::detectGround(float bx)
{
	int x = ((abs(bx) + player->playerSprite->getPosition().x) / tmap->getTileSize().width);

	int tileGid = this->background->getTileGIDAt(Vec2(x, 8));

	Value properties = tmap->getPropertiesForGID(tileGid);

	if (properties.isNull())
	{
		//log("Detected cliff : %d", count++);
	}
}