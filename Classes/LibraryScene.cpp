//
//  LibraryScene.cpp
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//

#include "LibraryScene.h"
#include "LibraryManager.h"
#include "HomeScene.h"
#include "config.h"
#include "chara.h"

USING_NS_CC;


Scene* LibraryScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LibraryScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // Touchイベント用
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(LibraryScene::onTouchBegan, layer);
    listener->onTouchEnded = CC_CALLBACK_2(LibraryScene::onTouchEnded, layer);
    listener->onTouchMoved = CC_CALLBACK_2(LibraryScene::onTouchMoved, layer);
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LibraryScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setTouchEnabled(true);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto label = Label::createWithTTF("図鑑画面", "fonts/Osaka.ttf", 24);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    

    auto libraryManager = LibraryManager::getInstance();

    int layerX = 0;
    int charaX = 0;
    int charaY = 0;

    // TODO: 雑過ぎリファクタ
    // carousel layer
    this->carouselLayer = LayerColor::create(Color4B::BLACK);
    carouselLayer->setPosition(Vec2(0, 106));
    carouselLayer->setContentSize(Size(320 * 3, 280));
    carouselLayer->setAnchorPoint(Vec2(0, 0));
    this->addChild(carouselLayer);

    auto group1 = LayerColor::create(Color4B::RED);
    group1->setContentSize(Size(320, 280));
    group1->setPosition(Vec2(layerX, 0));
    carouselLayer->addChild(group1);
    layerX = layerX + 320;

    auto group2 = LayerColor::create(Color4B::BLUE);
    group2->setContentSize(Size(320, 280));
    group2->setPosition(Vec2(layerX, 0));
    carouselLayer->addChild(group2);
    layerX = layerX + 320;

    auto group3 = LayerColor::create(Color4B::GREEN);
    group3->setContentSize(Size(320, 280));
    group3->setPosition(Vec2(layerX, 0));
    carouselLayer->addChild(group3);
    layerX = layerX + 320;

    for (int i = 0; i < 11; i++) {

        CHARA charaData = CHARA_DATA[i];

        // キャラ配置
        if (i % 4 == 0) {
            charaX = 43 + 68;
            charaY = 138 + 8 + 68;
        } else if (i % 4 == 1) {
            charaX = 43 + 136 + 9 + 68;
            charaY = 138 + 8 + 68;
        } else if (i % 4 == 2) {
            charaX = 43 + 68;
            charaY = 68;
        } else if (i % 4 == 3) {
            charaX = 43 + 136 + 9 + 68;
            charaY = 68;
        }

        auto chara = Chara::create(charaData);
        chara->setPosition(Vec2(charaX, charaY));

        if (i < 4) {
          group1->addChild(chara);
        }

        if (4 <= i && i < 8) {
          group2->addChild(chara);
        }

        if (8 <= i && i < 12) {
          group3->addChild(chara);
        }

        // if (libraryManager->hasGotten(charaData.id.c_str())) {

    }
    
    // add the label as a child to this layer
    this->addChild(label, 1);

    // ホーム画面へ移動ボタン
    auto btnToHome = MenuItemImage::create(
                                           "back.png",
                                           "back.png",
                                           CC_CALLBACK_1(LibraryScene::btnToHomeCallback, this));
    
    btnToHome->setPosition(Vec2(10,
                                origin.y + btnToHome->getContentSize().height/2));
    auto menu = Menu::create(btnToHome, NULL);
    this->addChild(menu, 1);

    
    return true;
}


bool LibraryScene::onTouchBegan(Touch* touch, Event* event)
{
    log("touch start");
    touchPoint = touch->getLocation();
    return true;
}


void LibraryScene::onTouchEnded(Touch* touch, Event* event)
{
    log("touch end");
    carouselCurrentPoint = this->carouselLayer->getPosition();
    Point endPoint = touch->getLocation();
}

void LibraryScene::onTouchMoved(Touch* touch, Event* event)
{
    Point movePoint = touch->getLocation();
    
    auto dist = touchPoint.x - movePoint.x;
    auto deltX = 0;
    log("moved %f", movePoint.x);
    log("touchX %f", touchPoint.x);
    log("carouselX %f", this->carouselLayer->getPositionX());
    log("dist %f", dist);

    if (dist > 0) {
        log("レイヤーは左へ移動");
        deltX = carouselCurrentPoint.x - abs(dist);
    } else {
        log("レイヤーは右へ移動");
        deltX = carouselCurrentPoint.x + abs(dist);
    }

    this->carouselLayer->setPositionX(deltX);
    log("carouselX移動後 %f", this->carouselLayer->getPositionX());
}

void LibraryScene::btnToHomeCallback(Ref* pSender)
{
    Scene* scene = Home::createScene();
    Director::getInstance()->replaceScene(scene);
}
