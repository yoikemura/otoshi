//
//  GameScene.h
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//

#ifndef __MaruOtoshi__GameScene__
#define __MaruOtoshi__GameScene__

#include <stdio.h>
#include <random>
#include "cocos2d.h"
#include <vector>
#include "Slot.h"
#include "Ufo.h"
#include "Chara.h"
#include "ProgressBar.h"
#include "Config.h"

using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // Layers
    Layer* gameLayer;
    Layer* slotLayer;
    LayerColor* overlayLaery;   
    
    void backToHome(cocos2d::Ref* pSender);

    // ゲーム管理
    void update(float dt);
    void stopGame();
    void resumeGame();
    
    void removeCharas();
    void moveCharas(int dst);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void updateCharaCount();
    
    bool isInUpperTable(Chara* chara);
    void dropFromUpperTable();
    void detectCollision();
    void sweep(int dst);
    void swapZOrder();
    void dropCharas();
    void detectUfoCollision();
    void incrementChara();
    void popPlus1(int x, int y);
    void popGet(int x, int y);
    void getChara(Chara* chara);
    void sortCharaWithYPosition();
    void showGetRareGomabi(Chara* chara);
    void closePopup(Ref* pSender);

    // share
    void shareWithTwitter(Ref* pSender);
    void shareWithFacebook(Ref* pSender);
    void shareWithLine(Ref* pSender);
    Chara* currentGetChara;
    
    // 上限管理
    int usableGomaCount;
    int loadUsableGomaCount();
    void saveUsableGomaCount();
    void showGameOver();
    Label* scoreLabel;

    // 進捗管理
    bool playing = true;
        
    // キャラクターを格納する配列
    Vector<Sprite*> charas;
    
    // スコア関係
    int score = FEVER_NUM;
    
    // スロット関係
    Slot* slot;
    Ufo* ufo;
    ProgressBar* progressBar;
    bool isSlotRotate = false;

    // キャラID取得関数
    std::string getCharaId();

    // イベント周り
    std::vector<int> eventQueue;
    bool isInEvent = false;
    int eventId;
    
    // テーブル
    Sprite* tableTop;
    Sprite* tableBottom;
    static const int TABLE_TOP_Y = 300;
    bool isTableFoward = true;
    bool isTableBack = false;

    // Util
    CC_SYNTHESIZE(std::mt19937, _engine, Engine);
    float generateRandom(float min, float max);

    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__

