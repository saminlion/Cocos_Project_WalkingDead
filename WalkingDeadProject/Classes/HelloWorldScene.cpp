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
	tmap->setPosition(Vec2(0, 0));
	tmap->setAnchorPoint(Vec2::ZERO);
	this->addChild(tmap, -1, 11);
	SetTmap(StageNumber::Stage0, true);

	tmap2 = CreateTmap(StageNumber::Stage1);
	tmap2->setPosition(Vec2(tmap->getContentSize().width, 0));
	tmap2->setAnchorPoint(Vec2::ZERO);
	this->addChild(tmap2, -1, 11);
	SetTmap(StageNumber::Stage1, true);

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

	this->MoveMap();

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

		Vec2 playerPos = Vec2(x, y + 100);

		player->playerSprite = Sprite::create("images/PlayerKnight/Run (1).png");

		player->playerSprite->setPosition(playerPos);
		player->playerSprite->setScale(1.f);
		//this->addChild(player->playerSprite, 2);

		player->playerAction(player->UWALK);
	}

	return newTileMap;
}

void HelloWorld::SetTmap(int tileMapNumber, bool firstTime)
{
	wormPositions.clear();
	crapPositions.clear();

	character->_wormPositions.clear();
	character->_crapPositions.clear();

	character->wormPositions.clear();
	character->crapPositions.clear();

	log("SpawnPoint");

	if (tileMapNumber == Stage0)
	{
		wormspawnPoint = tmap->getObjectGroup("WormSpawnPoints");

		crapspawnPoint = tmap->getObjectGroup("CrapSpawnPoints");

		character->getwormspawnPointPositions(wormspawnPoint);

		character->getcrapspawnPointPositions(crapspawnPoint);

		wormPositions = character->setWormPosition();

		crapPositions = character->setCrapPosition();
	}

	else
	{
		wormspawnPoint = tmap2->getObjectGroup("WormSpawnPoints");

		crapspawnPoint = tmap2->getObjectGroup("CrapSpawnPoints");

		character->getwormspawnPointPositions(wormspawnPoint);

		character->getcrapspawnPointPositions(crapspawnPoint);

		wormPositions = character->setWormPosition();

		crapPositions = character->setCrapPosition();
	}
	
	for (int j = 0; j < wormPositions.size() + crapPositions.size(); j++)
	{
		if (j < wormPositions.size())
		{
			position = (Sprite*)(wormPositions.at(j));

			if (tileMapNumber == Stage0)
			{
				if (!firstTime)
				{
					position->removeFromParentAndCleanup(true);
					tmap->addChild(position, 0, j);
				}
				else
				{
					tmap->addChild(position, 0, j);
				}
			}
			else
			{
				if (!firstTime)
				{
					position->removeFromParentAndCleanup(true);
					tmap2->addChild(position, 0, j);
				}
				else
				{
					tmap2->addChild(position, 0, j);
				}
			}

			positionDummy = Vec2(position->convertToWorldSpace(this->getPosition()).x, position->convertToWorldSpace(this->getPosition()).y);

			positions.push_back(position);

			positionDummies.push_back(positionDummy);
		}

		if (j >= wormPositions.size() && j < wormPositions.size() + crapPositions.size())
		{
			position = (Sprite*)crapPositions.at(j - wormPositions.size());

			if (tileMapNumber == Stage0)
			{
				if (!firstTime)
				{
					position->removeFromParentAndCleanup(true);
					tmap->addChild(position, 0, j);
				}
				else
				{
					tmap->addChild(position, 0, j);
				}
			}

			else
			{
				if (!firstTime)
				{
					position->removeFromParentAndCleanup(true);
					tmap2->addChild(position, 0, j);
				}
				else
				{
					tmap2->addChild(position, 0, j);
				}
			}

			positionDummy = Vec2(position->convertToWorldSpace(this->getPosition()).x, position->convertToWorldSpace(this->getPosition()).y);
		
			positions.push_back(position);

			positionDummies.push_back(positionDummy);
		}
	}

	this->SpawnEnemy();
}

void HelloWorld::MoveMap()
{
	auto move1 = Sequence::create(MoveBy::create(15, Vec2(-tmap->getContentSize().width, 0)),
		Place::create(Vec2(tmap2->getContentSize().width, 0)),
		CallFunc::create(CC_CALLBACK_0(HelloWorld::SetTmap, this, StageNumber::Stage0, false)),
		MoveBy::create(15, Vec2(-tmap->getContentSize().width, 0)),
		nullptr);

	auto move2 = Sequence::create(MoveBy::create(15, Vec2(-tmap2->getContentSize().width, 0)),
		MoveBy::create(15, Vec2(-tmap2->getContentSize().width, 0)),
		Place::create(Vec2(tmap->getContentSize().width, 0)),
		CallFunc::create(CC_CALLBACK_0(HelloWorld::SetTmap, this, StageNumber::Stage1, false)),
		nullptr);

	tmap->runAction(RepeatForever::create(move1));
	tmap2->runAction(RepeatForever::create(move2));
}

void HelloWorld::SpawnEnemy()
{
	log("Spawn Enemy");

	enemy->firstEnemySprites.clear();
	enemy->secondEnemySprites.clear();

	worms = enemy->CreateFirstEnemy(wormPositions);
	craps = enemy->CreateSecondEnemy(crapPositions);

	for (int i = 0; i < positionDummies.size(); i++)
	{
		positionDummy = positionDummies.at(i);

		if (i < worms.size())
		{
			worm = (Sprite*)worms.at(i);

			worm->setPosition(positionDummy.x, positionDummy.y + 30);

			worm->setScale(2.0f);

			auto eBody = this->addBody(positionDummy, worm->getContentSize() * 2, b2_dynamicBody, worm, 4, ENEMY, PLAYER);

			enemyBodies.push_back(eBody);

			enemyDatas.pushBack(worm);
		}

		else if (i >= worms.size() && i < craps.size() + worms.size())
		{
			crap = (Sprite*)craps.at(i - worms.size());

			crap->setPosition(positionDummy.x, positionDummy.y + 30);

			auto eBody = this->addBody(positionDummy, crap->getContentSize(), b2_dynamicBody, crap, 4, ENEMY, PLAYER);

			enemyBodies.push_back(eBody);

			enemyDatas.pushBack(crap);
		}
	}
}

void HelloWorld::ShootFire(float dt)
{
	log("Shooting fire");

	for (int i = 0; i < positionDummies.size(); i++)
	{
		positionDummy = positionDummies.at(i);

		projectileSprite = Sprite::create("images/Effect/Dragon_Fire_Effect_1.png");
		projectileSprite->setScale(2.0f);

		auto fireAnimation = character->createAnimationMultiSprite("images/Effect/Dragon_Fire_Effect_%d.png", 5, 0.2f);

		auto fireAnimate = Animate::create(fireAnimation);

		projectileSprite->runAction(fireAnimate);

		projectileBody = this->addBody(positionDummy, projectileSprite->getContentSize() / 2, b2_dynamicBody, projectileSprite, 3, FIRE, PLAYER);
		projectileBody->ApplyLinearImpulse(b2Vec2(-1000.f, 0.f), projectileBody->GetWorldCenter(), true);

		projectileBodies.push_back(projectileBody);
	}
	
	this->unschedule(schedule_selector(HelloWorld::ShootFire));
}

void HelloWorld::clearSpriteBody(float dt)
{
	killSuccess = false;

	destoryFire = false;

	log("Clear Begin");

	for (int i = 0; i < enemyBodies.size(); i++)
	{
		auto spriteBody = (b2Body*)enemyBodies.at(i);
		auto spriteData = (Sprite*)enemyDatas.at(i);

		if (spriteData == hitEnemySprite && spriteBody == hitEnemyBody && _world->IsLocked() == false && i == hitIndex)
		{
			enemyDatas.eraseObject(hitEnemySprite, true);
			enemyBodies.erase(enemyBodies.begin() + i);
			positions.erase(positions.begin() + i);
			positionDummies.erase(positionDummies.begin() + i);
								
			this->removeChild(spriteData, true);

			_world->DestroyBody(spriteBody);

			log("Clear Done");

			break;
		}
	}

	for (int i = 0; i < projectileBodies.size(); i++)
	{
		auto spriteBody = (b2Body*)projectileBodies.at(i);
		auto spriteData = (Sprite*)spriteBody->GetUserData();

		if (spriteData == hitProjectSprite && spriteBody == hitProjectBody && _world->IsLocked() == false && i == hitIndex)
		{
			projectileBodies.erase(projectileBodies.begin() + i);

			this->removeChild(spriteData, true);

			_world->DestroyBody(spriteBody);

			log("Clear Done");

			break;
		}
	}
}

void HelloWorld::AfterAction(float dt)
{
	if (isAttack)
	{
		effectNormalBody->SetActive(false);
		effectChargeBody->SetActive(false);

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

	player->playerPos = player->playerSprite->convertToWorldSpace(this->getPosition());

	effectDummy1->setPosition(Vec2(player->playerPos.x, player->playerPos.y));
	effectDummy2->setPosition(Vec2(player->playerPos.x, player->playerPos.y));

	effectNormalBody = this->addBody(player->playerPos, Size(effectDummy1->getContentSize() / 4), b2_staticBody, effectDummy1, 0, PLAYER, ENEMY | FIRE);
	effectChargeBody = this->addBody(player->playerPos, Size(effectDummy2->getContentSize() / 4), b2_staticBody, effectDummy2, 1, PLAYER, ENEMY | FIRE);
	playerDamageBody = this->addBody(Vec2(player->playerPos.x, player->playerPos.y), Size(player->playerSprite->getContentSize() / 2), b2_staticBody, player->playerSprite, 2, PLAYER, ENEMY | FIRE);
	
	//player->playerSprite->setAnchorPoint(Vec2::ZERO);
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
		bodyDef.position.Set((point.x) / PTM_RATIO, (point.y) / PTM_RATIO);
		bodyDef.userData = sprite;
		this->addChild(sprite, 2);
		//sprite->setAnchorPoint(Vec2::ZERO);

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
		
		playerDamageBody = body;

		playerDamageBody->SetActive(true);
	}

	else if (tag == 3)
	{
		bodyDef.position.Set((point.x + 50) / PTM_RATIO, (point.y) / PTM_RATIO);
		this->addChild(sprite, 2, 21);
		bodyDef.userData = sprite;

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
		projectileBody->SetBullet(true);
	}

	else
	{
		bodyDef.position.Set(((point.x) / PTM_RATIO), ((point.y) / PTM_RATIO));

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

	if (impacted->getName() == "Worm" || impacted->getName() == "Crap" || impacted->getName() == "DK" || impact->getName() == "Worm" || impact->getName() == "Crap" || impact->getName() == "DK")
	{
		log("Enemy Vs Player");

		for (int i = 0; i < enemyBodies.size(); i++)
		{
			if (bodyA == enemyBodies.at(i))
			{
				hitEnemySprite = (Sprite*)bodyA->GetUserData();

				hitIndex = i;

				hitPosition = positions.at(i);

				hitEnemyBody = enemyBodies.at(i);

				hitEnemyBody->ApplyForce(b2Vec2(1000, 800), hitEnemyBody->GetWorldCenter(), true);

				killSuccess = true;
			}

			else if (bodyB == enemyBodies.at(i))
			{
				hitEnemySprite = (Sprite*)bodyB->GetUserData();

				hitIndex = i;

				hitPosition = positions.at(i);

				hitEnemyBody = enemyBodies.at(i);

				hitEnemyBody->ApplyForce(b2Vec2(1000, 800), hitEnemyBody->GetWorldCenter(), true);

				killSuccess = true;
			}
		}
	}

	if (impact->getTag() == 10 && impacted->getTag() == 21)
	{
		log("Fire Vs Player");

		for (int i = 0; i < projectileBodies.size(); i++)
		{
			if (bodyA == projectileBodies.at(i))
			{
				hitProjectSprite = (Sprite*)bodyA->GetUserData();

				hitIndex = i;

				hitProjectBody = projectileBodies.at(i);

				destoryFire = true;
			}
		}
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

			if (spriteData->getName() != "Worm" || spriteData->getName() != "Crap" || spriteData->getName() != "DK")
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
	//log("Position Size : %d", positions.size());

	if (enemyDatas.size() != 0)
	{
		for (int i = 0; i < enemyDatas.size(); i++)
		{
			auto eSp = (Sprite*)enemyDatas.at(i);

			//position = (Sprite*)(positions.at(i));
			auto pos = (Sprite*)positions.at(i);

			positionDummy = positionDummies.at(i);

			//log("Position Dummy %d Postion : %f ... %f", i, positionDummies.at(i).x, positionDummies.at(i).y);

			//log("Position %d Check : %d", i, positions.at(i)->getTag());

			positionDummy = Vec2(pos->convertToWorldSpace(this->getPosition()).x, pos->convertToWorldSpace(this->getPosition()).y + 30);

			//log("Get esp position : %f .... %f", eSp->getPositionX(), eSp->getPositionY());

			if (!killSuccess && eSp->getPosition().x > 0.f)
			{
				enemyBody = (b2Body*)enemyBodies.at(i);

				enemyBodyPos = b2Vec2(enemyBody->GetWorldCenter());

				enemyBody->SetTransform(b2Vec2(positionDummy.x / PTM_RATIO, positionDummy.y / PTM_RATIO), 0);

				eSp->setPosition(Vec2(enemyBody->GetPosition().x * PTM_RATIO, enemyBody->GetPosition().y * PTM_RATIO));

				eSp->setRotation(-1 * CC_RADIANS_TO_DEGREES(enemyBody->GetAngle()));
			}

			else if (killSuccess && hitEnemySprite != nullptr)
			{
				hitEnemySprite->setPosition(Vec2(hitEnemyBody->GetPosition().x * PTM_RATIO, hitEnemyBody->GetPosition().y * PTM_RATIO));

				deathScale -= 0.06f;

				if (deathScale < 0.0f)
				{
					deathScale = 0.f;
				}

				hitEnemySprite->setScale(deathScale);

				if (hitEnemySprite->getScale() == 0)
				{
					bool oneTime = this->isScheduled(schedule_selector(HelloWorld::clearSpriteBody));

					if (!oneTime)
					{
						this->scheduleOnce(schedule_selector(HelloWorld::clearSpriteBody), 1);
						oneTime = true;
					}
				}
			}

			if (eSp->getPosition().x < 0.f)
			{
				hitEnemySprite = enemyDatas.at(i);
				hitEnemyBody = enemyBodies.at(i);
				hitIndex = i;
				hitPosition = positions.at(i);
				killSuccess = true;
			}

			if (eSp->getPosition().x < 480)
			{
				fireInterval -= 0.3f;

				if (fireInterval < 0 && !destoryFire && !killSuccess)
				{
					fireInterval = 20.0f;

					projectileSprite = Sprite::create("images/Effect/Dragon_Fire_Effect_1.png");

					projectileSprite->setScale(2.0f);

					auto fireAnimation = character->createAnimationMultiSprite("images/Effect/Dragon_Fire_Effect_%d.png", 5, 0.2f);

					auto fireAnimate = Animate::create(fireAnimation);

					projectileSprite->runAction(fireAnimate);

					projectileBody = this->addBody(positionDummy, projectileSprite->getContentSize() / 2, b2_dynamicBody, projectileSprite, 3, FIRE, PLAYER);

					projectileBody->ApplyLinearImpulse(b2Vec2(-1000.f, 0.f), projectileBody->GetWorldCenter(), true);

					projectileBodies.push_back(projectileBody);

					if (!projectileBodies.empty())
					{
						for (int i = 0; i < projectileBodies.size(); i++)
						{
							if (projectileSprite != nullptr)
							{
								if (projectileSprite->getPositionX() < player->playerSprite->getPositionX())
								{
									hitProjectBody = projectileBodies.at(i);

									hitIndex = i;

									destoryFire = true;
								}
							}
						}
					}
				}
			}
		}
	}

	if (destoryFire)
	{
		bool oneTime = this->isScheduled(schedule_selector(HelloWorld::clearSpriteBody));

		if (!oneTime)
		{
			this->scheduleOnce(schedule_selector(HelloWorld::clearSpriteBody), 0.1f);
			oneTime = true;
		}
	}

	if (countStart)
	{
		chargeCount += 0.1f;

		if (chargeCount > 2.0f)
		{
			player->playerAnimation = character->createAnimationMultiSprite("images/PlayerSwordsman/Charge_%d.png", 3, 0.07f);
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

		if (progressTimer->getPercentage() == 100)
		{
			progressTimer->setPercentage(0);

			isTransFormed = false;
		}
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

	if (isTransFormed)
	{
		transformButton->setColor(Color3B(Color4B(transButtonColor.r, transButtonColor.g, transButtonColor.b, 0)));

		player->playerAction(player->TWALK);
	}

	if (isAttack)
	{
		attackButton->setColor(Color3B::GRAY);

		if (!isTransFormed)
		{
			if (chargeCount < 5.0f)
			{
				effectSprite = Sprite::create("images/Effect/cut_b_0001.png");

				auto effectAnimation = character->createAnimationMultiSprite("images/Effect/cut_b_000%d.png", 5, 0.2f);

				auto effectAnimate = Animate::create(effectAnimation);

				character->switchingAction(effectSprite, effectAnimate, true);

				this->addChild(effectSprite, 3, 31);

				player->Attack(isTransFormed, effectSprite, effectNormalBody, chargeCount);
			}

			else
			{
				effectSprite = Sprite::create("images/Effect/HI_Effect_Charge_1.png");

				this->addChild(effectSprite, 3, 31);

				player->Attack(isTransFormed, effectSprite, effectChargeBody, chargeCount);
			}
		}
		else
		{
			if (chargeCount < 5.0f)
			{
				effectSprite = Sprite::create("images/Effect/HI2_Effect_Normal_1.png");

				this->addChild(effectSprite, 3, 31);

				player->Attack(isTransFormed, effectSprite, effectNormalBody, chargeCount);
			}
			else
			{
				effectSprite = Sprite::create("images/Effect/HI2_Effect_Charge_1.png");

				this->addChild(effectSprite, 3, 31);

				player->Attack(isTransFormed, effectSprite, effectChargeBody, chargeCount);
			}
		}
	}

	if (isJumped)
	{
		jumpButton->setColor(Color3B::GRAY);

		player->Jump(isTransFormed);
		//isJumped = false;
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
	delete  _world;
	delete 	character;
	delete  player;
	delete  enemy;

	_world = nullptr;
}