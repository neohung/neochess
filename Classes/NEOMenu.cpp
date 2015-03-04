//NEOMenu.cpp
#include "NEOMenu.h"
USING_NS_CC;

bool NEOMenu::init(){
	_index=0; 
	_lastindex=0; 
auto size = Director::getInstance()->getWinSize();  
    this->setContentSize(size*(2.0/3));  
    this->setAnchorPoint(Vec2(0.5f, 0.5f));  
    auto listener = EventListenerTouchOneByOne::create();  
    listener->onTouchBegan = CC_CALLBACK_2(NEOMenu::onTouchBegan, this);  
    listener->onTouchMoved = CC_CALLBACK_2(NEOMenu::onTouchMoved, this);  
    listener->onTouchEnded = CC_CALLBACK_2(NEOMenu::onTouchEnded, this);  
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);  
    return true;
}

bool NEOMenu::onTouchBegan(Touch* touch, Event* event){  
	log("onTouchBegan");
	 auto size = getContentSize();  
	log ("%lf,%lf",getContentSize().width,getContentSize().height);
	 auto position = this->convertToNodeSpace(touch->getLocation());  
  	 auto rect = Rect(0, 0, size.width, size.height);  
  	 if (rect.containsPoint(position)){  
  	 	log("in ret");
        		return true;  
    	}  
    	return false;  
}

void NEOMenu::onTouchMoved(Touch* touch, Event* event){  
	log("onTouchMoved");
	   auto xDelta = touch->getDelta().x;  
	   log("d=%lf", xDelta);
	   auto size = getContentSize();  
	log ("%lf,%lf",getContentSize().width,getContentSize().height);
	 _index -= xDelta / (size.width *(1.0/4));  
	for (int i = 0; i < _items.size(); i++){  
		  float x = (size.width/2)*(i - _index) / (abs(i - _index) + 1);
		_items.at(i)->setPosition(Vec2(size.width/2+x, size.height/2));  
        
	}
	return;
}

void NEOMenu::onTouchEnded(Touch* touch, Event* event){  
	log("onTouchEnded");
	   auto size = getContentSize();  
	log ("%lf,%lf",getContentSize().width,getContentSize().height);
	
	return;
}

void NEOMenu::addMenuItem(cocos2d::MenuItem *item){
	//log ("%lf,%lf",this->getContentSize().width,this->getContentSize().height);
	item->setPosition(this->getContentSize() / 2);  
    	this->addChild(item);  
    	_items.pushBack(item);
}