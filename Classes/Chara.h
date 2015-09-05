#ifndef __MaruOtoshi__Chara__
#define __MaruOtoshi__Chara__

#include <iostream>
#include "cocos2d.h"
#include "Config.h"
using namespace cocos2d;

class Chara : public Sprite
{
public:
    Chara(struct CHARA);
    
    struct CHARA charaData;
    
    // ステート
    // 上の台にいる
    bool isUpperStage;
    // 下の台にいる
    bool isLowerStage;
    // 落ちている途中
    bool isDropping;

    // 作成
    static Chara* create(struct CHARA);
    bool initWithFile(const char *pFileName);
    
    void update(float dt);

    // 画面に現れる
    void show();
    // 下に落ちる
    void drop();
    
    // 判定用のNODE
    
    
    
    const char* getName();
};

#endif