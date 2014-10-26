#include"Robot.h"

bool Robot::init(){
	if(!Player::initWithSpriteFrameName("robot_idle_00.png")){
		return false;
	}
	CallFunc *callbackIdle = CallFunc::create(std::bind(&Robot::idle, this));
	
	//空闲
	Animation *idle = createAnimation("robot_idle_%02d.png", 5, 10);
    setIdleAction(RepeatForever::create(Animate::create(idle)));
	//行走
	Animation *walk = createAnimation("robot_walk_%02d.png", 6, 18);
	setWalkAction(RepeatForever::create(Animate::create(walk)));
	//攻击
	Animation *attack = createAnimation("robot_attack_%02d.png", 5, 30);
    setAttackAction(Sequence::create(Animate::create(attack), callbackIdle, NULL));

    // 创建hurt（受伤）动画，播放后回调到idle动画
    Animation *hurt = createAnimation("robot_hurt_%02d.png", 3, 20);
    setHurtAction(Sequence::create(Animate::create(hurt), callbackIdle, NULL));

    // 创建knockout（被击倒）动画，播放后不做任何调用
    Animation *dead = createAnimation("robot_knockout_%02d.png", 5, 12);
    setDeadAction(Sequence::create(Animate::create(dead), NULL));

	setAttack(15);
	setHP(40);
	setAIState(ROBOT_AI_STATE_WAIT);
	excutedTime=25;
	return true;
}
void Robot::strategy(Point target){
	Point myself=this->getPosition();
	float distance=myself.getDistance(target);
	if(distance>ROBOT_VIEW_FIELD){
		//视野外，机器人巡逻或者站着发呆
		if(CCRANDOM_MINUS1_1()>0) aiState=ROBOT_AI_STATE_STAND;
		else aiState=ROBOT_AI_STATE_PATROL;
	}
	else{
		if(distance>ROBOT_ATTACK_FIELD) aiState=ROBOT_AI_STATE_FOLLOW;
		else if(this->getCurrentState()==ACTION_STATE_IDLE)
			aiState=ROBOT_AI_STATE_ATTACK;
		else 
			aiState=ROBOT_AI_STATE_WAIT;
	}
	float directionX;
	switch(aiState){
	case ROBOT_AI_STATE_ATTACK:
		this->attack();
		this->onAttack(this);
		excutedTime=60;
		break;
	case ROBOT_AI_STATE_FOLLOW:
		this->walk();
		direction=((target-myself).getNormalized());
		excutedTime=10;
		break;
	case ROBOT_AI_STATE_PATROL:
		this->walk();
		directionX=CCRANDOM_MINUS1_1();
		direction=(Point(directionX,sqrtf(1-directionX*directionX)));
		excutedTime=CCRANDOM_0_1()*100;
		break;
	case ROBOT_AI_STATE_STAND:
		this->idle();
		excutedTime=CCRANDOM_0_1()*100;
		break;
	case ROBOT_AI_STATE_WAIT:
		this->idle();
		excutedTime=30;
		break;
	}
}
void Robot::execute(Point target){

	if(excutedTime==0)
		strategy(target);
	else
		excutedTime--;
}

      