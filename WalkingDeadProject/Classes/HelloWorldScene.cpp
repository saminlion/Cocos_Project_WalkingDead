#include "HelloWorldScene.h"

USING_NS_CC;

//#define MASK_PLAYER = ENEMY;
//#define MASK_ENEMY = PLAYER;



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
	character = new Character();
	player = new Player();
	enemy = new Enemy();

	bodyData = new BodyUserData;

	killSuccess = false;
	isTransFormed = false;
	destoryFire = false;

	winSize = Director::getInstance()->getWinSize();

	backgroud_x = 0;
	count = 0;
	tmap = TMXTiledMap::create("tile/Platform.tmx");
	background = tmap->getLayer("Background");
	playerSpawnPoint = tmap->getObjectGroup("Objects");
	dragonSpawnPoint = tmap->getObjectGroup("DragonSpawnPoints");
	birdSpawnPoint = tmap->getObjectGroup("BirdSpawnPoints");
	character->getDragonSpawnPointPositions(dragonSpawnPoint);
	character->getBirdSpawnPointPositions(birdSpawnPoint);

	this->addChild(tmap, -1, 11);

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
	ValueMap spawnPoint = playerSpawnPoint->getObject("SpawnPoint");

	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	player->playerSprite = Sprite::create("images/HI/HI_Walk_1.png");

	player->playerSprite->setPosition(Vec2(x, y));
	player->playerSprite->setAnchorPoint(Vec2::ZERO);
	player->playerSprite->setScale(1.5f);
	this->addChild(player->playerSprite, 2);

	player->playerAction(player->UWALK);

#pragma endregion

#pragma region Init Enemy Character - Dragon

	dragonPostions = character->setDragonPosition();
	birdPositions = character->setBirdPosition();
	//vector<Sprite*> dkPositions = character->setDKPosition();

	positions = character->setEnemyPosition();

	log("Positions Size : %d", positions.size());

	for (int i = 0; i < positions.size(); i++)
	{
		position = (Sprite*)(positions.at(i));

		tmap->addChild(position);

		positionDummy = Vec2(position->convertToWorldSpace(this->getPosition()).x, position->convertToWorldSpace(this->getPosition()).y);

		positionDummies.push_back(positionDummy);
	}

#pragma endregion

	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(HelloWorld::tick), 1 / 60.f);
	}

	this->schedule(schedule_selector(HelloWorld::MoveMap));


	//this->schedule(schedule_selector(HelloWorld::ShootFire), 1.0f);

	return true;
}

void HelloWorld::SpawnEnemy()
{
	dragons = enemy->CreateDragon(dragonPostions);
	birds = enemy->CreateBird(birdPositions);

#pragma region Dragon

	log("Dragons Size : %d", dragons.size());
	log("Birds Size : %d", birds.size());

	for (int i = 0; i < dragons.size(); i++)
	{
		dragon = (Sprite*)dragons.at(i);

		positionDummy = positionDummies.at(i);

		dragon->setPosition(positionDummy.x, positionDummy.y - 30.0f);

		//log("Check Dragon Name : %s", dragon->getName().c_str());
		log("Check Dragon : %d", dragon->getTag());

		auto eBody = this->addBody(positionDummy, dragon->getContentSize() / 2, b2_dynamicBody, dragon, 4, ENEMY, PLAYER);

		//bodyData->Name = "Dragon";
		//bodyData->Tag = i;

		////string* eBodyName = new string("Dragon");

		//eBody->SetUserData(bodyData);

		//enemyBodies->addObject(eBody);
		enemyBodies.push_back(eBody);
	}

	for (int i = 0; i < birds.size(); i++)
	{
		bird = (Sprite*)birds.at(i);

		positionDummy = positionDummies.at(i);

		bird->setPosition(positionDummy.x, positionDummy.y - 30.0f);

		log("Check Bird : %d", bird->getTag());

		auto eBody = this->addBody(positionDummy, bird->getContentSize() / 2, b2_dynamicBody, bird, 4, ENEMY, PLAYER);

		//bodyData->Name = "Bird";
		//bodyData->Tag = i;

		////string* eBodyName = new string("Dragon");

		//eBody->SetUserData(bodyData);

		enemyBodies.push_back(eBody);
	}

	log("EnemyBodies Size : %d", enemyBodies.size()); //enemyBodies.size());

	for (int i = 0; i < enemyBodies.size(); i++)
	{
		auto eb = (b2Body*)enemyBodies.at(i);
		auto eSP = (Sprite*)eb->GetUserData();

		enemyDatas.pushBack(eSP);
		//enemyDatas.push_back(eSP);
	}

#pragma endregion
}

void HelloWorld::ShootFire(float dt)
{
	log("Shooting fire");

	fireSprite = Sprite::create("images/Effect/Dragon_Fire_Effect_1.png");
	fireSprite->setScale(2.0f);

	projectileBody = this->addBody(positionDummy, fireSprite->getContentSize() / 2, b2_dynamicBody, fireSprite, 3, FIRE, PLAYER);
	projectileBody->ApplyLinearImpulse(b2Vec2(-1000.f, 0.f), projectileBody->GetWorldCenter(), true);

	this->unschedule(schedule_selector(HelloWorld::ShootFire));
}

void HelloWorld::EnemyDeath(float dt)
{
	log("CallFunc");

	killSuccess = false;

	clearSpriteBody();
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

void HelloWorld::clearSpriteBody()
{
	log("EnemyDatas Size : %d", enemyDatas.size());
	log("EnemyBoides Size : %d", enemyBodies.size());
	log("Position Size : %d", positions.size());

	for (int i = 0; i < enemyDatas.size(); i++)
	{
		auto spriteData = (Sprite*)enemyDatas.at(i);
		auto spriteBody = (b2Body*)enemyBodies.at(i);

		if (spriteData == hitSprite && spriteBody == hitBody)//&& !_world->IsLocked()
		{
			log("Clear Start");

			enemyDatas.eraseObject(hitSprite, true);
			//enemyBodies.erase(enemyBodies.begin());
			//enemyBodies.clear();

			enemyBodies._Pop_back_n(hitIndex);
			//enemyBodies.erase(enemyBodies.at(hitIndex));

			positions.pop_back();

			//enemyDatas.re
			
			this->removeChild(hitSprite, true);

			_world->DestroyBody(hitBody);
			
			log("Clear Done");

			log("EnemyDatas Size : %d", enemyDatas.size());
			log("EnemyBoides Size : %d", enemyBodies.size());
			log("Position Size : %d", positions.size());

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

void HelloWorld::MoveMap(float f)
{
	if (backgroud_x == -1000)
	{
		backgroud_x = 0;
		tmap->setPosition(Vec2(backgroud_x, 0));
		//objects->setPositionOffset(Vec2(backgroud_x, 0));
		//background->setPosition(Vec2(backgroud_x, 0));
	}

	backgroud_x -= 2;
	tmap->setPosition(Vec2(backgroud_x, 0));
	//objects->setPositionOffset(background->getPosition());
	detectGround(backgroud_x);

}

void HelloWorld::detectGround(float bx)
{
	int x = ((abs(bx) + player->playerSprite->getPosition().x) / tmap->getTileSize().width);

	int tileGid = this->background->getTileGIDAt(Vec2(x, 8));

	Value properties = tmap->getPropertiesForGID(tileGid);

	if (properties.isNull())
	{
		//log("Detected cliff : %d", count++);
	}
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

#pragma region Create EffectBody 
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

#pragma endregion

	this->SpawnEnemy();

#pragma region FireBody

#pragma endregion

	_world->SetContactListener(this);

	return true;
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


	if (impact->getTag() == 10 && impacted->getName() == "Dragon" || impact->getTag() == 10 && impacted->getName() == "Bird" || impact->getTag() == 10 && impacted->getName() == "DK")
	{
		log("Enemy Vs Player");

		for (int i = 0; i < enemyBodies.size(); i++)
		{
			if (bodyA == enemyBodies.at(i))
			{
				hitSprite = (Sprite*)bodyA->GetUserData();

				//hitSprite->setVisible(false);
				log("HitSprite Name : %s", hitSprite->getName().c_str());

				log("Vector Index Number : %d", i);

				hitIndex = i;

				hitBody = enemyBodies.at(i);

				hitBody->ApplyForce(b2Vec2(1000, 800), hitBody->GetWorldCenter(), true);

				killSuccess = true;

				break;
			}
		}
	}

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

			if (spriteData->getName() != "Dragon" || spriteData->getName() != "Bird" || spriteData->getName() != "DK")
			{
				spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
				spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
			}
			else
			{
				//log("Check SpriteData Name : %s", spriteData->getName().c_str());
			}
		}
	}

	//log("EnemyDatas Size : %d", enemyDatas.size());
	//log("EnemyBoides Size : %d", enemyBodies.size());
	//log("Position Size : %d", positions.size());

	if (enemyDatas.size() != 0)
	{
		for (int i = 0; i < enemyDatas.size(); i++)
		{
			position = (Sprite*)positions.at(i);

			auto eSp = (Sprite*)enemyDatas.at(i);

			//enemyDatas.erase(enemyDatas.begin() + i);

			//log("Check enemyData Name : %s", eSp->getName().c_str());

			if (!killSuccess)
			{
				if (eSp != hitSprite && eSp != nullptr)
				{
					positionDummy = Vec2(position->convertToWorldSpace(this->getPosition()).x, position->convertToWorldSpace(this->getPosition()).y);

					//log("PositionDummy : %f .... %f", positionDummy.x, positionDummy.y);

					enemyBody = (b2Body*)enemyBodies.at(i);

					enemyBodyPos = b2Vec2(enemyBody->GetWorldCenter());

					enemyBody->SetTransform(b2Vec2(positionDummy.x / PTM_RATIO, positionDummy.y / PTM_RATIO), 0);

					if (eSp == nullptr)
					{
						log("null");
					}
					else
					{
						log("something");
					}

					eSp->setPosition(positionDummy);

					//log("SpriteData Position : %f .... %f", eSp->getPositionX(), eSp->getPositionY());

					eSp->setRotation(-1 * CC_RADIANS_TO_DEGREES(enemyBody->GetAngle()));
				}
			}
			else
			{
				if (eSp == hitSprite)
				{
					eSp->setPosition(Vec2(hitBody->GetPosition().x * PTM_RATIO, hitBody->GetPosition().y * PTM_RATIO));
					eSp->setRotation(-1 * CC_RADIANS_TO_DEGREES(hitBody->GetAngle()));

					//log("hitSprite position : %f ... %f", eSp->getPositionX(), eSp->getPositionY());
					//log("enemyBody Position : %f ... %f", hitBody->GetPosition().x, hitBody->GetPosition().y);

					deathScale -= 0.006f;

					if (deathScale < 0.0f)
					{
						deathScale = 0.f;
					}

					eSp->setScale(deathScale);

					bool oneTime = this->isScheduled(schedule_selector(HelloWorld::EnemyDeath));

					if (!oneTime)
					{
						this->scheduleOnce(schedule_selector(HelloWorld::EnemyDeath), 3);
						oneTime = true;
					}
				}
				//enemyDatas.pop_back();
			}
		}
	}

	if (countStart)
	{
		chargeCount += 0.1f;
		log("Charge Count : %f", chargeCount);

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