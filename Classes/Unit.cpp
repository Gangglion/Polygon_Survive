#include"stdafx.h"
#include"Unit.h"

Unit * Unit::create(UnitShapeType t, int bitmask, int tag)
{
	auto ret = new Unit();
	if (ret && ret->init(t,bitmask,tag)) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool Unit::init(UnitShapeType t, int bitmask, int tag)
{
	if (!Node::init()) return false;
	this->type = t;
	switch (this->type)
	{
	case UnitShapeType::TRI:
		this->setName("TriShape");
		logic_tri(bitmask, tag);
		break;
	case UnitShapeType::REC:
		this->setName("RecShape");
		logic_rec(bitmask, tag);
		break;
	case UnitShapeType::PEN:
		this->setName("PenShape");
		logic_pen(bitmask, tag);
		break;
	case UnitShapeType::HEX:
		this->setName("HexShape");
		logic_hex(bitmask, tag);
		break;
	case UnitShapeType::OCT:
		this->setName("OctShape");
		logic_oct(bitmask, tag);
		break;
	}
	return true;
}

bool Unit::logic_tri(int bitmask, int tag)//삼각형
{
	Vec2 pointsTri[3] = { Vec2(0,0),Vec2(26,0),Vec2(13,26) };
	body = PhysicsBody::createPolygon(pointsTri, 3);
	body->setTag(tag);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(bitmask);
	addComponent(body);
	body->setRotationEnable(false);
	return true;
}

bool Unit::logic_rec(int bitmask, int tag)//사각형
{
	Vec2 pointsRec[4] = { Vec2(0,0),Vec2(26,0),Vec2(0,26), Vec2(26,26) };//사각형
	body = PhysicsBody::createPolygon(pointsRec, 4);
	body->setTag(tag);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(bitmask);
	addComponent(body);
	body->setRotationEnable(false);
	return true;
}
bool Unit::logic_pen(int bitmask, int tag)//오각형
{
	Vec2 pointsPen[5] = { Vec2(6.5,0),Vec2(19.5,0),Vec2(0,13),Vec2(26,13),Vec2(13,26) }; //오각형
	body = PhysicsBody::createPolygon(pointsPen, 5);
	body->setTag(tag);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(bitmask);
	addComponent(body);
	body->setRotationEnable(false);
	return true;
}
bool Unit::logic_hex(int bitmask, int tag)//육각형
{
	Vec2 pointsHex[6] = { Vec2(6.5,0),Vec2(19.5,0),Vec2(0,13),Vec2(26,13),Vec2(6.5,26),Vec2(19.5,26) }; //육각형
	body = PhysicsBody::createPolygon(pointsHex, 6);
	body->setTag(tag);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(bitmask);
	addComponent(body);
	body->setRotationEnable(false);
	return true;
}
bool Unit::logic_oct(int bitmask, int tag)//팔각형
{
	Vec2 pointsOct[8] = { Vec2(6.5,0),Vec2(19.5,0),Vec2(0,6.5),Vec2(0,19.5),Vec2(26,6.5),Vec2(26,19.5),Vec2(6.5,26),Vec2(19.5,26) }; //팔각형
	body = PhysicsBody::createPolygon(pointsOct, 8);
	body->setTag(tag);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(bitmask);
	addComponent(body);
	body->setRotationEnable(false);
	return true;
}

PhysicsBody * Unit::getBody()
{
	return body;
}
