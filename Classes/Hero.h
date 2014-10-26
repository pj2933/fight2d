#ifndef _HERO_H_
#define _HERO_H_

#include"cocos2d.h"
#include"Player.h"
using namespace cocos2d;
class Hero :public Player{
public:
	Hero();
	~Hero();
	bool init();
	CREATE_FUNC(Hero);
};

#endif