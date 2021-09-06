#ifndef __UNIT_H__
#define __UNIT_H__

#include"stdafx.h"
enum class UnitShapeType
{
	TRI, REC, PEN, HEX, OCT
};
class Unit : public Node
{
private:
	UnitShapeType type;
	DrawNode* dn;
	PhysicsBody* body;
protected:
	Sprite* spr;
public:
	static Unit* create(UnitShapeType t, int bitmask, int tag);
	virtual bool init(UnitShapeType t, int bitmask, int tag);

	bool logic_tri(int bitmask, int tag);
	bool logic_rec(int bitmask, int tag);
	bool logic_pen(int bitmask, int tag);
	bool logic_hex(int bitmask, int tag);
	bool logic_oct(int bitmask, int tag);

	PhysicsBody* getBody();
};

#endif