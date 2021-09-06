#include "stdafx.h"
#include "EnemyComponent.h"
#include "SceneIngame.h"
#include "Bullet.h"



EnemyAttackRoutine * EnemyAttackRoutine::create(EnemyAttackType t)
{
	auto ret = new EnemyAttackRoutine();
	if (ret && ret->init(t)) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool EnemyAttackRoutine::init(EnemyAttackType t)
{
	if (!Component::init())return false;

	this->type = t;
	this->setName("EnemyAttack");
	return true;
}

void EnemyAttackRoutine::update(float dt)
{
	switch (this->type)
	{
	case EnemyAttackType::NORMAL:
		logic_normal(dt); 
		break;
	case EnemyAttackType::DOUBLE:
		logic_double(dt);
		break;
	case EnemyAttackType::TRIPLE:
		logic_triple(dt); 
		break;
	}
}

void EnemyAttackRoutine::logic_normal(float dt)
{
	elapsed += dt;
	if (elapsed > 0.2f)
	{
		elapsed = 0.0f;
		createBulletAndGo(ENEMY_BULLET_SPEED, 0);
	}
}

void EnemyAttackRoutine::logic_double(float dt)
{
	elapsed += dt;
	if (elapsed > 0.5f)
	{
		elapsed = 0.0f;
		createBulletAndGo(ENEMY_BULLET_SPEED, 30);
		createBulletAndGo(ENEMY_BULLET_SPEED, -30);
	}
}

void EnemyAttackRoutine::logic_triple(float dt)
{
	elapsed += dt;
	if (elapsed > 0.2f)
	{
		elapsed = 0.0f;
		createBulletAndGo(ENEMY_BULLET_SPEED, 0);
		createBulletAndGo(ENEMY_BULLET_SPEED, 20);
		createBulletAndGo(ENEMY_BULLET_SPEED, -20);
	}
}

void EnemyAttackRoutine::createBulletAndGo(float speed, float delta_angle)
{
	SceneIngame* scene = (SceneIngame*)Director::getInstance()->getRunningScene();
	auto player = scene->getPlayer();
	if (player == nullptr) return;

	auto bullet = Bullet::create(ENEMY_BULLET_MASK, TAG_ENEMY_BULLET);
	bullet->setPosition(getOwner()->getPosition());
	scene->addChild(bullet);

	Vec2 pos = player->getPosition() - getOwner()->getPosition();	//종점 - 시점(적 -> 플레이어)
	pos = pos.getNormalized() * speed;
	pos = pos.rotateByAngle(Vec2::ZERO, GET_RAD(delta_angle));
	bullet->getBody()->setVelocity(pos);
	bullet->runAction(Sequence::create(DelayTime::create(3.0f), RemoveSelf::create(), nullptr));
}

EnemyMovementRoutine * EnemyMovementRoutine::create(EnemyMovementType t)
{
	auto ret = new EnemyMovementRoutine();
	if (ret && ret->init(t)) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool EnemyMovementRoutine::init(EnemyMovementType t)
{
	if (!Component::init()) return false;

	this->type = t;
	this->setName("EnemyMovement");
	
	return true;
}

void EnemyMovementRoutine::onEnter()
{
	Component::onEnter();

	const float velocity = 300;

	u = (Unit*)getOwner();
	
	switch (this->type)
	{
	case EnemyMovementType::FROM_TOP:
		u->setPosition(Vec2(1280 / 2 + rand() % 400 - 200, 720 + 50));
		u->getPhysicsBody()->setVelocity(Vec2(0, -100).getNormalized() * velocity);
		break;
	case EnemyMovementType::FROM_LEFT:
		u->setPosition(Vec2(-50, 600 + rand() % 200 - 100));
		u->getPhysicsBody()->setVelocity(Vec2(300, -150).getNormalized() * velocity);
		break;
	case EnemyMovementType::FROM_RIGHT:
		u->setPosition(Vec2(1280+50, 600 + rand() % 200 - 100));
		u->getPhysicsBody()->setVelocity(Vec2(-300, -150).getNormalized()* velocity);
		break;
	}

	
	//u->runAction(Sequence::create(DelayTime::create(5.0f), RemoveSelf::create(), nullptr));
}

void EnemyMovementRoutine::update(float dt)
{
	switch (type)
	{
	case EnemyMovementType::FROM_TOP:logic_from_top(dt); break;
	case EnemyMovementType::FROM_LEFT:logic_from_left(dt); break;
	case EnemyMovementType::FROM_RIGHT:logic_from_right(dt); break;
	}
}

const Vec2 impulse_from_top = Vec2(0, 1500).getNormalized() * 300;
const Vec2 impulse_from_left = Vec2(-1500, 300).getNormalized() * 300;
const Vec2 impulse_from_right = Vec2(1500, 300).getNormalized() * 300;

void EnemyMovementRoutine::logic_from_top(float dt)
{
	if (u == nullptr) return;
	u->getBody()->applyImpulse(impulse_from_top);
}

void EnemyMovementRoutine::logic_from_left(float dt)
{
	if (u == nullptr) return;
	u->getBody()->applyImpulse(impulse_from_left);
}

void EnemyMovementRoutine::logic_from_right(float dt)
{
	if (u == nullptr) return;
	u->getBody()->applyImpulse(impulse_from_right);
}
