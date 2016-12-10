//
//  GameScene.cpp
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//
#include <time.h>
#include <stdio.h>
#include "GameScene.h"
#include "HomeScene.h"
#include "Config.h"
#include "Chara.h"
#include "ProgressBar.h"
#include "SimpleAudioEngine.h"
#include "LibraryManager.h"
#include "NativeLauncher.h"

#define call_after(callback, delay) \
runAction(Sequence::create(DelayTime::create(delay), CallFunc::create(callback), NULL))

template
<
    typename TYPE,
    int SIZE
>
int arrayLength(const TYPE (&)[SIZE])
{
    return SIZE;
}

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
    if ( !Layer::init() )
    {
        return false;
    }
    
    // 利用可能ゴマビィの設定
    this->usableGomaCount = GOMA_LIMIT;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // BACK GROUND LAYER
    auto bgLayer = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
    this->addChild(bgLayer);
    auto centerpos = visibleSize / 2;
    auto spritebg = Sprite::create("bg.png");
    spritebg->setPosition(centerpos);
    bgLayer->addChild(spritebg);
    
    // ProgressBar
    this->progressBar = ProgressBar::create();
    this->progressBar->setPosition(visibleSize.width - 105, visibleSize.height - 31);
    this->addChild(this->progressBar);
    
    // Table BOTTOM
    tableBottom = Sprite::create("table_under.png");
    tableBottom->setPosition(visibleSize.width*0.5, 210);
    this->addChild(tableBottom);
    
    // Table TOP
    tableTop = Sprite::create("table_top.png");
    tableTop->setPosition(visibleSize.width*0.5, TABLE_TOP_Y);
    this->addChild(tableTop);
    
    // スコアを記述する。
    int score = getScore();
    std::string score_str = std::to_string(score);
    this->scoreLabel = Label::createWithSystemFont(score_str, "HiraKakuProN-W6", 24);
    this->scoreLabel->setPosition(visibleSize.width*0.3, visibleSize.height*0.95);
    this->addChild(scoreLabel);
    
    // ホーム画面へ移動ボタン
    auto startHome = MenuItemImage::create(
                                           "btn_back.png", "btn_back_on.png",
                                           CC_CALLBACK_1(GameScene::backToHome, this));
    Menu* pMenu = Menu::create(startHome, NULL);
    pMenu->setPosition(visibleSize.width*0.1, visibleSize.height*0.95);
    pMenu->alignItemsHorizontally();
    this->addChild(pMenu);
    
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
    this->slot->setVisible(false);
    
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


// ゲーム再開
void GameScene::resumeBg()
{
    this->playing = true;
}

// ゲーム停止
void GameScene::stopBg()
{
    this->playing = false;
}

void GameScene::update(float dt)
{
    std::string score_str = std::to_string(this->usableGomaCount);
    this->scoreLabel->setString(score_str);
    
    // ポップアップが出ている場合など
    if (!this->playing) { return; };
    
    if (this->usableGomaCount == 0) {
        showGameOver();
    }
    
    // イベントキューに値があればイベントスタート
    // 何かしらのイベント終了時にはisInEventをfalseにして終了すること
    if (!this->isInEvent &&
        this->eventQueue.size() > 0) {
        auto itr = this->eventQueue.begin();
        this->eventId = *this->eventQueue.erase(itr);
        
        // NOTE: 実装済み
        // - 台が伸びるのみ実装
        // - キャラ増殖
        if (this->eventId == EVENT_LOGN || this->eventId == EVENT_INCREMENT) {
            this->isInEvent = true;
        }
    }
    
    // キャラの前後関係を整理
    this->sortCharaWithYPosition();
    
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

void GameScene::sortCharaWithYPosition()
{
    sort(this->charas.begin(),
         this->charas.end(),
         [](Sprite* a, Sprite* b){
             float aY = a->getBoundingBox().getMinY();
             float bY = b->getBoundingBox().getMinY();
             return aY > bY;}
         );
    
    this->swapZOrder();
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
                Rect rect1 = chara1->getBoundingBox();
                Rect rect2 = chara2->getBoundingBox();
                
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
                    
                    // 落下中は衝突判定しない
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
            // this->popPlus1(chara->getBoundingBox().getMidX());
        } else {
            itr++;
        }
    }
}

// 下に落ちたキャラを消す
void GameScene::dropCharas()
{
    // 10は台の側面の部分
    Rect tableRect = this->tableBottom->getBoundingBox();
    int tableY = tableRect.getMinY() + 10;
    
    for (auto itr = charas.begin(); itr != charas.end(); itr++) {
        auto chara = (Chara*)(*itr);
        if (chara->isLowerTable && !chara->isDropping) {
            Rect charaRect = chara->getBoundingBox();
            int charaY = charaRect.getMinY();
            if (tableY > charaY) {
                chara->isDropping = true;
                chara->drop();
            }
        }
    }
}

void GameScene::detectUfoCollision()
{
    Rect tableRect = tableBottom->getBoundingBox();
    int tableY = tableRect.getMinY();
    
    for (auto itr = charas.begin(); itr != charas.end(); itr++) {
        auto chara = (Chara*)(*itr);
        if (chara->isLowerTable) {
            Rect charaRect = chara->getBoundingBox();
            int charaY = charaRect.getMidY();
            if (tableY > charaY && this->ufo->detectCollision(chara) && !chara->isAbducting) {
                auto cb = CallFunc::create([this, chara](){
                    Rect ufoRect = this->ufo->getBoundingBox();
                    const char* charaId = chara->getId().c_str();
                    auto libraryManager = LibraryManager::getInstance();
                    if (libraryManager->hasGotten(charaId)) {
                        // 取得済の場合は+1
                        this->popPlus1(ufoRect.getMidX(), ufoRect.getMidY());
                    } else {
                        // 未取得の場合はGet!
                        this->popGet(ufoRect.getMidX(), ufoRect.getMidY());
                    }
                    
                    // フィーバー用のバーを伸ばす
                    this->updateCharaCount();
                    
                    // キャラを消す
                    chara->removeFromParent();
                });
                this->ufo->abductChara(chara, cb);
                this->getChara(chara);
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
    if (this->usableGomaCount <= 0) {
        return true;
    }
    
    //クリック音
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_put.mp3");
    
    //Touch 取得
    Point touchPoint = Vec2(touch->getLocationInView().x, touch->getLocationInView().y);
    Rect tableRect = tableTop->getBoundingBox();
    int tableMidY = tableRect.getMidY();
    std::string charaId = this->getCharaId();
    CHARA charaTmp;
    int charaCount = arrayLength(CHARA_DATA);
    for (int i = 0; i < charaCount; i++) {
        if (CHARA_DATA[i].id == charaId) {
            charaTmp = CHARA_DATA[i];
            break;
        }
    }
    auto chara = Chara::create(charaTmp);
    chara->show(Vec2(touchPoint.x, tableMidY + 50));
    // 先頭に追加
    charas.insert(0, chara);
    this->addChild(chara);
    this->swapZOrder();
    
    this->usableGomaCount--;
    
    return true;
}

void GameScene::swapZOrder()
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
            Rect tableRect = tableTop->getBoundingBox();
            int tableY = tableRect.getMinY();
            Rect charaRect = chara->getBoundingBox();
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
        std::string charaId = this->getCharaId();
        CHARA charaTmp;
        int charaCount = arrayLength(CHARA_DATA);
        for (int i = 0; i < charaCount; i++) {
            if (CHARA_DATA[i].id == charaId) {
                charaTmp = CHARA_DATA[i];
                break;
            }
        }
        auto chara = Chara::create(charaTmp);
        int randX = arc4random() % width;
        chara->show(Vec2(randX, tableMidY + 50));
        // 先頭に追加
        this->charas.insert(0, chara);
        this->addChild(chara);
    }
    
    this->swapZOrder();
}

void GameScene::backToHome(Ref* pSender)
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

void GameScene::popPlus1(int x, int y)
{
    int idx = arc4random() % 3 + 1;
    auto plus1 = Sprite::create(PLUS1_IMAGE[idx]);
    plus1->setPosition(x, y);
    this->addChild(plus1);
    MoveTo* move =  MoveTo::create(0.3f, Vec2(x, 35));
    auto remove = RemoveSelf::create(true);
    plus1->runAction(Sequence::create(move, remove, NULL));
}

void GameScene::popGet(int x, int y)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get.mp3");
    auto get = Sprite::create("get.png");
    get->setPosition(x, y);
    this->addChild(get);
    MoveTo* move =  MoveTo::create(0.3f, Vec2(x, y+20));
    auto easeAction = EaseOut::create(move, 2);
    auto remove = RemoveSelf::create(true);
    get->runAction(Sequence::create(easeAction, remove, NULL));
}

void GameScene::getChara(Chara* chara)
{
    auto libraryManager = LibraryManager::getInstance();
    const char* charaId = chara->getId().c_str();
    if (!libraryManager->hasGotten(charaId)) {
        libraryManager->save(charaId);
        // 取得していない場合キャラクター取得のポップアップを表示
        this->showGetRareGomabi(chara);
    }
    
    // 利用可能キャラ数を増やす
    this->usableGomaCount++;
}

void GameScene::updateCharaCount()
{
    this->score -= 4;
    
    // 進捗
    float feverRate = ((float)(FEVER_NUM - this->score)) / (float)(FEVER_NUM);
    this->progressBar->setWidth(feverRate);
    
    if (this->score <= 0) {
        this->score = FEVER_NUM;
        // スロットが回り終わったあとのコールバック
        // NOTE: スロットが回ってなかったら判定がいるかも
        auto cb = CallFunc::create([this](){
            int eventId = this->slot->getLastEventId();
            this->eventQueue.push_back(eventId);
            this->slot->isRotating = false;
            call_after([this](){
                this->slot->setVisible(false);
            }, 1.5);
        });
        
        this->slot->setVisible(true);
        this->slot->rotate(cb);
    }
}

std::string GameScene::getCharaId()
{
    int selectRate = 0;
    
    int charaCount = arrayLength(CHARA_DATA);
    for (int i = 0; i < charaCount; i++) {
        CHARA chara = CHARA_DATA[i];
        selectRate += chara.rarity;
    }
    
    // 出現率上限
    float r = this->generateRandom(0, selectRate);
    
    int sumRate = 0;
    std::string charaId;
    for (int i = 0; i < charaCount; i++) {
        CHARA chara = CHARA_DATA[i];
        sumRate += chara.rarity;
        charaId  = chara.id;
        
        if (r < sumRate) {
            break;
        }
    }
    
    return charaId;
}

float GameScene::generateRandom(float min, float max)
{
    std::uniform_real_distribution<float> dest(min, max);
    return dest(_engine);
}

void GameScene::showGetRareGomabi(Chara* chara)
{
    // 現在取得したキャラを保存
    this->currentGetChara = chara;
    // ゲームシーンを止める
    this->stopBg();
    
    Size size = Director::getInstance()->getWinSize();
    this->overlayLaery = LayerColor::create(Color4B::BLACK);
    this->overlayLaery->setOpacity(128);
    this->overlayLaery->setContentSize(size);
    this->addChild(this->overlayLaery, 1000);
    
    auto popup = Sprite::create("popup_bg.png");
    popup->setPosition(Point(size.width*0.5, size.height*0.5));
    popup->setCascadeOpacityEnabled(true);
    popup->setOpacity(0);
    
    // 閉じるボタン
    auto btnClose = MenuItemImage::create("popup_close.png",
                                          "popup_close.png",
                                          CC_CALLBACK_1(GameScene::closePopup, this));
    Menu* pMenu = Menu::create(btnClose, NULL);
    pMenu->setPosition(Point(221.0, 30.0));
    popup->addChild(pMenu);
    
    // Twitterボタン
    auto pTwitterItem = MenuItemImage::create("popup_tw.png",
                                              "popup_tw.png",
                                              CC_CALLBACK_1(GameScene::shareWithTwitter, this));
    
    Menu* pMenuTwitter = Menu::create(pTwitterItem, NULL);
    pMenuTwitter->setPosition(Point(33.0, 30.0));
    popup->addChild(pMenuTwitter);
    
    /*
     // Facebookボタン
     auto pFacebookItem = MenuItemImage::create("popup_fb.png",
     "popup_fb.png",
     CC_CALLBACK_1(GameScene::shareWithFacebook, this));
     Menu* pMenuFacebook = Menu::create(pFacebookItem, NULL);
     pMenuFacebook->setPosition(Point(86.0, 30.0));
     popup->addChild(pMenuFacebook);
     
     // Lineボタン
     auto pLineItem = MenuItemImage::create("popup_line.png",
     "popup_line.png",
     CC_CALLBACK_1(GameScene::shareWithLine, this));
     
     
     Menu* pMenuLine = Menu::create(pLineItem, NULL);
     pMenuLine->setPosition(Point(140.0, 30.0));
     popup->addChild(pMenuLine);
     */
    
    // キャラ画像
    auto fileName = chara->getFileName();
    Sprite* charaImage = Sprite::create(fileName);
    charaImage->setPosition(Point(139.0, 250.0));
    popup->addChild(charaImage);
    
    //キャラ名
    auto name = Label::createWithSystemFont(chara->getName(), "HiraKakuProN-W6", 24);
    name->setWidth(260);
    name->setColor(Color3B(0, 0, 0));
    name->setPosition(Point(139.0, 200.0));
    popup->addChild(name);
    
    
    // キャラ説明
    auto charaDesc = Label::createWithSystemFont(chara->getDescription(), "HiraKakuProN-W6", 12);
    charaDesc->setWidth(260);
    charaDesc->setColor(Color3B(0, 0, 0));
    charaDesc->setPosition(Point(139.0, 131.0));
    popup->addChild(charaDesc);
    
    // コンプリートまでxx対
    char str[16];
    auto charaId = chara->getId();
    int charInt = std::stoi(charaId);
    int restId = 10 - charInt;
    sprintf(str,"コンプリートまで残り%d種類", restId);
    auto completeLabel = Label::createWithSystemFont(str, "HiraKakuProN-W6", 12, Size(545, 32), TextHAlignment::CENTER);
    completeLabel->setWidth(260);
    completeLabel->setColor(Color3B(0, 0, 0));
    completeLabel->setPosition(Point(139.0, 75.0));
    popup->addChild(completeLabel);
    
    ActionInterval *action = FadeIn::create(0.3);
    popup->runAction(action);
    this->overlayLaery->addChild(popup);
}


//GAMEOVERになったら出るポップアップ
void GameScene::showGameOver()
{
    // ゲームシーンを止める
    this->stopBg();
    
    Size size = Director::getInstance()->getWinSize();
    this->overlayLaery = LayerColor::create(Color4B::BLACK);
    this->overlayLaery->setOpacity(128);
    this->overlayLaery->setContentSize(size);
    this->addChild(this->overlayLaery, 1000);
    
    auto popup = Sprite::create("popup_bg.png");
    popup->setPosition(Point(size.width*0.5, size.height*0.5));
    popup->setCascadeOpacityEnabled(true);
    popup->setOpacity(0);
    
    // 閉じるボタン
    auto btnClose = MenuItemImage::create("popup_close.png",
                                          "popup_close.png",
                                          CC_CALLBACK_1(GameScene::backToHome, this));
    Menu* pMenu = Menu::create(btnClose, NULL);
    pMenu->setPosition(Point(221.0, 30.0));
    popup->addChild(pMenu);
    
    ActionInterval *action = FadeIn::create(0.3);
    popup->runAction(action);
    this->overlayLaery->addChild(popup);
}

// ゲーム再開
void GameScene::closePopup(Ref* pSender)
{
    if (this->overlayLaery != NULL) {
        this->removeChild(this->overlayLaery);
        this->overlayLaery = NULL;
    }
    
    this->resumeBg();
}

void GameScene::shareWithTwitter(Ref* pSender)
{
    char tweet[500];
    NativeLauncher::openTweetDialog(tweet, this->currentGetChara->getFileName());
}

void GameScene::shareWithFacebook(Ref* pSender)
{
}

void GameScene::shareWithLine(Ref* pSender)
{
}

