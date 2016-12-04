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

    // NOTE:キャラ配置マップを作るためのログ
    int tb = tableBottom->getBoundingBox().getMinY();
    int tt = tableTop->getBoundingBox().getMaxY();
    log("table top maxY: %i, table bottom minY: %i", tt, tb);
    
    // ホーム画面へ移動ボタン
    auto btnToHome = MenuItemImage::create("back.png",
                                           "back.png",
                                           CC_CALLBACK_1(GameScene::btnToHomeCallback, this));
    
    btnToHome->setPosition(Vec2(visibleSize.width,
                                origin.y + btnToHome->getContentSize().height/2));
    
    auto menu = Menu::create(btnToHome, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

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
    // ポップアップが出ている場合など
    if (!this->playing) { return; };

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
            this->popPlus1(chara->getBoundingBox().getMidX());
        } else {
            itr++;
        }
    }
}

// 下に落ちたキャラを消す
void GameScene::dropCharas()
{
    // 10は台の側面の部分
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
                auto cb = CallFunc::create([this, chara](){
                    Rect ufoRect = this->ufo->getBoundingBox();
                    this->popGet(ufoRect.getMidX(), ufoRect.getMidY());
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
    log("touch!!");
    //クリック音
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_put.mp3");

    //Touch 取得
    Point touchPoint = Vec2(touch->getLocationInView().x, touch->getLocationInView().y);
    Rect tableRect = tableTop->getBoundingBox();
    int tableMidY = tableRect.getMidY();
    int charaId = this->getCharaIdx();
    log("chara %s", CHARA_DATA[charaId].name);
    auto chara = Chara::create(CHARA_DATA[charaId]);
    chara->show(Vec2(touchPoint.x, tableMidY + 50));
    // 先頭に追加
    charas.insert(0, chara);
    this->addChild(chara);
    this->swapZOrder();
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
        int charaId = this->getCharaIdx();
        int randX = arc4random() % width;
        auto chara = Chara::create(CHARA_DATA[charaId]);
        chara->show(Vec2(randX, tableMidY + 50));
        // 先頭に追加
        this->charas.insert(0, chara);
        this->addChild(chara);
    }
  
    this->swapZOrder();
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

void GameScene::popPlus1(int x)
{
    int idx = arc4random() % 3 + 1;
    auto plus1 = Sprite::create(PLUS1_IMAGE[idx]);
    plus1->setPosition(x, -5);
    this->addChild(plus1);
    MoveTo* move =  MoveTo::create(0.3f, Vec2(x, 35));
    auto remove = RemoveSelf::create(true);
    plus1->runAction(Sequence::create(move, remove, NULL));
}

void GameScene::popGet(int x, int y)
{
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
        // キャラクター取得のポップアップ
        this->showGetRareGomabi(chara);
    }
}

void GameScene::updateCharaCount()
{
    this->score -= 1;

    // 進捗
    float feverRate = ((float)(FEVER_NUM - this->score)) / (float)(FEVER_NUM);
    this->progressBar->setWidth(feverRate);

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
}

int GameScene::getCharaIdx()
{
    // TODO: レベル設計
    float r = this->generateRandom(0, 1);

    int idx;
    // レア出現率
    if (r < RARE_PROBABILITY_RATE) {
        idx = arc4random() % 10 + 1;
    } else {
        idx = 0;
    }

    return idx;
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
    auto fileName = chara->getExplainFimeName();
    Sprite* explainImage = Sprite::create(fileName);
    explainImage->setPosition(Point(139.0, 235.0));
    popup->addChild(explainImage);
    
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
  sprintf(tweet , "「%s」を捕獲！！ ", this->currentGetChara->getName());
  NativeLauncher::openTweetDialog(tweet, this->currentGetChara->getFileName());
}

void GameScene::shareWithFacebook(Ref* pSender)
{
}

void GameScene::shareWithLine(Ref* pSender)
{
}

