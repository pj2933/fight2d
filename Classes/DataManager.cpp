#include"DataManager.h"
#include "cocos2d.h"
#include "base\CCUserDefault.h"
USING_NS_CC;

DataManager* DataManager::instance=0;
DataManager::DataManager(){
	this->init();
}
void DataManager::init(){
	setAtk(10);
	setHP(100);
}
DataManager* DataManager::getInstance(){
	if(instance==0)
		instance=new DataManager();
	return instance;
}
