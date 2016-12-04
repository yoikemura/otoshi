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

template
<
    typename TYPE,
    int SIZE
>
int arrayLength(const TYPE (&)[SIZE])
{
    return SIZE;
}

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

    int charaCount = arrayLength(CHARA_DATA);
    int groupCount = ceil(((double)charaCount / 4)); 
    
    std::vector<LayerColor*> layerGroups;
    
    for (int i = 0; i < groupCount; i++) {
        auto group = LayerColor::create(Color4B::RED);
        group->setContentSize(Size(320, 280));
        group->setPosition(Vec2(layerX, 0));
        carouselLayer->addChild(group);
        layerX = layerX + 320;
        layerGroups.push_back(group);
    }

    for (int i = 0; i < charaCount; i++) {

        CHARA charaData = CHARA_DATA[i];
        log("-----------//");

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

        int groupNum = i / 4;
        layerGroups[groupNum]->addChild(chara);

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
    this->dist = 0.0f;

    return true;
}


void LibraryScene::onTouchEnded(Touch* touch, Event* event)
{
    log("touch end");
    carouselCurrentPoint = this->carouselLayer->getPosition();
    Point endPoint = touch->getLocation();
    
    float flick = 35.0f;
    log("dist %f", this->dist);
    if (this->dist >= flick) {
        next();
    } else if (this->dist <= -flick){
        prev();
    } else {
        // どこにも行かない
        this->moving = true;
        MoveTo* back = MoveTo::create(0.2f, Point(this->current * -320, 106));
        auto backEase = EaseInOut::create(back->clone(), 2);
        auto cb = CallFunc::create([this](){
            this->moving = false;
            this->carouselCurrentPoint = this->carouselLayer->getPosition();
        });
        auto seq = Sequence::create(backEase, cb, NULL);
        this->carouselLayer->runAction(seq);
    }
    
    return;
}

void LibraryScene::onTouchMoved(Touch* touch, Event* event)
{
    // 移動中はなにもしない
    if (this->moving) { return;}

    Point movePoint = touch->getLocation();
    
    this->dist = touchPoint.x - movePoint.x;
    auto deltX = 0;

    if (this->dist > 0) {
        deltX = carouselCurrentPoint.x - abs(this->dist);
    } else {
        deltX = carouselCurrentPoint.x + abs(this->dist);
    }

    this->carouselLayer->setPositionX(deltX);
}

void LibraryScene::next()
{
    log("next!");
    this->current++;
    this->moving = true;

    int charaCount = arrayLength(CHARA_DATA);
    int last = ceil(((double)charaCount / 4)) - 1; 

    // 右端
    if (this->current > last) { // TODO:仮実装のためマジックナンバー
        MoveTo* next = MoveTo::create(0.5f, Point(last * -320, 106));
        auto nextEase = EaseInOut::create(next->clone(), 2);
        auto cb = CallFunc::create([this](){
            this->moving = false;
            this->carouselCurrentPoint = this->carouselLayer->getPosition();
            this->current--;
        });
        auto seq = Sequence::create(nextEase, cb, NULL);
        this->carouselLayer->runAction(seq);
        
    // スライド可能
    } else {
        MoveTo* next = MoveTo::create(0.5f, Point(this->current * -320, 106));
        auto nextEase = EaseInOut::create(next->clone(), 2);
        auto cb = CallFunc::create([this](){
            this->moving = false;
            this->carouselCurrentPoint = this->carouselLayer->getPosition();
        });
        auto seq = Sequence::create(nextEase, cb, NULL);
        this->carouselLayer->runAction(seq);
    }
}

void LibraryScene::prev()
{
    log("prev!");
    this->current--;
    this->moving = true;
    log("%i", this->current);
    
    
    // 左端
    if (this->current < 0) {
        MoveTo* prev = MoveTo::create(0.5f, Point(0, 106));
        auto prevEase = EaseInOut::create(prev->clone(), 2);
        auto cb = CallFunc::create([this](){
            this->moving = false;
            this->carouselCurrentPoint = this->carouselLayer->getPosition();
            this->current++;
        });
        auto seq = Sequence::create(prevEase, cb, NULL);
        this->carouselLayer->runAction(seq);
    // スライド可能
    } else {
        MoveTo* prev = MoveTo::create(0.5f, Point(this->current * -320, 106));
        auto prevEase = EaseInOut::create(prev->clone(), 2);
        auto cb = CallFunc::create([this](){
            this->moving = false;
            this->carouselCurrentPoint = this->carouselLayer->getPosition();
        });
        auto seq = Sequence::create(prevEase, cb, NULL);
        this->carouselLayer->runAction(seq);
    }
}

void LibraryScene::btnToHomeCallback(Ref* pSender)
{
    Scene* scene = Home::createScene();
    Director::getInstance()->replaceScene(scene);
}
