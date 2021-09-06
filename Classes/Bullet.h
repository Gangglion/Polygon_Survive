#ifndef __Bullet_H__
#define __Bullet_H__

#include"stdafx.h"

class Bullet : public Node
{
private:
	PhysicsBody* body;
public:
	static Bullet* create(int bitmask, int tag);
	virtual bool init(int bitmask, int tag);

	PhysicsBody* getBody();
};

#endif
