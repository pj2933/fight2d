#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include"cocos2d.h"
#include"Player.h"
#include"Hero.h"
#include"Robot.h"
#include "OperateLayer.h"
USING_NS_CC;

#define MAX_POSITION_X 3800
#define MIN_POSITION_X 0
#define MAX_POSITION_Y 110
#define MIN_POSITION_Y 40

class GameLayer :public Layer,public OperateDelegate{
public:
	virtual bool init();
	CREATE_FUNC(GameLayer);
    void onWalk(cocos2d::Point direction, float distance);
	void onStop();
	void onAttack();
	void update(float dt);
	void addRobot();
	void updateRobots();
	void RobotAttack(Player* robot);
	void RemoveRobot(Player *robot);
	bool Collision(Sprite *attacker,Sprite *target);
	//CC_SYNTHESIZE(Hero*,hero,Hero);
	Player* hero;
private:
	TMXTiledMap *map;//地图
	SpriteBatchNode *actor;//将敌方和友方单位作为actor的子节点方便一次性绘制，提高效率
	
	Point Hero_speed;
	Array *robots;
};
#endif