//
//  GameScene.cpp
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//

#include "GameScene.h"
#include "HomeScene.h"
#include "Config.h"
#include "Chara.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    log("start game!");
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //BACK GROUND LAYER
    auto bgLayer = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
    this->addChild(bgLayer);
    auto centerpos = visibleSize / 2;
    auto spritebg = Sprite::create("bg.png");
    spritebg->setPosition(centerpos);
    bgLayer->addChild(spritebg);
    
    
    //Table LAYER BOTTOM
    auto tableLayerBottom = LayerColor::create(Color4B::RED, visibleSize.width, visibleSize.height*0.6);
    tableLayerBottom->setPosition(Vec2(0,visibleSize.height*0.2));
    this->addChild(tableLayerBottom);
    auto tableBottom = Sprite::create("table_under.png");
    tableBottom->setPosition(visibleSize.width*0.5, visibleSize.height*0.2);
    tableLayerBottom->addChild(tableBottom);
    

    //Table LAYER TOP
    auto tableLayerTop = LayerColor::create(Color4B::BLUE, visibleSize.width, visibleSize.height*0.2);
    tableLayerTop->setPosition(Vec2(0,visibleSize.height*0.6));
    this->addChild(tableLayerTop);
    auto tableTop = Sprite::create("table_top.png");
    tableTop->setPosition(visibleSize.width*0.5, visibleSize.height*0.1);
    tableLayerTop->addChild(tableTop);

    
    // LAYERを上下に動かす
    
    MoveTo* goup =  MoveTo::create(3.0f, Point(0, visibleSize.height*0.4));
    MoveTo* godown = MoveTo::create(3.0f, Point(0, visibleSize.height*0.6));
    auto spawnTableTop = Spawn::create(goup, godown, NULL);
    auto repeatTableTop = RepeatForever::create(spawnTableTop);
    tableLayerTop->runAction(repeatTableTop);
    
    auto label = Label::createWithTTF("ゲーム画面", "fonts/Osaka.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(visibleSize.width/2,
                            visibleSize.height - label->getContentSize().height));
    
    // ホーム画面へ移動ボタン
    auto btnToGame = MenuItemImage::create(
                                           "back.png",
                                           "back.png",
                                           CC_CALLBACK_1(GameScene::btnToHomeCallback, this));
    
    btnToGame->setPosition(Vec2(visibleSize.width,
                                origin.y + btnToGame->getContentSize().height/2));
    auto menu = Menu::create(btnToGame, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // スタート！みたいなのをだす
    int score = getScore();

    // キャラをばらまく 30体
    for(int i = 1; i <= 30; i++) {
        srand((unsigned int)time(NULL));
        int randY = arc4random() % ((int)visibleSize.height);
        int randX = arc4random() % ((int)visibleSize.width);

        CHARA charaData = CHARA_DATA[0];
        auto chara = Chara::create(charaData);
        chara->setPosition(Vec2(randX, randY));
        charas.pushBack(chara);
        this->addChild(chara);
    }
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    this->scheduleUpdate();
    
    slot = Slot::create();
    slot->setPosition(Vec2(visibleSize.width/2,
                           visibleSize.height/4*3 - slot->getContentSize().height));
    
    ufo = Ufo::create();
    slot->setPosition(Vec2(visibleSize.width/2,
                           visibleSize.height/4 - slot->getContentSize().height));
    
    this->addChild(slot);
    this->addChild(ufo);
    
    slot->rotation();
    
    // UFOを永遠に左右に動かす
    MoveTo* gogo =  MoveTo::create(1.0f, Point(visibleSize.width, 0));
    MoveTo* goback = MoveTo::create(1.0f, Point(0, 0));
    auto spawn = Spawn::create(gogo, goback, NULL);
    auto repeatForever = RepeatForever::create(spawn);
    ufo->runAction(repeatForever);
    
    return true;
}

void GameScene::update(float dt)
{
    // iteratorで内部要素を回し、該当値であればその要素を削除
    int i = 0;
    int j = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        j = 0;
        for (auto itr2 = charas.begin(); itr2 != charas.end(); itr2++)
        {

            auto chara1 = charas.at(i);
            auto chara2 = charas.at(j);
            if (chara1 != chara2) {
                Rect rect = chara1->boundingBox();
                Rect rect2 = chara2->boundingBox();
                if(rect.intersectsRect(rect2))
                {
                    //trueの場合に、何かしらの処理を行う
                    Vec2 vec = chara1->getPosition();
                    chara1->setPosition(Vec2(vec.x, vec.y - 1));
                    Vec2 vec2 = chara1->getPosition();
                    // log("x:%f y:%f", vec2.x, vec2.y);
                }
            }
            j++;
        }
        i++;
    }

    //removeChara();
    
    ufo->update(dt);
}

// 下に落ちたキャラを消す
void GameScene::removeChara()
{
    int i = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = charas.at(i);
        Vec2 vec = chara->getPosition();
        
        if (vec.y <= 0) {
            charas.erase(i);
            this->removeChild(chara);
        }
        
        i++;
    }
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    int randY = arc4random() % ((int)visibleSize.height);
    int randX = arc4random() % ((int)visibleSize.width);
    CHARA charaData = CHARA_DATA[0];
    auto chara = Chara::create(charaData);
    chara->setPosition(Vec2(randX, randY));
    charas.pushBack(chara);
    this->addChild(chara);
    
    return true;
}

void GameScene::btnToHomeCallback(Ref* pSender)
{
    Scene* scene = Home::createScene();
    Director::getInstance()->replaceScene(scene);
}

void GameScene::setScore()
{
    UserDefault* userDefault = UserDefault::getInstance();
    const char* scoreKey = "highScore";
    userDefault->setIntegerForKey(scoreKey, 0);//0 はtempora
    userDefault->flush();
}

int GameScene::getScore()
{
    UserDefault* userDefault = UserDefault::getInstance();
    const char* scoreKey = "highScore";
    int currentScore = userDefault->getIntegerForKey(scoreKey, 0);
    return currentScore;
}

