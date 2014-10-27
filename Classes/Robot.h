#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "cocos2d.h"
#include "Player.h"
USING_NS_CC;

#define ROBOT_VIEW_FIELD 150
#define ROBOT_ATTACK_FIELD 50

//»úÆ÷ÈËAI
typedef enum{
	ROBOT_AI_STATE_WAIT = 0,
    ROBOT_AI_STATE_STAND,
    ROBOT_AI_STATE_PATROL,
    ROBOT_AI_STATE_FOLLOW,
    ROBOT_AI_STATE_ATTACK,
}RobotAIState;
class Robot:public Player{
public:
	bool init();
    CREATE_FUNC(Robot);
	void strategy(Point target);
	void execute(Point target);
	//std::function<void(void)> onAttack;
	std::function<void(void)> onAttack;
	//void onAttack(Player *hero);
	CC_SYNTHESIZE(RobotAIState,aiState,AIState);
	CC_SYNTHESIZE(Point,direction,Direction);
private :
	unsigned int excutedTime;

};

#endif