
#ifndef __SCENE_INGAME_H__ 
#define __SCENE_INGAME_H__
#include "stdafx.h"
#include "Unit.h"

class SceneIngame : public Scene
{
private:
	Unit* player = nullptr;
	bool up= false, down=false, left=false, right=false;
	bool fire = false;
public:
	static SceneIngame* create();
	virtual bool init() override;
	virtual void onEnter() override;

	Unit* getPlayer();

	void onKeyPressed(EventKeyboard::KeyCode c, Event* e);
	void onKeyReleased(EventKeyboard::KeyCode c, Event* e);
	void onMouseDown(cocos2d::Event* event); //���콺 Ŭ���� ��� �ٲ�� �ϱ� ���� ���콺 �̺�Ʈ �������̵�

	bool onContactBegin(PhysicsContact& contact);

	void logic(float dt);

};

#endif