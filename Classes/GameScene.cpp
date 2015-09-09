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
#include "SimpleAudioEngine.h"
#include <time.h>
#include <stdio.h>

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
    
    //BACK GROUND LAYER
    auto bgLayer = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
    this->addChild(bgLayer);
    auto centerpos = visibleSize / 2;
    auto spritebg = Sprite::create("bg.png");
    spritebg->setPosition(centerpos);
    bgLayer->addChild(spritebg);
    
    //Table BOTTOM
    tableBottom = Sprite::create("table_under.png");
    tableBottom->setPosition(visibleSize.width*0.5, 210);
    this->addChild(tableBottom);
    
    //Table TOP
    tableTop = Sprite::create("table_top.png");
    tableTop->setPosition(visibleSize.width*0.5, TABLE_TOP_Y);
    this->addChild(tableTop);

    // NOTE:キャラ配置マップを作るためのログ
    int tb = tableBottom->getBoundingBox().getMinY();
    int tt = tableTop->getBoundingBox().getMaxY();
    log("table top maxY: %i, table bottom minY: %i", tt, tb);
    
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
    char text[20];
    sprintf(text, "フィーバーまであと%i体", this->score);
    this->scoreLabel = Label::createWithSystemFont(text, "arial", 12);
    this->scoreLabel->setPosition(Vec2(230, visibleSize.height*0.9));
    this->scoreLabel->setColor(Color3B::WHITE);
    this->addChild(scoreLabel);
    
    // キャラをばらまく 30体
    Rect rect = tableBottom->getBoundingBox();
    Rect rect2 = tableTop->getBoundingBox();

    for(int i = 0; i < 30; i++) {
        srand((unsigned int)time(NULL));
        int randX = arc4random() % ((int)visibleSize.width);

        CHARA charaData = CHARA_DATA[0];
        auto chara = Chara::create(charaData);
        chara->setPosition(Vec2(randX, DEFAULT_CHARA_MAP[i]));
        
        if (this->isInUpperTable(chara)) {
            chara->isUpperTable = true;
        } else {
            chara->isLowerTable = true;
        }

        charas.pushBack(chara);
        this->addChild(chara);
    }

    // キャラをばらまいた時点で落ちる奴は落ちるｗ
    this->dropCharas();
    
    this->slot = Slot::create();
    this->slot->setPosition(Vec2(visibleSize.width*0.5, 384));
    
    this->ufo = Ufo::create();
    ufo->setPosition(Vec2(0, ufo->getBoundingBox().size.height));
    
    this->addChild(slot);
    this->addChild(ufo);

    // UFOを永遠に左右に動かす
    this->ufo->move();

    // メインループ開始
    this->scheduleUpdate();
    
    return true;
}

void GameScene::update(float dt)
{
    // イベントキューに値があればイベントスタート
    // 何かしらのイベント終了時にはisInEventをfalseにして終了すること
    // TODO: Must refactor!
    if (!this->isInEvent &&
        this->eventQueue.size() > 0) {
        log("イベント開始 %i", this->eventId);
        auto itr = this->eventQueue.begin();
        this->eventId = *this->eventQueue.erase(itr);
        
        // NOTE: 実装済み
        // - 台が伸びるのみ実装
        // - キャラ増殖
        if (this->eventId == EVENT_LOGN || this->eventId == EVENT_INCREMENT) {
            this->isInEvent = true;
        }
    }

    Vec2 tableVec = tableTop->getPosition();
    int tableY = tableVec.y;
    
    if (isTableFoward) {
        tableTop->setPositionY(tableY - 1);
        this->moveCharas(-1);
    }

    if (isTableBack) {
        tableTop->setPositionY(tableY + 1);
        this->moveCharas(1);
    }

    if (tableY == TABLE_TOP_Y + 10) {
        isTableBack = false;
        isTableFoward = true; 
    } 

    // テーブルが伸びるイベント
    if (this->isInEvent && this->eventId == EVENT_LOGN) {
        if(tableY == TABLE_TOP_Y - 100) {
            isTableBack = true;
            isTableFoward = false; 
            this->isInEvent = false;
        }
    } else {
        if(tableY == TABLE_TOP_Y - 40) {
            isTableBack = true;
            isTableFoward = false; 
        }
    }

    // 増殖イベント
    if (this->isInEvent && this->eventId == EVENT_INCREMENT) {
        this->incrementChara();
        this->isInEvent = false;
    }

    // 上のテーブルから落ちる
    this->dropFromUpperTable();
    
    // 上のテーブルに押し出される
    this->sweep(1);

    // 衝突判定
    this->detectCollision();

    // キャラを落とす
    this->dropCharas();

    // キャラを消す
    this->removeCharas();
    
    // UFOとの衝突判定
    this->detectUfoCollision();
}

void GameScene::detectCollision() 
{
    // iteratorで内部要素を回し、該当値であればその要素を削除
    int i = 0;
    int j = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {

        j = 0;
        for (auto itr2 = charas.begin(); itr2 != charas.end(); itr2++) {
            auto chara1 = (Chara*)(*itr);
            auto chara2 = (Chara*)(*itr2);

            // TODO: 同じテーブル内でしか衝突しないようにしたい

            // 配列の先頭程奥にある
            // 奥にあるので自分より手前(配列のindexが大きい物)に対してのみ衝突判定をするべき
            // 奥の物が前のものを押し出すイメージ
            if (j > i) {
                Rect rect1 = chara1->boundingBox();
                Rect rect2 = chara2->boundingBox();

                // 斜辺
                float delt = rect2.size.width * rect2.size.width  - 500.0f;
                float ab_x = rect1.getMidX() - rect2.getMidX();
                float ab_y = rect1.getMidY() - rect2.getMidY();

                if (ab_x * ab_x +  ab_y * ab_y < delt)  {
                    Vec2 vec = chara2->getPosition();
                    // めり込んだ量
                    // float len = sqrtf(ab_x * ab_x + ab_y * ab_y);
                    // float dist = rect2.size.width + rect2.size.width - len;
                    float dist = 2.0f;
                    float angle = atan2f(ab_y, ab_x);
                    float nx = cosf(angle) * dist;
                    float ny = sinf(angle) * dist;
                    
                    // 少なくとも前に進ませる！
                    if (ny <= 0) {
                        ny = 1.0f;
                    }

                    // 落下中は衝突判定しない;w
                    if (!chara2->isDropping) {
                      // 自分より手前(chara2)を移動させる
                      // 移動量はchara1とのめり込み分
                      chara2->setPosition(Vec2(vec.x - nx, vec.y - ny));
                    }
                }
            }
            j++;
        }
        i++;
    }
}

// キャラが上のテーブルから落ちたか判定
void GameScene::dropFromUpperTable()
{
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = (Chara*)(*itr);
        if (chara->isUpperTable && !chara->isDroppingFromUpperTable)
        {
            if (!this->isInUpperTable(chara)) {
              chara->dropFromUpperTable();
              chara->isUpperTable = false;
              chara->isLowerTable = true;
            }
        }
    }
}

bool GameScene::isInUpperTable(Chara* chara)
{
    Rect tableRect = tableTop->getBoundingBox();
    int tableMaxY = tableRect.getMaxY();
    int tableMinY = tableRect.getMinY();
    int tableMaxX = tableRect.getMaxX();
    int tableMinX = tableRect.getMinX();

    Rect charaRect = chara->getBoundingBox();
    int charaMaxY = charaRect.getMaxY();
    int charaMinY = charaRect.getMinY();
    int charaMaxX = charaRect.getMaxX();
    int charaMinX = charaRect.getMinX();
    
    if (charaMaxY <= tableMaxY &&
        charaMaxX <= tableMaxX &&
        charaMinY >= tableMinY &&
        charaMinX >= tableMinX) {
        return true;
    } else {
        return false;
    }
}

// 下に落ちたキャラを消す
void GameScene::removeCharas()
{
    auto itr = charas.begin();
    while (itr != charas.end()) {
        auto chara = (*itr);
        int y = chara->getPositionY();

        if (y <= -30) {
            itr = charas.erase(itr);
            this->removeChild(chara);
        }
        else
        {
            itr++;
        }
    }
}

// 下に落ちたキャラを消す
void GameScene::dropCharas()
{
    // 15は台の側面の部分
    Rect tableRect = this->tableBottom->boundingBox();
    int tableY = tableRect.getMinY() + 10;

    for (auto itr = charas.begin(); itr != charas.end(); itr++) {
        auto chara = (Chara*)(*itr);
        if (chara->isLowerTable && !chara->isDropping) {
            Rect charaRect = chara->boundingBox();
            int charaY = charaRect.getMinY();
            if (tableY > charaY) {
                chara->isDropping = true;
                chara->drop();
                this->updateCharaCount();
            }
        }
    }
    
}

void GameScene::detectUfoCollision()
{
    Rect tableRect = tableBottom->boundingBox();
    int tableY = tableRect.getMinY();

    for (auto itr = charas.begin(); itr != charas.end(); itr++) {
        auto chara = (Chara*)(*itr);
        if (chara->isLowerTable) {
            Rect charaRect = chara->boundingBox();
            int charaY = charaRect.getMidY();
            if (tableY > charaY && this->ufo->detectCollision(chara) && !chara->isAbducting) {
                log("ufoに衝突!");
                auto cb = CallFunc::create([chara](){
                    chara->removeFromParent();
                });
                this->ufo->abductChara(chara, cb);
            }
        }
    }
}

void GameScene::moveCharas(int dst)
{
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = (Chara*)(*itr);
        if (chara->isUpperTable)
        {
            Vec2 vec = chara->getPosition();
            chara->setPositionY(vec.y + dst);
        }
    }
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    //クリック音
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_put.mp3");

    //Touch 取得
    Point touchPoint = Vec2(touch->getLocationInView().x, touch->getLocationInView().y);
    Rect tableRect = tableTop->getBoundingBox();
    int tableMidY = tableRect.getMidY();
    int charaId = this->getCharaId();
    auto chara = Chara::create(CHARA_DATA[charaId]);
    chara->show(Vec2(touchPoint.x, tableMidY + 50));
    // 先頭に追加
    charas.insert(0, chara);
    this->addChild(chara);
    this->swapZOerder();
    return true;
}

void GameScene::swapZOerder()
{
    int i = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = (Chara*)(*itr);
        this->reorderChild(chara, i);
        i++;
    }
}

// 上のテーブルに押し出される
// 引数はテーブルの移動距離
void GameScene::sweep(int dst)
{
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = (Chara*)(*itr);
        if (chara->isLowerTable)
        {
            Rect tableRect = tableTop->boundingBox();
            int tableY = tableRect.getMinY();
            Rect charaRect = chara->boundingBox();
            int charaY = charaRect.getMaxY();
            if (tableY <= charaY) {
              chara->setPositionY(charaRect.getMidY() - dst);
            }
        }
    }
}

void GameScene::incrementChara()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    int width = (int)visibleSize.width;
    Rect tableRect = GameScene::tableTop->getBoundingBox();
    int tableMidY = tableRect.getMidY();

    for (int i = 0; i < 10; i++) {
        int charaId = this->getCharaId();
        int randX = arc4random() % width;
        auto chara = Chara::create(CHARA_DATA[charaId]);
        chara->show(Vec2(randX, tableMidY + 50));
        // 先頭に追加
        this->charas.insert(0, chara);
        this->addChild(chara);
    }
  
    this->swapZOerder();
}

void GameScene::btnToHomeCallback(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_clicked.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("bgm_game.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm_normal.mp3", true);

    this->setScore();
    
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

void GameScene::updateCharaCount()
{
    this->score -= 1;

    if (this->score <= 0) {
        this->score = FEVER_NUM;
        // スロットが回り終わったあとのコールバック
        // NOTE: スロットが回ってなかったら判定がいるかも
        auto cb = CallFunc::create([this](){
            int eventId = this->slot->getLastEventId();
            log("スロット終わり eventId:%i", eventId);
            this->eventQueue.push_back(eventId);
            this->slot->isRotating = false;
        });

        this->slot->rotate(cb);
    }

    char text[20];
    sprintf(text, "フィーバーまであと%i体", this->score);
    this->scoreLabel->setString(text);
}

int GameScene::getCharaId()
{
    // TODO: キャラ出現ロジック
    int r = arc4random() % 10;
    int num;
    if (r < 8) {
        num = 0;
    } else {
        num = 1;
    }

    return num;
}
