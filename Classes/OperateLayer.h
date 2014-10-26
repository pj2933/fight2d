#ifndef _OPERATELAYER_H_
#define _OPERATELAYER_H_

#include "cocos2d.h";
USING_NS_CC;
class OperateDelegate{
	public:
	//三个纯虚函数
    // 移动，direction为向量，distance是与起点的直线距离
    virtual void onWalk(cocos2d::Point direction, float distance) =0;
    // 攻击
    virtual void onAttack()=0;
    // 停止移动
    virtual void onStop() =0;
};
class OperateLayer : public cocos2d::Layer
{
public:


    virtual bool init();
    CREATE_FUNC(OperateLayer);

    // 触控的三个事件函数重载
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
	


    // 委托
    CC_SYNTHESIZE(OperateDelegate*, delegator, Delegator);

private:
    // 摇杆，分为“摇杆”、“摇杆基座”两个部分
    cocos2d::Sprite *joystick;
    cocos2d::Sprite *joystick_bg;
    // 激活“摇杆精灵”，并更新其坐标
    void activityJoystick(cocos2d::Point position);
    // 隐藏“摇杆精灵”，并将“摇杆”置于“摇杆基座”中心
    void inactivityJoystick();
    // 刷新“摇杆”相对于“摇杆基座”的位置（根据触控手势）
    void updateJoystick(cocos2d::Point direction, float distance);
};
#endif