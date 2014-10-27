#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "cocos2d.h"
#include "GameLayer.h"
#include "OperateLayer.h"
USING_NS_CC;
class GameScene : public Scene{
public :
	virtual bool init();
	CREATE_FUNC(GameScene);
	//游戏层的get/set
	CC_SYNTHESIZE(GameLayer*, gameLayer, GameLayer);
	//操作层的get/set
	CC_SYNTHESIZE(OperateLayer*,operateLayer,OperateLayer);
};

#endif