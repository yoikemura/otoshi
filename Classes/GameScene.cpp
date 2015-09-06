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
    tableBottom->setPosition(visibleSize.width*0.5, 150);
    this->addChild(tableBottom);
    
    //Table TOP
    tableTop = Sprite::create("table_top.png");
    tableTop->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
    this->addChild(tableTop);

    
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
    score = getScore();
    if (score > 30){
        score = getScore();
        log("getscore is %i", score);
    }else{
        score = 30;
        log("score is %i", score);
    }
    
    scoreLabel = Label::createWithSystemFont(StringUtils::toString(score),"arial", 24);
    scoreLabel->setPosition(Vec2(visibleSize.width*0.9, visibleSize.height*0.9));
    scoreLabel->setColor(Color3B::WHITE);
    this->addChild(scoreLabel);
    
    
    // キャラをばらまく 30体
    Rect rect = tableBottom->getBoundingBox();
    Rect rect2 = tableTop->getBoundingBox();
    int yMin = rect.getMinY();
    int hoge = rect2.getMaxY();
    log("キャラクター配置最低値 MinY: %i", yMin);
    log("キャラクター配置最高いbottom ManY: %i", hoge);

    for(int i = 0; i < 30; i++) {
        srand((unsigned int)time(NULL));
        int randX = arc4random() % ((int)visibleSize.width);

        CHARA charaData = CHARA_DATA[0];
        auto chara = Chara::create(charaData);
        chara->setPosition(Vec2(randX, DEFAULT_CHARA_MAP[i]));
        
        if (this->isInUpperTable(chara)) {
           log("中にいるキャラ");
            chara->isUpperTable = true;
        } else {
            chara->isLowerTable = true;
        }

        charas.pushBack(chara);
        this->addChild(chara);
    }
    
    // add the label as a child to this layer
    this->scheduleUpdate();
    
    slot = Slot::create();
    slot->setPosition(Vec2(visibleSize.width*0.5,
                           visibleSize.height*0.75 - slot->getContentSize().height));
    
    ufo = Ufo::create();
    ufo->setPosition(Vec2(0, ufo->getBoundingBox().size.height));
    
    this->addChild(slot);
    this->addChild(ufo);
    
    // UFOを永遠に左右に動かす
    MoveTo* gogo =  MoveTo::create(3.0f, Point(visibleSize.width, ufo->getBoundingBox().size.height));
    MoveTo* goback = MoveTo::create(3.0f, Point(0, ufo->getBoundingBox().size.height));
    auto spawn = Spawn::create(gogo, goback, NULL);
    auto repeatForever = RepeatForever::create(spawn);
    ufo->runAction(repeatForever);
    
    return true;
}

void GameScene::update(float dt)
{
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

    if (tableY == 250) {
        isTableBack = false;
        isTableFoward = true; 
    } 

    if(tableY == 200) {
        isTableBack = true;
        isTableFoward = false; 
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

            auto chara1 = (Chara*)charas.at(i);
            auto chara2 = (Chara*)charas.at(j);

            // TODO: 処理軽く
            /*
            if (!((chara1->isUpperTable && chara2->isUpperTable) ||
                (chara1->isLowerTable && chara2->isLowerTable))) {
                j++;
                continue;
            }
            */

            // 配列の先頭程奥にある
            // 奥にあるので自分より手前(配列のindexが大きい物)に対してのみ衝突判定をするべき
            // 奥の物が前のものを押し出すイメージ
            if (j > i) {
                Rect rect1 = chara1->boundingBox();
                Rect rect2 = chara2->boundingBox();

                // 斜辺
                float delt = rect2.size.width * rect2.size.width;

                float ab_x = rect1.getMidX() - rect2.getMidX();
                float ab_y = rect1.getMidY() - rect2.getMidY();

                if (ab_x * ab_x +  ab_y * ab_y < delt)  {
                    Vec2 vec = chara2->getPosition();
                    // めり込んだ量
                    float len = sqrtf(ab_x * ab_x + ab_y * ab_y);
                    // float dist = rect2.size.width + rect2.size.width - len;
                    float dist = 2.0f;
                    float angle = atan2f(ab_y, ab_x);
                    float nx = cosf(angle) * dist;
                    float ny = sinf(angle) * dist;
                    
                    // 少なくとも前に進ませる！
                    if (ny <= 0) {
                        ny = 1.0f;
                    }

                    // 自分より手前(chara2)を移動させる
                    // 移動量はchara1とのめり込み分
                    chara2->setPosition(Vec2(vec.x - nx, vec.y - ny));
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
    int i = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = (Chara*)charas.at(i);
        if (chara->isUpperTable)
        {
            if (!this->isInUpperTable(chara)) {
              log("上テーブルから落ちる");
              chara->isUpperTable = false;
              chara->isLowerTable = true;
            }
        }
        
        i++;
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
    auto itr2 = charas.begin();
    while (itr2 != charas.end()) {
        auto chara = (*itr2);
        int y = chara->getPositionY();

        if (y <= -30) {
            itr2 = charas.erase(itr2);
            this->removeChild(chara);
        }
        else
        {
            itr2++;
        }
    }
}

// 下に落ちたキャラを消す
void GameScene::dropCharas()
{
    
    int i = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++) {
        auto chara = (Chara*)charas.at(i);
        if (chara->isLowerTable) {
            Rect tableRect = tableBottom->boundingBox();
            int tableY = tableRect.getMinY();
            Rect charaRect = chara->boundingBox();
            int charaY = charaRect.getMidY();
            if (tableY > charaY) {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_drop.mp3");
                chara->isDropping = true;
                chara->drop();
                this->updateScore();
            }
        }
        i++;
    }
    
}

void GameScene::detectUfoCollision()
{
    int i = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++) {
        auto chara = (Chara*)charas.at(i);
        if (chara->isLowerTable) {
            Rect tableRect = tableBottom->boundingBox();
            int tableY = tableRect.getMinY();
            Rect charaRect = chara->boundingBox();
            int charaY = charaRect.getMidY();
            if (tableY > charaY) {
                Point charaPoint = chara->getPosition();

                Point ufoPoint = ufo->getPosition();
                
                float dif_f = ufoPoint.x - charaPoint.x;
                int dif_i;
                dif_i = dif_f;
                
                //UFOとの衝突判定スタート
                if (abs(dif_i) < 10){
                    //log("衝突GREAT");
                    slot->rotation();
                }
            }
        }
        i++;
    }
    
}

void GameScene::moveCharas(int dst)
{
    int i = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = (Chara*)charas.at(i);
        if (chara->isUpperTable)
        {
            Vec2 vec = chara->getPosition();
            chara->setPositionY(vec.y + dst);
        }
        
        i++;
    }
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    //クリック音
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_put.mp3");

    //合計値マイナス
    score -= 1;
    scoreLabel->setString(StringUtils::toString(score));
    
    //Touch 取得
    Point touchPoint = Vec2(touch->getLocationInView().x, touch->getLocationInView().y);
    
    Rect tableRect = tableTop->getBoundingBox();
    int tableMaxY = tableRect.getMaxY();

    int r = arc4random() % 10;
    int num;
    if (r < 8) {
        num = 0;
    }else{
        num = 1;
    }

    CHARA charaData = CHARA_DATA[num];
    auto chara = Chara::create(charaData);
    chara->setPosition(Vec2(touchPoint.x, tableMaxY + 50));
    // 先頭に追加
    charas.insert(0, chara);
    // あとから追加されたやつは絶対上のテーブル
    chara->isUpperTable = true;
    
    this->addChild(chara);
    
    MoveTo* fallDown =  MoveTo::create(0.1f, Point(touchPoint.x, tableMaxY));
    chara->runAction(fallDown);
    
    this->swapZOerder();
    
    return true;
}

void GameScene::swapZOerder()
{
    int i = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = (Chara*)charas.at(i);
        this->reorderChild(chara, i);
        i++;
    }
}



// 上のテーブルに押し出される
// 引数はテーブルの移動距離
void GameScene::sweep(int dst)
{
    int i = 0;
    for (auto itr = charas.begin(); itr != charas.end(); itr++)
    {
        auto chara = (Chara*)charas.at(i);
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
        i++;
    }
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

int GameScene::updateScore()
{
    score += 1;
    scoreLabel->setString(StringUtils::toString(score));
}

