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
#include "cocos2d.h"
#include <vector>
#include "Slot.h"
#include "Ufo.h"
#include "Chara.h"
#include "Config.h"

using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void btnToHomeCallback(cocos2d::Ref* pSender);

    void update(float dt);
    void removeCharas();
    void moveCharas(int dst);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void setScore();
    int getScore();
    void updateCharaCount();
    
    bool isInUpperTable(Chara* chara);
    void dropFromUpperTable();
    void detectCollision();
    void sweep(int dst);
    void swapZOerder();
    void dropCharas();
    void detectUfoCollision();
    void incrementChara();
    void popPlus1();
        
    // キャラクターを格納する配列
    Vector<Sprite*> charas;
    
    //変数
    int score = FEVER_NUM;
    Label* scoreLabel;
    
    // スロット
    Slot* slot;
    Ufo* ufo;

    // キャラID取得関数
    int getCharaId();

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

    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__

