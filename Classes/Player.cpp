#include"Player.h"
Player ::Player(){
	idleAction=NULL;
	walkAction=NULL;
	attackAction=NULL;
	hurtAction=NULL;
	deadAction=NULL;
	
}

Player::~Player(){}

//是否能切换到相应状态，若能则做相应的动作
void Player::idle(){
	if(ChangeState(ACTION_STATE_IDLE)){
		runAction(idleAction);
	}
}

void Player::walk(){
	if(ChangeState(ACTION_STATE_WALK)){
		runAction(walkAction);
	}
}

void Player::attack(){
	if(ChangeState(ACTION_STATE_ATTACK)){
		runAction(attackAction);
	}
}

void Player::hurt(){
	if(ChangeState(ACTION_STATE_HURT)){
		runAction(hurtAction);
	}
}

void Player::dead(){
	if(ChangeState(ACTION_STATE_DEAD)){
		runAction(deadAction);
	}
}

bool Player::ChangeState(ActionState state)
{
    bool ret = false;

    // 精灵已经被击倒（Game Over），就不能再出发其他动作了！
    if (currentState == ACTION_STATE_DEAD) {
        goto change_state_failed;
    }
    
    // 精灵已经处于要改变的状态，就没必要在改变了！
    if (currentState == state) {
        goto change_state_failed;
    }
	/*
	if(currentState==ACTION_STATE_HURT&&state==ACTION_STATE_ATTACK){
		goto change_state_failed;
	}*/

    // 改变动作之前，先停止所有动作
    this->stopAllActions();

	currentState = state;
    ret = true;

change_state_failed:
    return ret;
}

Animation* Player::createAnimation(const char* fileName,int count,float fps){
	Vector<SpriteFrame*> frames;
	int i;
	for (i=0;i<count;i++){
		const char *png = String::createWithFormat(fileName, i)->getCString();
        SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(png);
		frames.pushBack(frame);
	}
	return Animation::createWithSpriteFrames(frames, 1 / fps);
}