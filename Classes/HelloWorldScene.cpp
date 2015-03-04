#include "HelloWorldScene.h"
#include <3d/CCSprite3D.h>
#include <3d/CCAnimation3D.h>
#include <3d/CCAnimate3D.h>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::onKeyPressed(EventKeyboard ::KeyCode keyCode, Event*event)
{
    //log("Key with keycode %d pressed", keyCode );
   ActionInterval *moveRight = MoveBy::create(0.1, Vec2(32, 0));
   ActionInterval *moveLeft = MoveBy::create(0.1, Vec2(-32, 0));
   ActionInterval *moveUp = MoveBy::create(0.1, Vec2(0, 32));
   ActionInterval *moveDown = MoveBy::create(0.1, Vec2(0, -32));
   switch((int)keyCode)
   {
    case 6:
    Director::getInstance()->end();
    break;
        case 32: //Enter
        this->map->setAnchorPoint(Point(0, 0.0));
        break;
        case 23: //Left
        this->hero->runAction(moveLeft);
        break;
        case 24: //Right
        this->hero->runAction(moveRight);
        break;
        case 25: //UP
        this->hero->runAction(moveUp);
        break;
        case 26: //Down
        this->hero->runAction(moveDown);
        break;
        default:
        break;
    }
}

void HelloWorld::onKeyReleased(EventKeyboard ::KeyCode keyCode, Event*event)
{
    //log("Key with keycode %d released", keyCode );
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
       "CloseNormal.png",
       "CloseSelected.png",
       CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
        origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 2);
/*
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 2);
*/

    /////////////////////////////
    //設定key event
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed , this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased , this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority (listener, this);
    /////////////////////////////
    auto tileMap = TMXTiledMap::create("neomap.tmx");
    this->map = tileMap;
    tileMap->setAnchorPoint(Point(0, 0.7));
    this->addChild(tileMap, -1);

    auto tileSize = tileMap->getTileSize();
    auto tileMapSize = Size(tileMap->getMapSize().width * tileSize.width, tileMap->getMapSize().height * tileSize.height);
    CCLOG("tileSize is %lfx%lf\n",tileSize.width,tileSize.height);
    CCLOG("tileMapSize is %lfx%lf\n",tileMapSize.width,tileMapSize.height);
    auto background = tileMap->layerNamed("Background");
    auto objects = tileMap->getObjectGroup("Objects");

    ValueMap spawnpoint = objects->getObject("SpawnPoint");
    float x = spawnpoint["x"].asFloat();
    float y = spawnpoint["y"].asFloat();
    //this->player_location.x = x;
    //this->player_location.y = y;
    
    CCLOG("[%f,%f]\n",x,y);
    this->hero = Sprite::create("Player.png");
    this->hero->setPosition(x, y);
    tileMap->addChild(this->hero);
    //得到該tile並修改
    //Sprite* sp = background->getTileAt(Vec2(8, 42));
    //sp->setScale(2.0f);
    //改變該tile的gid
    //unsigned int gid = 46;
    //background->setTileGID(gid, Vec2(8, 42));
    //得到該tile的gid
    //unsigned int gid = background->getTileGIDAt(Vec2(8, 42));
    //CCLOG("gid = %d", gid);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
 MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
 return;
#endif

 Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
 exit(0);
#endif
}
