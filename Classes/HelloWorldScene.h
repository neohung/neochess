#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    TMXTiledMap* map;
    Vec2 selected;
    std::vector<Vec2> needed_undo_index;
    std::vector<int> needed_undo_gid;
    Sprite* hero;
    char turn[6];
    //std::string turn;
    cocos2d::Sprite *bg_sprite[5];  
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    	/////////////////////////////
    void menuItem1Callback(cocos2d::Ref* pSender);  
    void menuItem2Callback(cocos2d::Ref* pSender);  
    void menuItem3Callback(cocos2d::Ref* pSender);  
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event*event);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event*event);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    Vec2 TouchpositionToTile(Point pt);
    Point TileIndexToPosition(Vec2 index);  
    Point TileIndexToMapPosition(Vec2 index);  
    void show_moveable(Vec2 index);

	////////////////////////////////
    // implement the "static create()" method manually
        CREATE_FUNC(HelloWorld);
private:
  
};

#endif // __HELLOWORLD_SCENE_H__
