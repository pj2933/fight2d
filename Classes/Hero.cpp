#include"Hero.h"

Hero::Hero(){
	CCLog("in Hero");}
Hero::~Hero(){}
bool Hero::init(){
	if(!Player::initWithSpriteFrameName("hero_idle_00.png")){
		return false;
	}
	
	// 之前的CallFunc::create(this, callfunc_selector(Hero::idle))已经没了，现在用c++11特性来回调
	CallFunc *callbackIdle=CallFunc::create(std::bind(&Hero::idle,this));

	 // 创建空闲动画，反复播放
     Animation *idle = createAnimation("hero_idle_%02d.png", 6, 12);
     setIdleAction(RepeatForever::create(Animate::create(idle)));

	 //行走动画
	Animation *walk = createAnimation("hero_walk_%02d.png", 7, 14);
	this->setWalkAction(RepeatForever::create(Animate::create(walk)));

	 //攻击动画,攻击完后处于空闲状态
	 Animation *attack = createAnimation("hero_attack_00_%02d.png", 3, 20);
     setAttackAction(Sequence::create(Animate::create(attack), callbackIdle, NULL));

	 //受伤动画
	 Animation *hurt=createAnimation("hero_hurt_%02d.png", 3, 15);
	 setHurtAction(Sequence::create(Animate::create(hurt),callbackIdle,NULL));
	  
	 // 创建死亡动画，播放后不做任何调用
     Animation *dead = createAnimation("hero_knockout_%02d.png", 5, 12);
	 setDeadAction(Sequence::create(Animate::create(dead), NULL));

	 setHP(100);
	 setAttack(10);
	 setSpeed(2);
	 return true;
}	