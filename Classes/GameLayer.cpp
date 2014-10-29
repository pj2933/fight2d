#include "GameLayer.h"
#include "GameConfig.h"
#include "DataManager.h"
#include "GameScene.h"
//GameLayer初始化
bool GameLayer::init(){
	if(!Layer::init()){
		return false;
	}
	auto dataManager=DataManager::getInstance();
	//加载地图
	map=TMXTiledMap::create("pd_tilemap.tmx");
	this->addChild(map);

	//将图片序列资源加载到缓存中
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pd_sprites.plist");
	
	dataManager->setAtk(10);
	dataManager->setHP(100);
	hero=Hero::create();
	hero->setCurrentState(ACTION_STATE_NONE);
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
		/*char a[30];
		printf(a,"%d",(int)hero->getHP());
		CCLog(a);*/
	if(hero->getCurrentState()==ACTION_STATE_DEAD)
		return;
	Object *object = NULL;

	CCARRAY_FOREACH(robots, object) {
        Robot *robot = (Robot*)object;
		auto dataManager=DataManager::getInstance();
        if (Collision(hero, robot)) {
            robot->hurt();
			robot->setHP(robot->getHP() - dataManager->getAtk());
			float hp=robot->getHP();
			
            if (robot->getHP() <= 0) {
                // 延时是为了等待dead
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
		hero->setLocalZOrder(200-hero->getPositionY());
	}
	updateRobots();
}

void GameLayer::addRobot(){
	Robot *robot=Robot ::create();
	robot->onAttack = std::bind(&GameLayer::onRobotAttack, this, robot);
	Point location=Point::ZERO;
	//在英雄周围随机生成机器人
	location.x=hero->getPositionX()+CCRANDOM_MINUS1_1()*SCREEN.width/1.5;
	//若是这个点超出地图范围，则移进来
	location.x=(location.x<30)?(SCREEN.width+location.x):location.x;
	location.x=(location.x>(MAX_POSITION_X-30))?(location.x-SCREEN.width):location.x;

	location.y=CCRANDOM_0_1()*MAX_POSITION_Y;
	if(location.y<MIN_POSITION_Y){
		location.y=MIN_POSITION_Y;
	}
	robot->setCurrentState(ACTION_STATE_NONE);
	robot->idle();
	robot->setPosition(location);
	
	robots->addObject(robot);
	this->addChild(robot);
}
void GameLayer::onRobotAttack(Player *robot){
	auto dataManager=DataManager::getInstance();
	if (Collision(robot, hero)) {
		dataManager->setHP(dataManager->getHP() - robot->getAttack());
        if (dataManager->getHP() <= 0) {
            hero->dead();
			//英雄死亡游戏结束

			endgame();
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
		/*if (robot->getCurrentState() == ACTION_STATE_DEAD) {
            robots->removeObject(object);
            continue;
        }*/
		if(robot->getCurrentState()==ACTION_STATE_WALK){
		
			Point location = robot->getPosition();
            Point direction = robot->getDirection();
            Point expect = location + direction;
            if (expect.y > 110 || expect.y < 40) {
                direction.y = 0;
            }
            robot->setFlipX(direction.x < 0 ? true : false);
			
            robot->setPosition(location + direction);
			//调整位置防止出现错误遮盖
			robot->setLocalZOrder(200-robot->getPositionY());


	}
	}
}
void GameLayer::RemoveRobot(Player *robot)
{
    robots->removeObject(robot);
    this->removeChild(robot, true);
}


bool GameLayer::Collision(Sprite *attacker,Sprite *target){
	Point attackerLocation=attacker->getPosition();
	Point targetLocation=target->getPosition();
	Point offset=attackerLocation-targetLocation;
	bool isFlipX=attacker->isFlipX();
	bool isTargetLeft=(attackerLocation.x<targetLocation.x)?true:false;
	//没有翻转的“攻击者”可以打到其右边的单位，反之打到左边
	if((!isFlipX&&isTargetLeft)||(isFlipX&&!isTargetLeft)){
		if(abs(offset.x)<=65&&abs(offset.y)<=35)
			return true;
	}
	return false;

}
void GameLayer::endgame(){
	this->pauseSchedulerAndActions();
	Object *object=NULL;
	
	CCARRAY_FOREACH(robots,object){
		Robot *robot=(Robot*) object;
		robot->idle();
	}
	auto closeItem = MenuItemImage::create(
                                           "again_off.png",
                                           "again_on.png",
										   CC_CALLBACK_1(GameLayer::menuRestartCallback, this));
	float locX=hero->getPositionX();
	if(hero->getPositionX()<SCREEN.width/2)
	{
		locX=SCREEN.width/2;
	}
	if(hero->getPositionX()>map->getContentSize().width-SCREEN.width/2){
		locX=map->getContentSize().width-SCREEN.width/2;
	}
	closeItem->setPosition(locX,SCREEN.height/2);
	auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 500);
}
void GameLayer::menuRestartCallback(Ref* pSender)
{
	auto director=Director::getInstance();
	auto scene = GameScene::create();
    // run
	director->replaceScene(scene);
}