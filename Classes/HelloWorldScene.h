#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"

USING_NS_CC;


class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void onKeyPressed(EventKeyboard::KeyCode, Event*);
	void onKeyReleased(EventKeyboard::KeyCode, Event*);
	void setViewpointCenter(CCPoint position);
	void update(float dt) override;
	void mapInit();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	Node* RootNode = NULL;
	Node* Player = NULL;
	CCTMXTiledMap* map = NULL;
	int run = 0;
	boolean maprun = false;
	boolean isMaprun = false;
};

#endif // __HELLOWORLD_SCENE_H__
