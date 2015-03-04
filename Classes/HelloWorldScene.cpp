#include "HelloWorldScene.h"
#include "NEOMenu.h"

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
 log("Key with keycode %d pressed", keyCode );
 ActionInterval *moveRight = MoveBy::create(0.1, Vec2(60, 0));
 ActionInterval *moveLeft = MoveBy::create(0.1, Vec2(-60, 0));
 ActionInterval *moveUp = MoveBy::create(0.1, Vec2(0, 60));
 ActionInterval *moveDown = MoveBy::create(0.1, Vec2(0, -60));
 auto obj = this->map->getLayer("chess")->getTileAt(this->selected);
 switch((int)keyCode)
 {
    case 6:
    Director::getInstance()->end();
    break;
        case 35: //Enter
        break;
        case 26: //Left
        obj->runAction(moveLeft);
        break;
        case 27: //Right
        obj->runAction(moveRight);
        break;
        case 28: //UP
        obj->runAction(moveUp);
        break;
        case 29: //Down
        obj->runAction(moveDown);
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
    this->addChild(label, 1);

 //////////////////////////////////////
    //設定key event
    auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed , this);
   // keylistener->onKey = CC_CALLBACK_2(HelloWorld::onKeyPressed , this);
    keylistener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased , this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority (keylistener, this);
////////////////////////////////////
    auto touchlistener = EventListenerTouchOneByOne::create();  
    touchlistener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);  
    touchlistener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);  
    touchlistener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);  
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlistener, this);  

    ////////////////////////////////
    auto tileMap = TMXTiledMap::create("chess.tmx");
    this->map = tileMap;
    float map_offset_x = ((tileMap->getMapSize().width*tileMap->getTileSize().width)-visibleSize.width)/visibleSize.width ;
    float map_offset_y = ((tileMap->getMapSize().height*tileMap->getTileSize().height)-visibleSize.height)/visibleSize.height ;
    tileMap->setAnchorPoint(Point(0,0));
    tileMap->setPosition(100,100);

//    tileMap->setAnchorPoint(Point(map_offset_x, map_offset_y+0.15));
    this->addChild(tileMap, -1);

    log("%lf,%lf\n", visibleSize.width,(visibleSize.width-(tileMap->getMapSize().width*tileMap->getTileSize().width))/visibleSize.width );
/*
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
/////////////////////////////////////
  */
    return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event){  
    Point pt = this->convertToNodeSpace(touch->getLocation());
    this->selected = TouchpositionToTile(pt);
    show_moveable(this->selected);
    //TileIndexToPosition(Vec2(0,0));
    return true;
}

void HelloWorld::show_moveable(Vec2 index){

   int gid = this->map->getLayer("chess")->getTileGIDAt(index); 
   if (gid) {
       auto properties = this->map->getPropertiesForGID(gid).asValueMap();
       if (!properties.empty()) {
        auto arms = properties["arms"].asString();
        if ("pawn" == arms) {
            this->needed_undo_index.push_back(Vec2(index.x,index.y-1));
            this->needed_undo_gid.push_back(this->map->getLayer("chess")->getTileGIDAt(Vec2(index.x,index.y-1)));
            this->map->getLayer("chess")->setTileGID(1,Vec2(index.x,index.y-1));
            log("is pawn\n");
            return;
        }else if("king" == arms){
          log("is king\n");
          return;
      }else if("queen" == arms){
         log("is queen\n");
         return;

     }else if("bishop" == arms){
         log("is bishop\n");
         return;

     }else if("rook" == arms){
         log("is rook\n");
         return;

     }else if("knight" == arms){
         log("is knight\n");
         return;
     }
 }else{
   log("no properties\n");   
}
}
     //log();
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event){  
    if (!needed_undo_gid.empty()){
        Vec2 index =this->needed_undo_index[this->needed_undo_index.size()-1];
        int gid = this->needed_undo_gid[this->needed_undo_gid.size()-1];
        this->map->getLayer("chess")->setTileGID(gid,index);
        this->needed_undo_index.pop_back();
        this->needed_undo_gid.pop_back();
    }
    return;
}
void HelloWorld::onTouchMoved(Touch* touch, Event* event){ 
  auto xDelta = touch->getDelta().x;
  auto yDelta = touch->getDelta().y; 
  return;
}

Vec2 HelloWorld::TouchpositionToTile(Point pt){
   Point zero_board = this->map->getLayer("board")->getPositionAt(Vec2(0,0)); 
   zero_board.x = zero_board.x + this->map->getPosition().x;
   zero_board.y = zero_board.y + this->map->getPosition().y;
   Size mapSize = this->map->getMapSize();
   Size tileSize = this->map->getTileSize();  
   Size visibleSize = Director::getInstance()->getVisibleSize(); 
   int x = mapSize.width - (mapSize.width * tileSize.width - (pt.x-zero_board.x)) / tileSize.width ;  
   int y = mapSize.height - (mapSize.height * tileSize.height - ((zero_board.y+tileSize.height - pt.y) ) ) / tileSize.height ; 
 //log("%d,%d \n",x,y); 
   return Vec2(x,y);    
}

Point HelloWorld::TileIndexToPosition(Vec2 index)
{  
    Size mapSize = this->map->getMapSize();   
    Size tileSize = this->map->getTileSize();  
    int x = index.x * tileSize.width + this->map->getPosition().x;//+ tileSize.width / 2;  
    int y = (mapSize.height * (tileSize.height - 1)) - (index.y * tileSize.height) + this->map->getPosition().y; //+ tileSize.height / 2);  
    //log("%d,%d \n",x,y);
return Point(x,y);  
}  

void HelloWorld::menuItem1Callback(cocos2d::Ref* pSender){  
    log("get menuItem1Callback");
    bg_sprite[0]->setVisible(false);  
    bg_sprite[1]->setVisible(false);  
    bg_sprite[2]->setVisible(false);  
    bg_sprite[3]->setVisible(false);  
    bg_sprite[4]->setVisible(false);  

    bg_sprite[0]->setVisible(true);  
}  
void HelloWorld::menuItem2Callback(cocos2d::Ref* pSender){  
    log("get menuItem2Callback");
    bg_sprite[0]->setVisible(false);  
    bg_sprite[1]->setVisible(false);  
    bg_sprite[2]->setVisible(false);  
    bg_sprite[3]->setVisible(false);  
    bg_sprite[4]->setVisible(false);  

    bg_sprite[1]->setVisible(true);  

}  
void HelloWorld::menuItem3Callback(cocos2d::Ref* pSender){  
    log("get menuItem3Callback");
    bg_sprite[0]->setVisible(false);  
    bg_sprite[1]->setVisible(false);  
    bg_sprite[2]->setVisible(false);  
    bg_sprite[3]->setVisible(false);  
    bg_sprite[4]->setVisible(false);  

    bg_sprite[2]->setVisible(true);  

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
