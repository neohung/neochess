#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
	TMXTiledMap* map;
	Sprite* hero;
	//Vec2 player_location;
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
    // a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	virtual	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event*event);
	virtual	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event*event);
    // implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
