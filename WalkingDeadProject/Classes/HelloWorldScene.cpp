#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();

	auto layer = HelloWorld::create();

	scene->addChild(layer);

	return scene;
}

bool HelloWorld::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}

#pragma region Init Default
	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(HelloWorld::tick), 1 / 60.f);
	}

	character = new Character();
	player = new Player();
	enemy = new Enemy();

	bodyData = new BodyUserData;

	killSuccess = false;
	isTransFormed = false;
	destoryFire = false;

	winSize = Director::getInstance()->getWinSize();

	count = 0;
	
	tmap = CreateTmap(StageNumber::Stage0);
	tmap->setAnchorPoint(Vec2::ZERO);
	this->addChild(tmap, -1, 11);
	SetTmap(1, true);

	tmap2 = CreateTmap(currentLevel + 1);
	tmap2->setPosition(tmap->getContentSize().width, 0);
	tmap2->setAnchorPoint(Vec2::ZERO);
	this->addChild(tmap2, -1, 11);
	SetTmap(2, false);

	attackButton = Sprite::create("images/UI/AttackButton.png");
	attackButton->setPosition(440, 20);
	this->addChild(attackButton, 2, 41);

	jumpButton = Sprite::create("images/UI/JumpButton.png");
	jumpButton->setPosition(30, 20);
	this->addChild(jumpButton, 2, 42);

	transformButton = Sprite::create("images/UI/TransformButton.png");
	transformButton->setPosition(30, 300);
	transButtonColor = Color3B(transformButton->getColor().r, transformButton->getColor().g, transformButton->getColor().b);
	transformButton->setColor(transButtonColor);
	this->addChild(transformButton, 2, 43);

	progressTimer = ProgressTimer::create(Sprite::create("images/UI/Button.png"));
	progressTimer->setPosition(transformButton->getPositionX(), transformButton->getPositionY());
	progressTimer->setPercentage(0);
	this->addChild(progressTimer, 100);

#pragma endregion

#pragma region Init Player Character
	CreatePlayerEffectBody();

#pragma endregion
		
#pragma region Init Enemy Character

#pragma endregion

	//if (this->createBox2dWorld(true))
	//{
	//	//SetTmap(tmaps, true);
	//	SetTmap(tmap, true);
	//	SetTmap(tmap2, false);

	//	this->schedule(schedule_selector(HelloWorld::tick), 1 / 60.f);
	//}

	this->schedule(schedule_selector(HelloWorld::MoveMap));
	//this->schedule(schedule_selector(HelloWorld::ShootFire), 1.0f);

	return true;
}

TMXTiledMap* HelloWorld::CreateTmap(int mapNumber)
{
	log("Create Tmap");
	currentLevel = mapNumber;

	char str[100];

	sprintf(str, "tile/Platform%d.tmx", mapNumber);

	auto newTileMap = TMXTiledMap::create(str);

	background = newTileMap->getLayer("Background");

	if (mapNumber == Stage0)
	{
		auto playerSpawnPoint = newTileMap->getObjectGroup("Objects");

		ValueMap spawnPoint = playerSpawnPoint->getObject("SpawnPoint");

		int x = spawnPoint["x"].asInt();
		int y = spawnPoint["y"].asInt();

		player->playerSprite = Sprite::create("images/HI/HI_Walk_1.png");

		player->playerSprite->setPosition(Vec2(x, y));
		player->playerSprite->setAnchorPoint(Vec2::ZERO);
		player->playerSprite->setScale(0.15f);
		this->addChild(player->playerSprite, 2);

		player->playerAction(player->UWALK);
	}

	wormPositions.clear();

	crapPositions.clear();

	log("worms Postion Size : %d", wormPositions.size());
	log("craps Postion Size : %d", crapPositions.size());

	character->_wormPositions.clear();
	character->_crapPositions.clear();
	character->_dkPositions.clear();

	character->wormPositions.clear();
	character->crapPositions.clear();

	log("_worms Postion Size : %d", character->_wormPositions.size());
	log("_craps Postion Size : %d", character->_crapPositions.size());
	
	//dkPositions.clear();

	auto wormspawnPoint = newTileMap->getObjectGroup("WormSpawnPoints");

	auto crapspawnPoint = newTileMap->getObjectGroup("CrapSpawnPoints");
	
	character->getwormspawnPointPositions(wormspawnPoint);

	character->getcrapspawnPointPositions(crapspawnPoint);
		
	return newTileMap;
}

void HelloWorld::SetTmap(int tileMapNumber, bool firstTime)
{
	wormPositions = character->setWormPosition();

	crapPositions = character->setCrapPosition();

	log("SpawnPoint");

	log("_worms Postion Size : %d", character->_wormPositions.size());
	log("_craps Postion Size : %d", character->_crapPositions.size());

	log("worms Postion Size : %d", wormPositions.size());
	log("craps Postion Size : %d", crapPositions.size());

	for (int j = 0; j < wormPositions.size() + crapPositions.size(); j++)
	{
		if (j < wormPositions.size())
		{
			position = (Sprite*)(wormPositions.at(j));

			log("%d", j);

			if (tileMapNumber == 1)
			{
				tmap->addChild(position);
			}
			else
			{
				tmap2->addChild(position);
			}

			positionDummy = Vec2(position->convertToWorldSpace(this->getPosition()).x, position->convertToWorldSpace(this->getPosition()).y);

			positions.push_back(position);

			positionDummies.push_back(positionDummy);

			//log("Position Dummies Size : %d", positionDummies.size());
		}

		if (j >= wormPositions.size() && j < wormPositions.size() + crapPositions.size())
		{
			position = (Sprite*)crapPositions.at(j - wormPositions.size());

			log("%d", j);

			if (tileMapNumber == 1)
			{
				tmap->addChild(position);
			}
			else
			{
				tmap2->addChild(position);
			}

			positionDummy = Vec2(position->convertToWorldSpace(this->getPosition()).x, position->convertToWorldSpace(this->getPosition()).y);
		
			positions.push_back(position);

			positionDummies.push_back(positionDummy);
		}
	}

	log("Position Dummies Size : %d", positionDummies.size());

	this->SpawnEnemy();
}

//void HelloWorld::DestoryTileMap()
//{
//	for (b2Body* b = _world->GetBodyList(); b != enemyBodies.at(4); b = b->GetNext())
//	{
//		if (b->GetUserData() != nullptr)
//		{
//			auto spriteData = (Sprite*)b->GetUserData();
//
//			for (int i = 0; i < 4; i++)
//			{
//				//0, 1, 2, 3
//				//4, 5, 6, 7
//
//				log("Check1");
//
//				enemyDatas.swap(i, i + 4);
//
//				std::swap(enemyBodies.at(i), enemyBodies.at(i + 4));
//				std::swap(positions.at(i), positions.at(i + 4));
//				std::swap(positionDummies.at(i), positionDummies.at(i + 4));
//
//				if (spriteData->getName() == "Dragon")
//				{
//					if (spriteData == worms.at(i))
//					{
//						log("%d", i);
//						spriteData->removeFromParent();
//					}
//				}
//
//				if (spriteData->getName() == "Bird" )
//				{
//					log("%d", i);
//					if (spriteData == craps.at(i))
//					{
//						spriteData->removeFromParent();
//					}
//				}
//
//				//spriteData->getName() == "DK"
//				if (b == enemyBodies.at(i))
//				{
//					_world->DestroyBody(b);
//				}
//			}
//			log("Check2");
//
//			worms.erase(worms.begin(), worms.begin() + 2);
//			wormPositions.erase(wormPositions.begin(), wormPositions.begin() + 2);
//			craps.erase(craps.begin(), craps.begin() + 2);
//			crapPositions.erase(crapPositions.begin(), crapPositions.begin() + 2);
//			enemyDatas.erase(enemyDatas.begin() + 4, enemyDatas.end());
//			positions.erase(positions.begin() + 4, positions.end());
//			positionDummies.erase(positionDummies.begin() + 4, positionDummies.end());
//			enemyBodies.erase(enemyBodies.begin() + 4, enemyBodies.end());
//			character->_spawnPositions.erase(character->_spawnPositions.begin(), character->_spawnPositions.begin() + 4);
//
//			log("worms Size : %d", worms.size());
//			log("craps Size : %d", craps.size());
//			log("worms Size : %d", wormPositions.size());
//			log("craps Size : %d", crapPositions.size());
//		}
//		log("Check3");
//		break;
//	}
//}

void HelloWorld::MoveMap(float f)
{
	//log("Tmap X position : %f", tmap->getPositionX());
	//log("Background_x : %f", backgroud_x);

	backgroud_x -= 0.2f;
	
	//log("%f", -480 + tmap->getContentSize().width);
	//log("tmap X Position : %f", tmap->getPositionX());

	if (tmap->getPositionX() == -(480 + tmap->getContentSize().width))
	{
		log("Switch and Change tmap");

		log("tmap Position : %f ... %f", tmap->getPositionX(), tmap->getPositionY());
		log("tmap2 Position : %f ... %f", tmap2->getPositionX(), tmap2->getPositionY());

		//backgroud_x = 0;

		//tmap->setVisible(false);
		//tmap->removeFromParentAndCleanup(true);
		//removeChild(tmap, false);
		//tmap->setPosition(Vec2(tmap2->convertToWorldSpace(this->getPosition()).x + tmap2->getContentSize().width, 0));

		tmap->setPosition(Vec2(tmap2->getPosition().x + tmap2->getContentSize().width, 0));

		log("tmap Position : %f ... %f", tmap->getPositionX(), tmap->getPositionY());

		Vec2 test = Vec2(tmap2->getPosition().x + tmap2->getContentSize().width, 0);

		log("test position : %f ... %f", test.x, test.y);

		//log("tmaps Size : %d", tmaps.size());

		//swap(tmaps.at(0), tmaps.at(1));

		//tmaps.erase(tmaps.begin());

		//this->SpawnEnemy();

		//this->DestoryTileMap();

		//log("worms Postion Size : %d", wormPositions.size());
		//log("craps Postion Size : %d", crapPositions.size());
		//log("Positions Size : %d", positions.size());
		//log("Position Dummies Size : %d", positionDummies.size());
		//log("tmaps Size : %d", tmaps.size());

		//SetTmap(tmaps, false);

		backgroud_x = 0;
	}

	tmap->setPosition(Vec2(backgroud_x + tmap->getPositionX(), 0));
	tmap2->setPosition(Vec2(backgroud_x + tmap->getContentSize().width, 0));

	//detectGround(backgroud_x);

	log("tmap Position : %f ... %f", tmap->getPositionX(), tmap->getPositionY());
	log("tmap2 Position : %f ... %f", tmap2->getPositionX(), tmap2->getPositionY());
}



void HelloWorld::SpawnEnemy()
{
	//vector<Sprite*> dkPositions = character->setDKPosition();

	enemy->firstEnemySprites.clear();
	enemy->secondEnemySprites.clear();

	worms = enemy->CreateFirstEnemy(wormPositions);
	craps = enemy->CreateSecondEnemy(crapPositions);

	log("worms Size : %d", worms.size());
	log("craps Size : %d", craps.size());

	for (int i = 0; i < positionDummies.size(); i++)
	{
		positionDummy = positionDummies.at(i);

		if (i < worms.size())
		{
			log("Dragon Index : %d", i);

			worm = (Sprite*)worms.at(i);

			worm->setPosition(positionDummy.x, positionDummy.y + 30);

			worm->setScale(2.0f);

			auto eBody = this->addBody(positionDummy, worm->getContentSize(), b2_dynamicBody, worm, 4, ENEMY, PLAYER);

			enemyBodies.push_back(eBody);

			enemyDatas.pushBack(worm);
		}

		else if (i >= worms.size() && i < craps.size() + worms.size())
		{
			log("Bird Index : %d", i);

			crap = (Sprite*)craps.at(i - worms.size());

			crap->setPosition(positionDummy.x, positionDummy.y + 30);

			auto eBody = this->addBody(positionDummy, crap->getContentSize(), b2_dynamicBody, crap, 4, ENEMY, PLAYER);

			enemyBodies.push_back(eBody);

			enemyDatas.pushBack(crap);
		}
		//else if (i < dks.size() + craps.size() + worms.size())

	}

	log("Enemybodies : %d", enemyBodies.size());

	log("EnemyDatas : %d", enemyDatas.size());
}

void HelloWorld::ShootFire(float dt)
{
	//log("Shooting fire");

	//fireSprite = Sprite::create("images/Effect/Dragon_Fire_Effect_1.png");
	//fireSprite->setScale(2.0f);

	//projectileBody = this->addBody(positionDummy, fireSprite->getContentSize() / 2, b2_dynamicBody, fireSprite, 3, FIRE, PLAYER);
	//projectileBody->ApplyLinearImpulse(b2Vec2(-1000.f, 0.f), projectileBody->GetWorldCenter(), true);

	//this->unschedule(schedule_selector(HelloWorld::ShootFire));
}

void HelloWorld::EnemyDeath(float dt)
{
	killSuccess = false;

	for (int i = 0; i < enemyDatas.size(); i++)
	{
		auto spriteData = (Sprite*)enemyDatas.at(i);
		auto spriteBody = (b2Body*)enemyBodies.at(i);

		if (spriteData == hitSprite && spriteBody == hitBody)
		{
			enemyDatas.eraseObject(hitSprite, true);

			for (int i = 0; i < enemyBodies.size(); i++)
			{
				if (i == hitIndex)
				{
					enemyBodies.erase(enemyBodies.begin() + i);
					positions.erase(positions.begin() + i);
					positionDummies.erase(positionDummies.begin() + i);
				}
			}

			this->scheduleOnce(schedule_selector(HelloWorld::clearSpriteBody), 0.01f);
		}
	}
}

//void HelloWorld::FireDeath(float dt)
//{
//	log("CallFunc");
//
//	destoryFire = false;
//
//	auto fireAnimation = character->createAnimationMultiSprite("images/Effect/Dragon_Fire_Effect_%d.png", 5, 0.2f);
//	
//	auto fireAnimate = Animate::create(fireAnimation);
//
//	auto fireSeq = Sequence::create(fireAnimate, DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(HelloWorld::clearSpriteBody, this, 21)), nullptr);
//
//	fireSprite->runAction(fireSeq);
//}

void HelloWorld::clearSpriteBody(float dt)
{
	log("Clear Begin");

	for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			auto spriteData = (Sprite*)b->GetUserData();
			
			if (hitSprite != nullptr)
			{
				if (spriteData == hitSprite && b == hitBody && _world->IsLocked() == false)
				{
					this->removeChild(hitSprite, true);
					_world->DestroyBody(hitBody);

					log("Clear Done");

					//this->SpawnEnemy();

					break;
				}
			}
		}
	}
}

void HelloWorld::AfterAction(float dt)
{
	if (isAttack)
	{
		effectNormalBody->SetActive(false);
		effectChargeBody->SetActive(false);
		effectJumpBody->SetActive(false);

		for (auto i = this->getChildren().begin(); i != this->getChildren().end(); i++)
		{
			if (this->getChildByTag(31))
			{
				this->removeChildByTag(31, true);
			}
		}

		if (!isTransFormed)
		{
			player->playerAction(player->UWALK);
		}
		else
		{
			player->playerAction(player->TWALK);
		}

		chargeCount = 0.0f;

		attackButton->setColor(Color3B::WHITE);
	}

	isJumped = false;
	isAttack = false;

	jumpButton->setColor(Color3B::WHITE);

	this->unschedule(schedule_selector(HelloWorld::AfterAction));
}

Vec2 HelloWorld::convertPosition(float px, float cx, float py, float cy)
{
	Vec2 newPos = Vec2((px - cx / 2), (py - cy / 2));

	return newPos;
}

bool HelloWorld::createBox2dWorld(bool debug)
{
	b2Vec2 gravity = b2Vec2(0.0f, -30.0f);

	_world = new b2World(gravity);

	_world->SetAllowSleeping(true);

	_world->SetContinuousPhysics(true);

	if (debug)
	{
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;

		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;

		m_debugDraw->SetFlags(flags);
	}

	bDrag = false;

	_world->SetContactListener(this);

	return true;
}

void HelloWorld::CreatePlayerEffectBody()
{
	effectDummy1 = Sprite::create("images/Dummy.png");
	effectDummy2 = Sprite::create("images/Dummy.png");
	effectDummy3 = Sprite::create("images/Dummy.png");

	player->playerPos = player->playerSprite->convertToWorldSpace(this->getPosition());

	effectDummy1->setPosition(Vec2(player->playerPos.x, player->playerPos.y));
	effectDummy2->setPosition(Vec2(player->playerPos.x, player->playerPos.y));
	effectDummy3->setPosition(Vec2(player->playerPos.x, player->playerPos.y));

	effectNormalBody = this->addBody(player->playerPos, Size(effectDummy1->getContentSize() / 4), b2_staticBody, effectDummy1, 0, PLAYER, ENEMY | FIRE);
	effectChargeBody = this->addBody(player->playerPos, Size(effectDummy2->getContentSize() / 4), b2_staticBody, effectDummy2, 1, PLAYER, ENEMY | FIRE);
	effectJumpBody = this->addBody(player->playerPos, Size(effectDummy3->getContentSize() / 4), b2_staticBody, effectDummy3, 2, PLAYER, ENEMY | FIRE);
}

b2Body* HelloWorld::addBody(Vec2 point, Size size, b2BodyType bodyType, Sprite* sprite, int tag, uint16 categoryBits, uint16 maskBits)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;

	b2Body *body = nullptr;

	if (tag == 0)
	{
		bodyDef.position.Set((point.x + 100) / PTM_RATIO, (point.y + 30) / PTM_RATIO);
		this->addChild(sprite, 2, 10);
		bodyDef.userData = sprite;

		//bodyData->Name = "Effect";
		//bodyData->Tag = 0;

		//body->SetUserData(bodyData);

		body = _world->CreateBody(&bodyDef);
		body->SetGravityScale(0.0f);

		b2FixtureDef fixtureDef;
		b2PolygonShape dynamicBox;

		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;

		dynamicBox.SetAsBox(size.width / PTM_RATIO, size.height / PTM_RATIO);

		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = 100.0f;
		fixtureDef.friction = 0.8f;
		fixtureDef.restitution = 0.8f;

		body->CreateFixture(&fixtureDef);

		effectNormalBody = body;
		effectDummy1 = sprite;

		effectNormalBody->SetActive(false);
	}

	else if (tag == 1)
	{
		bodyDef.position.Set((point.x + 100) / PTM_RATIO, (point.y + 30) / PTM_RATIO);
		this->addChild(sprite, 2, 10);
		bodyDef.userData = sprite;

		//bodyData->Name = "Effect";
		//bodyData->Tag = 0;

		//body->SetUserData(bodyData);

		body = _world->CreateBody(&bodyDef);
		body->SetGravityScale(0.0f);

		b2FixtureDef fixtureDef;
		b2PolygonShape dynamicBox;

		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;

		dynamicBox.SetAsBox(size.width * 2 / PTM_RATIO, size.height / PTM_RATIO);

		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = 100.0f;
		fixtureDef.friction = 0.8f;
		fixtureDef.restitution = 0.8f;

		body->CreateFixture(&fixtureDef);
		effectChargeBody = body;
		effectDummy2 = sprite;

		effectChargeBody->SetActive(false);
	}

	else if (tag == 2)
	{
		bodyDef.position.Set((point.x) / PTM_RATIO, (point.y + 30) / PTM_RATIO);
		this->addChild(sprite, 2, 10);
		bodyDef.userData = sprite;

		//bodyData->Name = "Effect";
		//bodyData->Tag = 0;

		//body->SetUserData(bodyData);

		body = _world->CreateBody(&bodyDef);
		body->SetGravityScale(0.0f);

		b2FixtureDef fixtureDef;
		b2PolygonShape dynamicBox;

		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;

		dynamicBox.SetAsBox(size.width * 2 / PTM_RATIO, size.height / PTM_RATIO);

		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = 100.0f;
		fixtureDef.friction = 0.8f;
		fixtureDef.restitution = 0.8f;

		body->CreateFixture(&fixtureDef);
		effectJumpBody = body;
		effectDummy3 = sprite;

		effectJumpBody->SetActive(false);
	}

	else if (tag == 3)
	{
		bodyDef.position.Set((point.x + 50) / PTM_RATIO, (point.y) / PTM_RATIO);
		this->addChild(sprite, 2, 21);
		bodyDef.userData = sprite;

		//bodyData->Name = "Fire";
		//bodyData->Tag = 2;

		//body->SetUserData(bodyData);

		body = _world->CreateBody(&bodyDef);
		body->SetGravityScale(0.0f);

		b2FixtureDef fixtureDef;
		b2PolygonShape dynamicBox;

		dynamicBox.SetAsBox(size.width / PTM_RATIO, size.height / PTM_RATIO);

		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;

		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = 50.0f;
		fixtureDef.friction = 0.8f;
		fixtureDef.restitution = 0.8f;

		body->CreateFixture(&fixtureDef);
		projectileBody = body;

		projectileBody->SetActive(true);
		//projectileBody->SetBullet(true);
	}

	else
	{
		bodyDef.position.Set(((point.x) / PTM_RATIO), ((point.y) / PTM_RATIO));
		//sprite->setName("Enemy");

		this->addChild(sprite, 2);
		bodyDef.userData = sprite;

		body = _world->CreateBody(&bodyDef);
		body->SetGravityScale(0.0f);

		b2FixtureDef fixtureDef;
		b2PolygonShape dynamicBox;

		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;

		dynamicBox.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = 10.0f;
		fixtureDef.friction = 0.8f;
		fixtureDef.restitution = 0.8f;

		body->CreateFixture(&fixtureDef);
	}

	return body;
}

void HelloWorld::BeginContact(b2Contact* contact)
{
	log("contact begin");

	//fixA가 충격을 받는 물체 fixB는 충격을 주는 물체 
	b2Fixture* fixA = contact->GetFixtureA();
	b2Fixture* fixB = contact->GetFixtureB();

	b2Body* bodyA = fixA->GetBody();
	b2Body* bodyB = fixB->GetBody();

	Sprite* impacted = (Sprite*)bodyA->GetUserData();
	Sprite* impact = (Sprite*)bodyB->GetUserData();

	log("%d", impact->getTag());
	log("%d", impacted->getTag());

	log("Impact Name : %s", impact->getName().c_str());
	log("Impacted Name : %s", impacted->getName().c_str());

	if (impacted->getName() == "Dragon" || impacted->getName() == "Bird" || impacted->getName() == "DK" || impact->getName() == "Dragon" || impact->getName() == "Bird" || impact->getName() == "DK")
	{
		log("Enemy Vs Player");

		for (int i = 0; i < enemyBodies.size(); i++)
		{
			if (bodyA == enemyBodies.at(i))
			{
				hitSprite = (Sprite*)bodyA->GetUserData();

				log("HitSprite Name : %s", hitSprite->getName().c_str());

				hitIndex = i;

				log("Vector Index Number : %d", i);

				hitBody = enemyBodies.at(i);

				hitBody->ApplyForce(b2Vec2(1000, 800), hitBody->GetWorldCenter(), true);

				killSuccess = true;
			}
			else if (bodyB == enemyBodies.at(i))
			{
				hitSprite = (Sprite*)bodyB->GetUserData();

				log("HitSprite Name : %s", hitSprite->getName().c_str());

				hitIndex = i;

				log("Vector Index Number : %d", i);

				hitBody = enemyBodies.at(i);

				hitBody->ApplyForce(b2Vec2(1000, 800), hitBody->GetWorldCenter(), true);

				killSuccess = true;
			}
		}
	}
	
	//else if (impacted->getTag() == 10 && impact->getName() == "Dragon" || impacted->getTag() == 10 && impact->getName() == "Bird" || impacted->getTag() == 10 && impact->getName() == "DK")
	
	//else if (impact->getName() == "Dragon" ||impact->getName() == "Bird" || impact->getName() == "DK")
	//{
	//	log("Enemy Vs Player");

	//	for (int i = 0; i < enemyBodies.size(); i++)
	//	{
	//		if (bodyA == enemyBodies.at(i))
	//		{
	//			hitSprite = (Sprite*)bodyA->GetUserData();

	//			//hitSprite->setVisible(false);
	//			log("HitSprite Name : %s", hitSprite->getName().c_str());

	//			log("Vector Index Number : %d", i);

	//			hitIndex = i;

	//			hitBody = enemyBodies.at(i);

	//			hitBody->ApplyForce(b2Vec2(1000, 800), hitBody->GetWorldCenter(), true);

	//			killSuccess = true;
	//		}
	//	}
	//}

	if (impact->getTag() == 10 && impacted->getTag() == 21)
	{
		log("Fire Vs Player");

		log("Density : %f", fixB->GetDensity());
		log("Restitution : %f", fixB->GetRestitution());
		log("Friction : %f", fixB->GetFriction());

		destoryFire = true;
	}
}

void HelloWorld::tick(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 3;

	_world->Step(dt, velocityIterations, positionIterations);

	for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			Sprite *spriteData = (Sprite*)b->GetUserData();

			//log("Check SpriteData Name : %s", spriteData->getName().c_str());

			if (spriteData->getName() != "Dragon" || spriteData->getName() != "Bird" || spriteData->getName() != "DK")
			{
				spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
				spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
			}
			else
			{
				log("Check SpriteData Name : %s", spriteData->getName().c_str());
			}
		}
	}

	//log("EnemyDatas Size : %d", enemyDatas.size());
	//log("EnemyBoides Size : %d", enemyBodies.size());
	//log("Position Dummies : %d", positionDummies.size());

	if (enemyDatas.size() != 0)
	{
		for (int i = 0; i < enemyDatas.size(); i++)
		{
			auto eSp = (Sprite*)enemyDatas.at(i);

			position = (Sprite*)(positions.at(i));

			positionDummy = positionDummies.at(i);

			positionDummy = Vec2(position->convertToWorldSpace(this->getPosition()).x, position->convertToWorldSpace(this->getPosition()).y + 30);
			
			if (!killSuccess)
			{
				enemyBody = (b2Body*)enemyBodies.at(i);

				enemyBodyPos = b2Vec2(enemyBody->GetWorldCenter());

				enemyBody->SetTransform(b2Vec2(positionDummy.x / PTM_RATIO, positionDummy.y / PTM_RATIO), 0);
				
				eSp->setPosition(Vec2(enemyBody->GetPosition().x * PTM_RATIO, enemyBody->GetPosition().y * PTM_RATIO));
				//eSp->setPosition(Vec2(positionDummy.x, positionDummy.y));
				
				eSp->setRotation(-1 * CC_RADIANS_TO_DEGREES(enemyBody->GetAngle()));
			}

			else
			{
				if (eSp != hitSprite)
				{
					enemyBody = (b2Body*)enemyBodies.at(i);

					enemyBodyPos = b2Vec2(enemyBody->GetWorldCenter());

					enemyBody->SetTransform(b2Vec2(positionDummy.x / PTM_RATIO, positionDummy.y / PTM_RATIO), 0);

					eSp->setPosition(Vec2(enemyBody->GetPosition().x * PTM_RATIO, enemyBody->GetPosition().y * PTM_RATIO));

					eSp->setRotation(-1 * CC_RADIANS_TO_DEGREES(enemyBody->GetAngle()));
				}

				else
				{
					hitSprite->setPosition(Vec2(hitBody->GetPosition().x * PTM_RATIO, hitBody->GetPosition().y * PTM_RATIO));

					deathScale -= 0.06f;

					if (deathScale < 0.0f)
					{
						deathScale = 0.f;
					}

					hitSprite->setScale(deathScale);

					bool oneTime = this->isScheduled(schedule_selector(HelloWorld::EnemyDeath));

					if (!oneTime)
					{
						this->scheduleOnce(schedule_selector(HelloWorld::EnemyDeath), 1);
						oneTime = true;
					}
				}
			}
		}
	}

	if (countStart)
	{
		chargeCount += 0.1f;
		//log("Charge Count : %f", chargeCount);

		if (chargeCount > 2.0f)
		{
			player->playerAnimation = character->createAnimationMultiSprite("images/HI/HI_Charge_%d.png", 3, 0.07f);
			player->playerAnimate = Animate::create(player->playerAnimation);
			character->switchingAction(player->playerSprite, player->playerAnimate, false);
		}

		if (chargeCount > 10.0f)
		{
			chargeCount = 10.0f;
		}
	}

	if (isTransFormed)
	{
		inverseTransformCount += 0.001f;

		progressTimer->setColor(Color3B::GRAY);
		progressTimer->setPercentage(progressTimer->getPercentage() + inverseTransformCount);

		log("Transbutton Percentage : %f", progressTimer->getPercentage());

		if (progressTimer->getPercentage() == 100)
		{
			progressTimer->setPercentage(0);

			isTransFormed = false;
		}
	}

	if (enemyBodyPos.x < 400)
	{
		if (numberOfFire < 11 && !destoryFire && !killSuccess)
		{
			if (!this->isScheduled(schedule_selector(HelloWorld::ShootFire)))
			{
				this->schedule(schedule_selector(HelloWorld::ShootFire), 2);
			}
		}

		//else if (destoryFire)
		//{
		//	bool oneTime = this->isScheduled(schedule_selector(HelloWorld::FireDeath));

		//	if (!oneTime)
		//	{
		//		this->scheduleOnce(schedule_selector(HelloWorld::FireDeath), 1);
		//		oneTime = true;
		//	}
		//}
	}
}

void HelloWorld::detectGround(float bx)
{
	//int x = ((abs(bx) + player->playerSprite->getPosition().x) / tmap->getTileSize().width);

	//int tileGid = this->background->getTileGIDAt(Vec2(x, 8));

	//Value properties = tmap->getPropertiesForGID(tileGid);

	//if (properties.isNull())
	//{
	//	//log("Detected cliff : %d", count++);
	//}
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint1 = attackButton->convertToNodeSpace(touch->getLocation());

	Size s1 = attackButton->getContentSize();

	Rect r1 = Rect(0, 0, s1.width, s1.height);

	auto touchPoint2 = jumpButton->convertToNodeSpace(touch->getLocation());

	Size s2 = jumpButton->getContentSize();

	Rect r2 = Rect(0, 0, s2.width, s2.height);

	auto touchPoint3 = transformButton->convertToNodeSpace(touch->getLocation());

	Size s3 = transformButton->getContentSize();

	Rect r3 = Rect(0, 0, s3.width, s3.height);

	if (r1.containsPoint(touchPoint1) && !isAttack)
	{
		log("Attack");

		countStart = true;

		isAttack = true;
	}
	else if (r1.containsPoint(touchPoint1) && isAttack)
	{
		return false;
	}

	if (r2.containsPoint(touchPoint2) && !isJumped)
	{
		log("Jump");

		isJumped = true;
	}
	else if (r2.containsPoint(touchPoint2) && isJumped)
	{
		return false;
	}

	if (r3.containsPoint(touchPoint3) && !isTransFormed)
	{
		log("Transfrom");

		transformButton->setColor(Color3B(Color4B(transButtonColor.r, transButtonColor.g, transButtonColor.b, 0)));

		player->playerAction(player->TWALK);

		isTransFormed = true;
	}
	else if (r3.containsPoint(touchPoint3) && isTransFormed)
	{
		return false;
	}

	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	countStart = false;

	if (isAttack)
	{
		attackButton->setColor(Color3B::GRAY);

		auto effect = Sprite::create();

		if (!isTransFormed)
		{
			if (chargeCount < 5.0f)
			{
				effect = Sprite::create("images/Effect/HI_Effect_Charge_1.png");

				this->addChild(effect, 3, 31);

				player->Attack(isTransFormed, effect, effectNormalBody, chargeCount);
			}
			else
			{
				effect = Sprite::create("images/Effect/HI_Effect_Charge_1.png");

				this->addChild(effect, 3, 31);

				player->Attack(isTransFormed, effect, effectChargeBody, chargeCount);
			}
		}
		else
		{
			if (chargeCount < 5.0f)
			{
				effect = Sprite::create("images/Effect/HI2_Effect_Normal_1.png");

				this->addChild(effect, 3, 31);

				player->Attack(isTransFormed, effect, effectNormalBody, chargeCount);
			}
			else
			{
				effect = Sprite::create("images/Effect/HI2_Effect_Charge_1.png");

				this->addChild(effect, 3, 31);

				player->Attack(isTransFormed, effect, effectChargeBody, chargeCount);
			}
		}
	}

	if (isJumped)
	{
		jumpButton->setColor(Color3B::GRAY);

		player->Jump(isTransFormed);
		//isJumped = false;
	}

	if (isJumped && isAttack)
	{
		auto effect = Sprite::create();

		effect = Sprite::create("images/Effect/HI2_Effect_Charge_1.png");

		this->addChild(effect, 3, 31);

		player->Attack(isTransFormed, effect, effectJumpBody, chargeCount);
	}

	bool oneTime = this->isScheduled(schedule_selector(HelloWorld::AfterAction));

	if (!oneTime)
	{
		this->schedule(schedule_selector(HelloWorld::AfterAction), 0.8f);
		oneTime = true;
	}
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);

	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void HelloWorld::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

HelloWorld::~HelloWorld()
{
	delete _world;
	_world = nullptr;
}