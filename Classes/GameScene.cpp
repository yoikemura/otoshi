//
//  GameScene.cpp
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//

#include "GameScene.h"
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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto label = Label::createWithTTF("ゲーム画面", "fonts/Osaka.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(visibleSize.width/2,
                            visibleSize.height - label->getContentSize().height));
    

    // スタート！みたいなのをだす
    int score = getScore();

    // キャラをばらまく 30体
    for(int i = 1; i <= 30; i++) {
        CHARA charaData = CHARA_DATA[0];
        auto chara = Chara::create(charaData);
        chara->setPosition(Vec2(visibleSize.width/2, visibleSize.height));
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
    for (auto i = charas.begin(); i != charas.end(); i++)
    {
        
    }
    ufo->update(dt);
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

