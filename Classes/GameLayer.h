#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include"cocos2d.h"
#include"Hero.h"
#include"Robot.h"
#include "OperateLayer.h"
USING_NS_CC;
//可活动区域
#define MAX_POSITION_X 3800
#define MIN_POSITION_X 0
#define MAX_POSITION_Y 110
#define MIN_POSITION_Y 40

class GameLayer :public Layer,public OperateDelegate{
public:
	virtual bool init();
	CREATE_FUNC(GameLayer);
	//实现operatedelegate的三个函数
    void onWalk(cocos2d::Point direction, float distance);
	void onStop();
	void onAttack();
	//每帧的更新函数
	void update(float dt);
	//添加机器人
	void addRobot();
	//每帧的机器人更新
	void updateRobots();
	//实现机器人攻击英雄的逻辑
	void onRobotAttack(Player *robot);
	//移除机器人
	void RemoveRobot(Player *robot);
	//碰撞
	bool Collision(Sprite *attacker,Sprite *target);
	//结束游戏
	void endgame();
	//重启键的回调函数
	void menuRestartCallback(Ref* pSender);

	
private:
	TMXTiledMap *map;//地图
	SpriteBatchNode *actor;//将敌方和友方单位作为actor的子节点方便一次性绘制，提高效率
	Player* hero;//英雄，玩家控制
	Point Hero_speed;//英雄的移动速度
	Array *robots;//机器人数组
};
#endif