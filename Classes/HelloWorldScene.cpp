#include "HelloWorldScene.h"
#include "Player.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();

	//DebugModel
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	scheduleUpdate();
    auto rootNode = CSLoader::createNode("MainScene.csb");
	auto player = rootNode->getChildByName("Player");
	map = dynamic_cast<CCTMXTiledMap*>(rootNode->getChildByName("Map"));
	Player = player;
	RootNode = rootNode;
	auto keyEvent = EventListenerKeyboard::create();
	keyEvent->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyEvent, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//eage
	//Size mapSize = map->getMapSize() * map->getTileSize;Size(map->getMapSize().height*)
	Size mapSize = map->getContentSize();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto body = PhysicsBody::createEdgeBox(mapSize,
		PHYSICSBODY_MATERIAL_DEFAULT, 5.0f);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(mapSize.width / 2, mapSize.height / 2));
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);
	mapInit();

	//mapcover
	auto xx = map->getLayer("wall");
	//Player
	auto sp = dynamic_cast<Sprite*>(Player);
	auto Playerbody = PhysicsBody::createBox(sp->getContentSize());
	sp->setPhysicsBody(Playerbody);

    addChild(rootNode);

    return true;
}
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {

		float speed = 30;
		int tag = 0;
		log(run);

		CCLOG("X:%f", RootNode->convertToWorldSpace(Player->getPosition()).x);

		Vec2  movespeed = Vec2(0, 0);
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:movespeed = Vec2(speed, 0); tag = 1; break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:movespeed = Vec2(-speed, 0); tag = 2; break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:movespeed = Vec2(0, speed); tag = 3; break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:movespeed = Vec2(0, -speed); tag = 4;  break;
		}
		run += tag;
		auto moveBy = MoveBy::create(0.1f, movespeed);
		auto action = RepeatForever::create(moveBy);
		action->setTag(tag);

		Player->runAction(action);


	}
}
void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {

		int tag = 0;
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: tag = 1; break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW: tag = 2;  break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW: tag = 3; break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW: tag = 4;  break;
		}
		Player->stopActionByTag(tag);
		run -= tag;

	}
}
void HelloWorld::setViewpointCenter(CCPoint position)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float x = MAX(position.x, winSize.width / 2);
	float y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (map->getMapSize().width*map->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (map->getMapSize().height*map->getTileSize().height) - winSize.height / 2);

	CCPoint acturalPosition = ccp(x, y);

	CCPoint centerView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint viewPoint = ccpSub(centerView, acturalPosition);
	this->setPosition(viewPoint);
}
void HelloWorld::update(float dt) {
	setViewpointCenter(Player->getPosition());
}
void HelloWorld::mapInit() {
	auto wall = map->getLayer("wall");
	int x = wall->getLayerSize().width;
	int y = wall->getLayerSize().height;
	for (int i = 0; i < x; i++) {
		for (int u = 0; u < y; u++) {
			auto sp = wall->getTileAt(Vec2(i, u));
			if (sp == NULL) {
				continue;
			}
			auto spbody = PhysicsBody::createBox(Size(32, 32));
			spbody->setDynamic(false);
			sp->setPhysicsBody(spbody);
		}
	}
	log("0.0");
}
