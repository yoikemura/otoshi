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
    
    bool isInUpperTable(Chara* chara);
    void dropFromUpperTable();
    void detectCollision();
    void sweep(int dst);
    void swapZOerder();
    void dropCharas();
    void detectUfoCollision();
        
    // キャラクターを格納する配列
    Vector<Sprite*> charas;
    
    //変数
    int score;
    Label* scoreLabel;
    
    // スロット
    Slot* slot;
    Ufo* ufo;
    
    // テーブル
    Sprite* tableTop;
    Sprite* tableBottom;
    bool isTableFoward = true;
    bool isTableBack = false;
    
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__

