#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "cocos2d.h"
USING_NS_CC;
//从plist得知五种状态
typedef enum {
    ACTION_STATE_NONE = 0,
    ACTION_STATE_IDLE,
    ACTION_STATE_WALK,
    ACTION_STATE_ATTACK,
    ACTION_STATE_HURT,
    ACTION_STATE_DEAD,
} ActionState;
class Player:public Sprite{
public:
	Player();
	~Player();
	
	//对应五种状态的函数
	void idle();
	void walk();
	void attack();
 	void hurt();
	void dead();

	//一些属性的get/set
	//为五种状态设置对应的动作
	CC_SYNTHESIZE_RETAIN(Action*,idleAction,IdleAction);
	CC_SYNTHESIZE_RETAIN(Action*,walkAction,WalkAction);
	CC_SYNTHESIZE_RETAIN(Action*,attackAction,AttackAction);
	CC_SYNTHESIZE_RETAIN(Action*,hurtAction,HurtAction);
	CC_SYNTHESIZE_RETAIN(Action*,deadAction,DeadAction);

	//精灵的当前状态
	CC_SYNTHESIZE(ActionState,currentState,CurrentState);

	//生命值攻击力
	CC_SYNTHESIZE(unsigned int , hp,HP);
	CC_SYNTHESIZE(unsigned int ,atk,Attack);
	CC_SYNTHESIZE(unsigned int ,speed,Speed);
	//改变角色状态
	bool ChangeState(ActionState state);

protected:
	// 定义一个创建状态动画的方法
    // filename   - 状态的图片名前缀(查看pd_sprites.plist,每种状态格式都类似hero_idle_xx.png)
    // count - 状态图片序列的数量
    // fps   - 动画的播放帧率
	static cocos2d::Animation* createAnimation(const char* fileName,int count,float fps);
};
#endif