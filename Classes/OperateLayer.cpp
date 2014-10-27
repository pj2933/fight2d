#include"OperateLayer.h"
#include"GameConfig.h"

bool OperateLayer::init(){
	if(!Layer::init()){
		return false;
	}
	 joystick = Sprite::create("joystick.png");
     joystick_bg = Sprite::create("joystick_bg.png");
     this->addChild(joystick_bg);
     this->addChild(joystick);
	 inactivityJoystick();
     //setTouchEnabled(true);
	 auto listener = EventListenerTouchAllAtOnce::create();
	 listener->onTouchesBegan = CC_CALLBACK_2(OperateLayer::onTouchesBegan, this);
	 listener->onTouchesMoved = CC_CALLBACK_2(OperateLayer::onTouchesMoved, this);
	 listener->onTouchesEnded = CC_CALLBACK_2(OperateLayer::onTouchesEnded, this);
	 _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	 return true;

}

void OperateLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event){
	
	for(auto &item:touches)
	{
		Touch *t = item;
		Point p=t->getLocation();
		//点击区域在触屏的左半边，激活摇杆
		if(p.x<SCREEN.width/2){
			this->activityJoystick(p);
		}
		else{
			//点击屏幕右侧，发动攻击
			this->delegator->onAttack();
		}
	}
}
void OperateLayer::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    std::vector<cocos2d::Touch*>::const_iterator setI = touches.begin();
	Touch *touchPoint=(Touch *)(*setI);
	Point start = touchPoint->getStartLocation();

    // 如果该触控的起点是右屏产生的，则不做“滑动”处理
	if (start.x > SCREEN.width / 2) {
        return;
    }

    Point p = touchPoint->getLocation();
    // 获取位移点与起始点的偏移量（直线距离）
    float distance = start.getDistance(p);
    // 转换得到起始点到位移点的向量（单位为1的坐标）
	Point direction = (p - start).getNormalized();

    this->updateJoystick(direction, distance);
    this->delegator->onWalk(direction, distance);
}
void OperateLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event){
	if(joystick_bg->isVisible()){
		this->inactivityJoystick();
		this->delegator->onStop();
	}
}

void OperateLayer::activityJoystick(Point position){
	joystick->setPosition(position);
	joystick->setVisible(true);
	joystick_bg->setPosition(position);
	joystick_bg->setVisible(true);
}
void OperateLayer::inactivityJoystick(){
	joystick->setPosition(joystick_bg->getPosition());
	joystick->setVisible(false);
	joystick_bg->setVisible(false);
}

void OperateLayer::updateJoystick(Point direction, float distance){
	Point bgPoint=joystick_bg->getPosition();
	//当手指移出摇杆，摇柄不能超出基座
	if(distance<64){
		joystick->setPosition(bgPoint+direction*distance);
	}
	else{
		joystick->setPosition(bgPoint+direction*64);
	}
}