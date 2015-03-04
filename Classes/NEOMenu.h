//NEOMenu.h
#ifndef __NEOMENU_H__
#define __NEOMENU_H__

#include "cocos2d.h"

USING_NS_CC;

class NEOMenu:public cocos2d::Layer{
public:
	CREATE_FUNC(NEOMenu);  
	 void addMenuItem(cocos2d::MenuItem *item); 
	virtual bool init();
private:
	float _index;
	float _lastindex;
	cocos2d::Vector<cocos2d::MenuItem *> _items;  
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);

};
#endif