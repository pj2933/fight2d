#include "GameLayer.h"
#include "GameConfig.h"
//GameLayer初始化
bool GameLayer::init(){
	if(!Layer::init()){
		return false;
	}
	
	//加载地图
	map=TMXTiledMap::create("pd_tilemap.tmx");
	this->addChild(map);
	
	//将图片序列资源加载到缓存中
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pd_sprites.plist");
	actor=SpriteBatchNode::create("pd_sprites.pvr.ccz");
	this->addChild(actor);
	actor->retain();
	hero=Hero::create();
	hero->setPosition(SCREEN.width/10,SCREEN.height/4);
	hero->idle();
	this->addChild(hero);
	
	robots=Array::createWithCapacity(6);
	robots->retain();
	int i=0;
	while(i<6){
		addRobot();
		i++;
	}


	this->scheduleUpdate();
	return true;
}
void GameLayer::onWalk(Point direction,float distance){
	//根据摇杆 改变人物朝向
	if(hero->getCurrentState()!=ACTION_STATE_DEAD)
	hero->setFlipX(direction.x < 0 ? true : false);
	hero->walk();
	//根据摇杆的位置，选择1倍速和2倍速
	Hero_speed=hero->getSpeed()*direction*((distance < 64) ? 1 : 1.5);
}
void GameLayer::onStop(){
	 hero->idle();
}
void GameLayer::onAttack(){
		hero->attack();

	Object *object = NULL;

	CCARRAY_FOREACH(robots, object) {
        Robot *robot = (Robot*)object;

        if (Collision(hero, robot)) {
            robot->hurt();
			robot->setHP(robot->getHP() - hero->getAttack());
			float hp=robot->getHP();
			char a[30];
			sprintf(a,"%d",(int)hp);
			CCLog(a);
            if (robot->getHP() <= 0) {
                // 延时是为了等待knockout
                DelayTime *delay = DelayTime::create(1);
                Blink *blink = Blink::create(1, 4);
				CallFunc *remove = CallFunc::create(std::bind(&GameLayer::RemoveRobot, this, robot));
                Action *die = Sequence::create(delay, blink, remove, NULL);
                robot->dead();
                robot->runAction(die);
            }
        }
    }
}

void GameLayer::update(float dt){
	float mapWidth=map->getContentSize().width;

	if(hero->getCurrentState()==ACTION_STATE_WALK){
		//期望在的点和实际在的点
		Point expect=hero->getPosition()+Hero_speed;
		Point actual=expect;
		//不让hero走出底部，也不能走到墙壁上去
		if(expect.y<40||expect.y>SCREEN.height/2.7){
			actual.y=hero->getPosition().y;
		}
		//根据hero的x轴位置，进行滚屏
		if(expect.x>SCREEN.width/2&&expect.x<(mapWidth-SCREEN.width/2)){
			this->setPositionX(this->getPositionX() - Hero_speed.x);
		}else if (expect.x < 35 || expect.x > mapWidth) {
            // 限制hero在x轴的移动范围（防止走出屏幕之外）
            actual.x = hero->getPositionX();
        }
		hero->setPosition(actual);
        //调整z轴位置，防止出现反常的遮盖
		hero->setZOrder(120-hero->getPositionY());
	}
	updateRobots();
}

void GameLayer::addRobot(){
	Robot *robot=Robot ::create();
	robot->onAttack= std::bind(&GameLayer::RobotAttack, this, robot);
	Point location;
	
	location.x=hero->getPositionX()+CCRANDOM_MINUS1_1()*SCREEN.width/1.5;
	location.x=(location.x<30)?(SCREEN.width+location.x):location.x;
	location.x=(location.x>(MAX_POSITION_X-30))?(location.x-SCREEN.width):location.x;

	location.y=CCRANDOM_0_1()*MAX_POSITION_Y;
	if(location.y<MIN_POSITION_Y){
		location.y=MIN_POSITION_Y;
	}
	robot->setPosition(location);
	robot->idle();
	robots->addObject(robot);
	actor->addChild(robot);
}
void GameLayer::RobotAttack(Player *robot){
	if (Collision(robot, hero)) {
        hero->setHP(hero->getHP() - robot->getAttack());
		char a[30];
		printf(a,"%d",(int)hero->getHP());
		CCLog(a);
        if (hero->getHP() <= 0) {
            hero->dead();
        } else {
            hero->hurt();
        }
	}
}

void GameLayer::updateRobots(){
	Object *object=NULL;
	while(robots->count()<6){
		addRobot();
	}
	
	CCARRAY_FOREACH(robots,object){
		Robot *robot=(Robot*) object;
		robot->execute(hero->getPosition());
		if (robot->getCurrentState() == ACTION_STATE_DEAD) {
            robots->removeObject(object);
            continue;
        }
		if(robot->getCurrentState()==ACTION_STATE_WALK){
		/*	Point expect=robot->getPosition()+robot->getDirection();
			Point actual=expect;
			if(expect.x>MAX_POSITION_X-30)
				actual.x=MAX_POSITION_X-30;
			if(expect.x<MIN_POSITION_X+30)
				actual.x=MIN_POSITION_X+30;
			if(expect.y<40||expect.y>SCREEN.height/2.7){
			actual.y=hero->getPosition().y;
			robot->setFlipX(robot->getDirection().x < 0 ? true : false);
			robot->setPosition(actual);
			robot->setZOrder(120-robot->getPositionY());
			}
			*/
			Point location = robot->getPosition();
            Point direction = robot->getDirection();
            Point expect = location + direction;
            if (expect.y > 110 || expect.y < 40) {
                direction.y = 0;
            }
            robot->setFlipX(direction.x < 0 ? true : false);
			
            robot->setPosition(location + direction);
            robot->setZOrder(robot->getPositionY());


	}
	}
}
void GameLayer::RemoveRobot(Player *robot)
{
    robots->removeObject(robot);
    actor->removeChild(robot, true);
}


bool GameLayer::Collision(Sprite *attacker,Sprite *target){
	Point attackerLocation=attacker->getPosition();
	Point targetLocation=target->getPosition();
	Point offset=attackerLocation-targetLocation;
	bool isFlipX=attacker->isFlipX();
	bool isTargetLeft=(attackerLocation.x<targetLocation.x)?true:false;
	if((!isFlipX&&isTargetLeft)||(isFlipX&&!isTargetLeft)){
		if(abs(offset.x)<=65&&abs(offset.y)<=35)
			return true;
	}
	return false;

}