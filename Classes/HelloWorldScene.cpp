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

void HelloWorld::changeTurn(void){
   if (strcmp(this->turn, "chess1")){
          log("chess1's turn");
          strcpy(this->turn, "chess1");
        }else{
          log("chess2's turn");
          strcpy(this->turn, "chess2");
        }
}
std::string HelloWorld::getOppositeName(void){
 if (strcmp(this->turn, "chess1")){
          return "chess1";
        }else{
          return "chess2";
        }
}

void HelloWorld::onKeyPressed(EventKeyboard ::KeyCode keyCode, Event*event)
{
 log("Key with keycode %d pressed", keyCode );
 /*ActionInterval *moveRight = MoveBy::create(0.1, Vec2(60, 0));
 ActionInterval *moveLeft = MoveBy::create(0.1, Vec2(-60, 0));
 ActionInterval *moveUp = MoveBy::create(0.1, Vec2(0, 60));
 ActionInterval *moveDown = MoveBy::create(0.1, Vec2(0, -60));
 auto obj = this->map->getLayer(this->turn)->getTileAt(this->selected);
*/
 switch((int)keyCode)
 {
  case 6:

  Director::getInstance()->end();
  break;
        case 35: //Enter
        if (strcmp(this->turn, "chess1")){
          log("chess1's turn");
          strcpy(this->turn, "chess1");
        }else{
          log("chess2's turn");
          strcpy(this->turn, "chess2");
        }
        break;
        case 26: //Left
  //      obj->runAction(moveLeft);
        break;
        case 27: //Right
  //      obj->runAction(moveRight);
        break;
        case 28: //UP
  //      obj->runAction(moveUp);
        break;
        case 29: //Down
  //      obj->runAction(moveDown);
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
      strcpy(this->turn, "chess1"); 
      auto tileMap = TMXTiledMap::create("chess.tmx");
      this->map = tileMap;
      float map_offset_x = ((tileMap->getMapSize().width*tileMap->getTileSize().width)-visibleSize.width)/visibleSize.width ;
      float map_offset_y = ((tileMap->getMapSize().height*tileMap->getTileSize().height)-visibleSize.height)/visibleSize.height ;
      tileMap->setAnchorPoint(Point(0,0));
      tileMap->setPosition(100,100);
      this->addChild(tileMap, -1);

      log("%lf,%lf\n", visibleSize.width,(visibleSize.width-(tileMap->getMapSize().width*tileMap->getTileSize().width))/visibleSize.width );
    return true;
  }

  bool HelloWorld::onTouchBegan(Touch* touch, Event* event){  
    //log("TouchBegan");
    Point pt = this->convertToNodeSpace(touch->getLocation());
    this->selected = TouchpositionToTile(pt);
    if ((this->selected.x >= 0) && (this->selected.x <= 7) && (this->selected.y >= 0) && (this->selected.y <= 7)){
    	show_moveable(this->selected);
    }else{
     this->selected.x = -1;
     this->selected.y = -1;
   }
   return true;
 }

 void HelloWorld::pushCanMove(std::vector<Vec2> canMoves,int canMove_gid)
 {  
  Size mapSize = this->map->getMapSize();
  while(!canMoves.empty())
  {
   Vec2 canMove =  canMoves[canMoves.size()-1];
   if ((canMove.x < 0) || (canMove.x >= (mapSize.width) ) || (canMove.y < 0) || (canMove.y >= (mapSize.height) )){
     canMoves.pop_back();
     continue;
   }else{
     int canMove_gid = this->map->getLayer(this->turn)->getTileGIDAt(canMove);
     if (canMove_gid) {
       canMoves.pop_back();
       continue;
     }
   }
   this->needed_undo_index.push_back(canMove);
   this->needed_undo_gid.push_back(this->map->getLayer(this->turn)->getTileGIDAt(canMove));
   this->map->getLayer(this->turn)->setTileGID(canMove_gid,canMove);
   this->map->getLayer(this->turn)->getTileAt(canMove)->setOpacity(100);        
   indexCanMove.push_back(canMove);
   canMoves.pop_back();
 }
}

void HelloWorld::addXCrossCanMove(Vec2 index, std::vector<Vec2>* canMoves){
  Size mapSize = this->map->getMapSize();
  for(int moveCase=0;moveCase < 4;moveCase++){
      int i = 1;
      Vec2 canMove = Vec2(-1,-1);
      bool exit_condition = true;
      while(exit_condition){
        if (moveCase == 0){
          exit_condition = ((index.x+i) < (mapSize.width) && (index.y+i) < (mapSize.height));
          canMove.x = index.x+i;
          canMove.y = index.y+i;
        }else if (moveCase == 1){
          exit_condition = ((index.x - i) >= 0 && ((index.y+i) < (mapSize.height)) );
          canMove.x = index.x - i;
          canMove.y = index.y + i;
        }else if (moveCase == 2){
          exit_condition = ((index.x+i) < (mapSize.width) && (index.y-i) >= 0);
          canMove.x = index.x + i;
          canMove.y = index.y - i;
        }else if (moveCase == 3){
          exit_condition = ((index.x - i) >= 0 && ((index.y-i) >= 0));
          canMove.x = index.x - i;
          canMove.y = index.y - i;
        }
        if (!exit_condition) {
          i++;
          continue;
        }
        int canMove_gid = this->map->getLayer(this->turn)->getTileGIDAt(canMove);
        if (!canMove_gid) {
          canMove_gid = this->map->getLayer(this->getOppositeName())->getTileGIDAt(canMove);
          canMoves->push_back(canMove);
          i++;
          if (!canMove_gid) {
            continue;
          }else{
            break;
          } 
        }
        i++;
        break;
      }
    }
}

void HelloWorld::addPawnCanMove(Vec2 index, std::vector<Vec2>* canMoves){
  Size mapSize = this->map->getMapSize();
     float canMoveOne;
     float canMoveTwo;
     Vec2 canMoveEatLeft;
     Vec2 canMoveEatRight;
     bool moveTwoCondition = false;
     if (strcmp(this->turn, "chess1")){
          //chess2
      canMoveOne = index.y+1;
      canMoveTwo = index.y+2;
      canMoveEatLeft.x = index.x+1;
      canMoveEatLeft.y = index.y+1;
       canMoveEatRight.x = index.x-1;
      canMoveEatRight.y = index.y+1;
      moveTwoCondition = (index.y == 1);

    }else{
          //chess1
      canMoveOne = index.y-1;
      canMoveTwo = index.y-2;
       canMoveEatLeft.x = index.x-1;
      canMoveEatLeft.y = index.y-1;
       canMoveEatRight.x = index.x+1;
      canMoveEatRight.y = index.y-1;
      moveTwoCondition = (index.y == 6);
    }
      if ((canMoveOne >= 0) && (canMoveOne < mapSize.height)){
        int checkgid = this->map->getLayer(this->turn)->getTileGIDAt(Vec2(index.x,canMoveOne));
        int checkoppositegid = this->map->getLayer(this->getOppositeName())->getTileGIDAt(Vec2(index.x,canMoveOne));
        if (!checkgid && !checkoppositegid){
          canMoves->push_back(Vec2(index.x,canMoveOne)); 
          if (moveTwoCondition){
            checkgid = this->map->getLayer(this->turn)->getTileGIDAt(Vec2(index.x,canMoveTwo));
            checkoppositegid = this->map->getLayer(this->getOppositeName())->getTileGIDAt(Vec2(index.x,canMoveTwo));
             if (!checkgid && !checkoppositegid){
              canMoves->push_back(Vec2(index.x,canMoveTwo)); 
             }
          }
        }
    }
    if ((canMoveEatRight.x >= 0) && (canMoveEatRight.y >= 0 )&&(canMoveEatRight.x < mapSize.width) && (canMoveEatRight.y < mapSize.height)){
         int checkoppositegid = this->map->getLayer(this->getOppositeName())->getTileGIDAt(canMoveEatRight);
     if (checkoppositegid){
      canMoves->push_back(canMoveEatRight);
     }
    }
    if ((canMoveEatLeft.x >= 0) && (canMoveEatLeft.y >= 0 )&&(canMoveEatLeft.x < mapSize.width) && (canMoveEatLeft.y < mapSize.height)){
           int checkoppositegid = this->map->getLayer(this->getOppositeName())->getTileGIDAt(canMoveEatLeft);
     if (checkoppositegid){
      canMoves->push_back(canMoveEatLeft);
     }
    }
}

void HelloWorld::addTenCrossCanMove(Vec2 index, std::vector<Vec2>* canMoves){
  Size mapSize = this->map->getMapSize();
  for(int moveCase=0;moveCase < 4;moveCase++){
      int i = 1;
      Vec2 canMove = Vec2(-1,-1);
      bool exit_condition = true;
      while(exit_condition){
        if (moveCase == 0){
          exit_condition = ((index.x+i) < (mapSize.width));
          canMove.x = index.x+i;
          canMove.y = index.y;
        }else if (moveCase == 1){
          exit_condition = ((index.y - i) >= 0);
          canMove.x = index.x;
          canMove.y = index.y - i;
        }else if (moveCase == 2){
          exit_condition = ((index.x - i) >= 0);
          canMove.x = index.x - i;
          canMove.y = index.y;
        }else if (moveCase == 3){
          exit_condition = ((index.y + i) < (mapSize.height));
          canMove.x = index.x;
          canMove.y = index.y + i;
        }
        if (!exit_condition) {
          i++;
          continue;
        }
        int canMove_gid = this->map->getLayer(this->turn)->getTileGIDAt(canMove);
        if (!canMove_gid) {
          //log("push %lf,%lf",canMove.x,canMove.y);
          canMove_gid = this->map->getLayer(this->getOppositeName())->getTileGIDAt(canMove);
          canMoves->push_back(canMove);
          i++;
          if (!canMove_gid) {
            continue;
          }else{
            break;
          } 
        }
        i++;
        break;
      }
    }
}

void HelloWorld::show_moveable(Vec2 index){
  Size mapSize = this->map->getMapSize();
  int gid = this->map->getLayer(this->turn)->getTileGIDAt(index); 
   //log("gid=%d",gid);
  if (gid) {
   this->selected_arms_gid = gid;
   auto properties = this->map->getPropertiesForGID(gid).asValueMap();
   if (!properties.empty()) {
    auto arms = properties["arms"].asString();
    std::vector<Vec2> canMoves;    
    if ("pawn" == arms) {
  addPawnCanMove(index, &canMoves);
    //log("is pawn\n");
  }else if("king" == arms){
    canMoves.push_back(Vec2(index.x,index.y+1));
    canMoves.push_back(Vec2(index.x+1,index.y+1));
    canMoves.push_back(Vec2(index.x-1,index.y+1));
    canMoves.push_back(Vec2(index.x+1,index.y));
    canMoves.push_back(Vec2(index.x-1,index.y));
    canMoves.push_back(Vec2(index.x-1,index.y-1));
    canMoves.push_back(Vec2(index.x,index.y-1));
    canMoves.push_back(Vec2(index.x+1,index.y-1));
    //log("is king\n");
  }else if("queen" == arms){
   addXCrossCanMove(index, &canMoves);
   addTenCrossCanMove(index, &canMoves);
   //    log("is queen\n");
  }else if("bishop" == arms){
    addXCrossCanMove(index, &canMoves);
    //log("is bishop\n");
  }else if("rook" == arms){
    addTenCrossCanMove(index, &canMoves);
    //log("is rook\n");
  }else if("knight" == arms){
    canMoves.push_back(Vec2(index.x-1,index.y+2));
    canMoves.push_back(Vec2(index.x+1,index.y+2));
    canMoves.push_back(Vec2(index.x-2,index.y+1));
    canMoves.push_back(Vec2(index.x+2,index.y+1));
    canMoves.push_back(Vec2(index.x-2,index.y-1));
    canMoves.push_back(Vec2(index.x+2,index.y-1));
    canMoves.push_back(Vec2(index.x-1,index.y-2));
    canMoves.push_back(Vec2(index.x+1,index.y-2));
    //log("is knight\n");
  }
  pushCanMove(canMoves,1);    
}else{
  log("no properties\n");   
  this->selected.x = -1;
  this->selected.y = -1;
  this->selected_arms_gid = -1;
}
}else{
  log("gid=0\n"); 
  this->selected.x = -1;
  this->selected.y = -1;
  this->selected_arms_gid = -1;
}
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event){  
  Point pt = this->convertToNodeSpace(touch->getLocation());
  recoveryBoardChange();
  moveChess(pt);
  this->selected.x = -1;
  this->selected.y = -1;
  this->selected_arms_gid = -1;
  indexCanMove.clear();
  return;
}

void HelloWorld::gotEat(std::string arms){
  log("[%s] got eat!", arms.c_str()); 
}

void HelloWorld::moveChess(Point pt){  
   Size mapSize = this->map->getMapSize();
 if ((this->selected.x >= 0) && (this->selected.x < mapSize.width) && (this->selected.y >= 0) && (this->selected.y < mapSize.height)){
  Vec2 targetIndex = TouchpositionToTile(pt);
  auto obj = this->map->getLayer(this->turn)->getTileAt(this->selected);
  while(!indexCanMove.empty()){
    Vec2 canMove = indexCanMove[indexCanMove.size()-1];
    if (targetIndex.x==canMove.x && targetIndex.y == canMove.y){
      int checkOppsiteGid = this->map->getLayer(this->getOppositeName())->getTileGIDAt(targetIndex);
      if (checkOppsiteGid){
        //Got Eat
        this->map->getLayer(this->getOppositeName())->removeTileAt(targetIndex);
        auto gotEatProperties = this->map->getPropertiesForGID(checkOppsiteGid).asValueMap();
        if (!gotEatProperties.empty()) {
          auto gotEatArms = gotEatProperties["arms"].asString();
          gotEat(gotEatArms);
        }
      }    
      this->map->getLayer(this->turn)->setTileGID(this->selected_arms_gid,targetIndex);
      this->map->getLayer(this->turn)->removeTileAt(this->selected);
     break;
    }
    indexCanMove.pop_back();
  }
  if (indexCanMove.empty()){
   ActionInterval *moveoffset = MoveTo::create(0.0, TileIndexToMapPosition(this->selected));
   obj->runAction(moveoffset);
  }
 }
}

void HelloWorld::recoveryBoardChange(void){  
 while(!needed_undo_gid.empty()){
  Vec2 index =this->needed_undo_index[this->needed_undo_index.size()-1];
  int gid = this->needed_undo_gid[this->needed_undo_gid.size()-1];
  this->map->getLayer(this->turn)->setTileGID(gid,index);
  this->needed_undo_index.pop_back();
  this->needed_undo_gid.pop_back();
}
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event){ 
  //log("TouchMoved");
  if (this->selected.y == -1 || this->selected.x == -1){
   return;
 }else{
  auto obj = this->map->getLayer(this->turn)->getTileAt(this->selected);
 //obj->setZOrder(999);
 // obj->getParent()->reorderChild(obj,666);

  auto xDelta = touch->getDelta().x;
  auto yDelta = touch->getDelta().y;
  ActionInterval *moveoffset = MoveBy::create(0.0, Vec2(xDelta, yDelta ));
  obj->runAction(moveoffset);
}
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

Point HelloWorld::TileIndexToMapPosition(Vec2 index)
{
  Size mapSize = this->map->getMapSize();
  Size tileSize = this->map->getTileSize();
  int x = index.x * tileSize.width;   
  int y = (mapSize.height * (tileSize.height)) - (index.y * tileSize.height) - tileSize.height;  
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
