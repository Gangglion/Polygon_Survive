#include "stdafx.h"
#include "SceneIngame.h"
#include "Bullet.h"
#include "Unit.h"
#include "EnemyComponent.h"

SceneIngame * SceneIngame::create()
{
	auto ret = new SceneIngame();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool SceneIngame::init()
{
	if (!Scene::initWithPhysics()) return false;

	//Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//auto visibleSize = Director::getInstance()->getVisibleSize();

	auto keybd = EventListenerKeyboard::create();
	keybd->onKeyPressed = std::bind(&SceneIngame::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keybd->onKeyReleased = std::bind(&SceneIngame::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
	
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keybd, this);

	auto contact = EventListenerPhysicsContact::create();
	contact->onContactBegin = std::bind(&SceneIngame::onContactBegin, this, std::placeholders::_1);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contact, this);

	//모양변경용 마우스 클릭 이벤트
	auto Mouse = EventListenerMouse::create();
	Mouse->onMouseDown = CC_CALLBACK_1(SceneIngame::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Mouse, this);

	//초당 60회 업데이트 
	schedule(CC_SCHEDULE_SELECTOR(SceneIngame::logic));

	//모양변경용 임시 레이블 생성
	auto label = Label::createWithTTF("If Click Left MouseButton, Change Shape", "fonts/Marker Felt.ttf", 36);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(720 / 2, 1280 / 2 - 100));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	return true;
}

void SceneIngame::onEnter()
{
	Scene::onEnter();

	//물리공간 할당
	auto world = getPhysicsWorld();

	//디버그 모드(Environment.h 메크로)
	if(DEBUG_MODE) world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//world->setGravity(Vec2(0,-9.81)); // 중력 있음
	world->setGravity(Vec2::ZERO); // 중력 없음
	player = Unit::create(UnitShapeType::TRI, PLAYER_MASK, TAG_PLAYER);
	player->setPosition(Vec2(720 / 2, 1280 / 2 - 200));
	addChild(player);

	//enemy생성 - 임시주석처리
	//{
	//	auto enemy = Unit::create(UnitShapeType::REC, ENEMY_MASK, TAG_ENEMY);
	//	enemy->addComponent(EnemyAttackRoutine::create(EnemyAttackType::TRIPLE));
	//	enemy->addComponent(EnemyMovementRoutine::create(EnemyMovementType::FROM_LEFT));
	//	enemy->setPosition(Vec2(720 / 2 - 300, 1280 / 2 + 300));
	//	addChild(enemy);
	//}
	//{
	//	auto enemy = Unit::create(UnitShapeType::REC, ENEMY_MASK, TAG_ENEMY);
	//	enemy->addComponent(EnemyAttackRoutine::create(EnemyAttackType::TRIPLE));
	//	enemy->addComponent(EnemyMovementRoutine::create(EnemyMovementType::FROM_TOP));
	//	enemy->setPosition(Vec2(720 / 2, 1280 / 2 + 200));
	//	addChild(enemy);
	//}
	//{
	//	auto enemy = Unit::create(UnitShapeType::REC, ENEMY_MASK, TAG_ENEMY);
	//	enemy->addComponent(EnemyAttackRoutine::create(EnemyAttackType::TRIPLE));
	//	enemy->addComponent(EnemyMovementRoutine::create(EnemyMovementType::FROM_RIGHT));
	//	enemy->setPosition(Vec2(720 / 2 + 300, 1280 / 2 + 300));
	//	addChild(enemy);
	//}
}

Unit * SceneIngame::getPlayer()
{
	return player;
}

void SceneIngame::onKeyPressed(EventKeyboard::KeyCode c, Event * e)
{
	const bool value = true;
	switch (c)
	{
	case EventKeyboard::KeyCode::KEY_W:up = value; break;
	case EventKeyboard::KeyCode::KEY_S:down = value; break;
	case EventKeyboard::KeyCode::KEY_A:left = value; break;
	case EventKeyboard::KeyCode::KEY_D:right = value; break;
	case EventKeyboard::KeyCode::KEY_SPACE:fire = value; break;
	}
}

void SceneIngame::onKeyReleased(EventKeyboard::KeyCode c, Event * e)
{
	const bool value = false;
	switch (c)
	{
	case EventKeyboard::KeyCode::KEY_W:up = value; break;
	case EventKeyboard::KeyCode::KEY_S:down = value; break;
	case EventKeyboard::KeyCode::KEY_A:left = value; break;
	case EventKeyboard::KeyCode::KEY_D:right = value; break;
	case EventKeyboard::KeyCode::KEY_SPACE:fire = value; break;
	}
}

bool SceneIngame::onContactBegin(PhysicsContact & contact)
{
	int tagA = contact.getShapeA()->getBody()->getTag();
	int tagB = contact.getShapeB()->getBody()->getTag();
	Node* a = contact.getShapeA()->getBody()->getNode();
	Node* b = contact.getShapeB()->getBody()->getNode();
	if (tagA == TAG_PLAYER_BULLET && tagB == TAG_ENEMY)
		b->removeFromParent();
	if (tagB == TAG_PLAYER_BULLET && tagA == TAG_ENEMY)
		a->removeFromParent();

	//플레이어 사망
	if (tagA == TAG_PLAYER && tagB == TAG_ENEMY)
	{
		a->removeFromParent();
		player = nullptr;
	}
		
	if (tagB == TAG_PLAYER && tagA == TAG_ENEMY)
	{
		b->removeFromParent();
		player = nullptr;
	}
		
	if (tagA == TAG_PLAYER && tagB == TAG_ENEMY_BULLET)
	{
		a->removeFromParent();
		player = nullptr;
	}
		
	if (tagB == TAG_PLAYER && tagA == TAG_ENEMY_BULLET)
	{
		b->removeFromParent();
		player = nullptr;
	}
		

	return true;
}

// 이동값 할당
void SceneIngame::logic(float dt)
{
	if (!player) return;
	auto cam = Camera::getDefaultCamera(); //카메라 생성
	Vec2 pos = player->getPosition();
	
	if (up) pos += Vec2(0, dt*PLAYER_MOVEMENT_SPEED);
	if (down) pos -= Vec2(0, dt*PLAYER_MOVEMENT_SPEED);
	if (left) pos -= Vec2(dt*PLAYER_MOVEMENT_SPEED, 0);
	if (right) pos += Vec2(dt*PLAYER_MOVEMENT_SPEED, 0);
	player->setPosition(pos);
	
	// 카메라 이동
	if(pos.x > 1280/2)
		cam->setPositionX(pos.x);
	if (pos.y > 720 / 2)
		cam->setPositionY(pos.y);
	
	if (fire)
	{
		auto bullet = Bullet::create(PLAYER_BULLET_MASK, TAG_PLAYER_BULLET);
		bullet->setPosition(player->getPosition());
		bullet->getBody()->setVelocity(Vec2(0,1800));
		bullet->runAction(Sequence::create(DelayTime::create(0.1f), RemoveSelf::create(), nullptr));
		addChild(bullet);
	}
}
//모양 변경 확인을 위한 마우스 클릭
void SceneIngame::onMouseDown(cocos2d::Event* event)
{
	player->removeFromParent();
	player = nullptr;
	player = Unit::create(UnitShapeType::PEN, PLAYER_MASK, TAG_PLAYER);
	player->setPosition(Vec2(720 / 2, 1280 / 2 - 200));
	addChild(player);
}
