#include "GameScene.h"

bool GameScene::init(){
	if(!Scene::init()){
		return false;
	}
	this->setGameLayer(GameLayer::create());
	this->setOperateLayer(OperateLayer::create());

	operateLayer->setDelegator(gameLayer);
	//讲游戏层和操作层添加到场景中
	this->addChild(gameLayer,0);
	this->addChild(operateLayer,1);
	
	return true;
}