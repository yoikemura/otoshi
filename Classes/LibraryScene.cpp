//
//  LibraryScene.cpp
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//

#include "LibraryScene.h"
#include "LibraryManager.h"
#include "SimpleAudioEngine.h"
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
    
    // 詳細表示用キャラ配列
    Vector<Sprite*> v1 = Vector<Sprite*>(); 
    
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
    
    //　背景
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    // タイトル画像
    auto titleImg = Sprite::create("karte_title.png");
    // position the label on the center of the screen
    titleImg->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - titleImg->getContentSize().height));
    this->addChild(titleImg);
    

    auto libraryManager = LibraryManager::getInstance();

    int layerX = 0;
    int charaX = 0;
    int charaY = 0;

    // TODO: 雑過ぎリファクタ
    // carousel layer
    this->carouselLayer = LayerColor::create();
    carouselLayer->setPosition(Vec2(0, 106));
    carouselLayer->setContentSize(Size(320 * 3, 280));
    carouselLayer->setAnchorPoint(Vec2(0, 0));
    this->addChild(carouselLayer);

    int charaCount = arrayLength(CHARA_DATA);
    int groupCount = ceil(((double)charaCount / 4)); 
    
    std::vector<LayerColor*> layerGroups;
    
    for (int i = 0; i < groupCount; i++) {
        auto group = LayerColor::create();
        group->setContentSize(Size(320, 280));
        group->setPosition(Vec2(layerX, 0));
        carouselLayer->addChild(group);
        layerX = layerX + 320;
        layerGroups.push_back(group);
    }

    for (int i = 0; i < charaCount; i++) {
        CHARA charaData = CHARA_DATA[i];
        
        int leftEdge = 19;

        // キャラ配置
        if (i % 4 == 0) {
            charaX = leftEdge + 68;
            charaY = 138 + 8 + 68;
        } else if (i % 4 == 1) {
            charaX = leftEdge + 136 + 9 + 68;
            charaY = 138 + 8 + 68;
        } else if (i % 4 == 2) {
            charaX = leftEdge + 68;
            charaY = 68;
        } else if (i % 4 == 3) {
            charaX = leftEdge + 136 + 9 + 68;
            charaY = 68;
        }
        
        // 配置対象レイヤー　
        int groupNum = i / 4;

        // キャラクターを置く背景
        auto karuteBg = Sprite::create("karte_bg.png");
        karuteBg->setPosition(Vec2(charaX, charaY));
        layerGroups[groupNum]->addChild(karuteBg);
        karuteBg->setTag(i);
        
        // タッチしたら詳細を表示する処理
        auto listener = EventListenerTouchOneByOne::create();
        
        // onTouchBeganイベントコールバック関数実装のラムダ式の例
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            log("タッチ開始");
            this->detailTouchPoint = touch->getLocation();
            
            auto target = static_cast<Sprite*>(event->getCurrentTarget());
            
            // ボタンに対する相対的な位置を取得する。
            Point locationInNode = target->convertToNodeSpace(touch->getLocation());
            // コンテンツのサイズを取得する。
            Size s = target->getContentSize();
            // コンテンツ矩形を作成する。
            Rect rect = Rect(0, 0, s.width, s.height);
            
            // クリックエリアをチェックする。
            if (rect.containsPoint(locationInNode))
            {
                log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
                return true;
            }
            return false;
        };
        
        // タッチが移動した場合
        listener->onTouchMoved = [=](Touch* touch, Event* event) {
            log("タッチ move");
            this->detailMoving = true;
        };
        
        // タッチイベントが終了した場合
        listener->onTouchEnded = [=](Touch* touch, Event* event) {
            Point p = touch->getLocation();
            
            auto deltX = this->detailTouchPoint.x - abs(p.x);
            log(this->detailTouchPoint.x);
            log(p.x);
            
            if (deltX >= 20 && this->detailMoving) {
                return;
            }
            
            this->detailMoving = false;
            
            log("タッチ 終わり");
            log(deltX);
            auto target = static_cast<Sprite*>(event->getCurrentTarget());
            int charaIdx = target->getTag();
            auto targetChara = charas.at(charaIdx);
            this->showDetail(targetChara);
        };
        
        // イベントリスナーをスプライトに追加する。
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, karuteBg);
        
        // キャラクター
        auto chara = Chara::create(charaData);
        chara->setPosition(Vec2(charaX, charaY));
        layerGroups[groupNum]->addChild(chara);
        charas.pushBack(chara);
        
        // キャラ名前
        auto charaName = Label::createWithTTF(charaData.name, "fonts/Osaka.ttf", 15);
        charaName->setColor(ccc3(0,0,0));
        charaName->setPosition(Vec2(charaX, charaY+50));
        layerGroups[groupNum]->addChild(charaName);
    }

    // ホーム画面へ移動ボタン
    auto startHome = MenuItemImage::create(
                                           "btn_back.png", "btn_back_on.png",
                                           CC_CALLBACK_1(LibraryScene::backToHome, this));
    
    Menu* pMenu = Menu::create(startHome, NULL);
    pMenu->setPosition(visibleSize.width*0.1, visibleSize.height*0.95);
    pMenu->alignItemsHorizontally();
    this->addChild(pMenu);
    
    return true;
}

void LibraryScene::showDetail(Chara* chara)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 詳細用モーダルオーバーレイ
    auto dialogLayer = LayerColor::create(Color4B::BLACK);
    dialogLayer->setOpacity(128);
    dialogLayer->setContentSize(visibleSize);
    
    // 詳細用モーダルオーバーレイ
    auto dialogBox = Sprite::create("karte_detail_bg.png");
    dialogBox->setPosition(Point(visibleSize.width * 0.5, visibleSize.height * 0.5));
    dialogLayer->addChild(dialogBox);
    
    // 画像
    auto charaImg = Sprite::create(chara->getFileName());
    charaImg->setPosition(Vec2(70, 340));
    dialogLayer->addChild(charaImg);
    
    // 説明文
    auto descLabel = Label::createWithSystemFont(chara->getDescription(), "fonts/Osaka.ttf", 15);
    descLabel->setWidth(240);
    descLabel->setColor(ccc3(0,0,0));
    descLabel->setPosition(Point(visibleSize.width * 0.5, 150));
    dialogLayer->addChild(descLabel);
   
    // タッチしたら詳細を非表示にする処理
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        target->removeFromParentAndCleanup(true);
        return false;
    };
    
    // イベントリスナーをスプライトに追加する。
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, dialogLayer);
    
    this->addChild(dialogLayer);
}

bool LibraryScene::onTouchBegan(Touch* touch, Event* event)
{
    touchPoint = touch->getLocation();
    this->dist = 0.0f;

    return true;
}


void LibraryScene::onTouchEnded(Touch* touch, Event* event)
{
    carouselCurrentPoint = this->carouselLayer->getPosition();
    Point endPoint = touch->getLocation();
    
    float flick = 35.0f;
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
    this->current--;
    this->moving = true;
    
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

void LibraryScene::backToHome(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_clicked.mp3");
    Scene* scene = Home::createScene();
    Director::getInstance()->replaceScene(scene);
}
