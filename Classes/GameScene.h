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

using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    void update(float dt);
    void setScore();
    int getScore();
        
    // キャラクターを格納する配列
    Vector<Sprite*> charas;
    void btnToHomeCallback(cocos2d::Ref* pSender);
    
    // スロット
    Slot* slot;
    Ufo* ufo;
    
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__

